#include "ObjectData.h"
#include "DuskConstants.h"

namespace Dusk
{

ObjectData::ObjectData()
{
  m_ReferenceList.clear();
}

ObjectData::~ObjectData()
{
  ClearData();
}

ObjectData& ObjectData::GetSingleton()
{
  static ObjectData Instance;
  return Instance;
}

unsigned int ObjectData::NumberOfReferences() const
{
  return m_ReferenceList.size();
}

DuskObject* ObjectData::addReference(const std::string ID,
    const Ogre::Vector3 position, const Ogre::Vector3 rotation, const float scale)
{
  DuskObject * ObjectPointer = new DuskObject(ID, position, rotation, scale);
  m_ReferenceList.push_back(ObjectPointer);
  return ObjectPointer;
}


bool ObjectData::SaveToFile(const std::string FileName)
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "ObjectData::SaveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  unsigned int len;
  bool success = false;

  len = m_ReferenceList.size();
  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //number of elements to write (and later to read, on loading)
  output.write((char*) &len, sizeof(unsigned int));
  success = SaveToStream(&output);
  output.close();
  return success;
}

bool ObjectData::SaveToStream(std::ofstream* Stream)
{
  unsigned int i, len;
  float x,y,z;

  for (i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      //write header "RefO" (reference of Object)
      Stream->write((char*) &cHeaderRefO, sizeof(unsigned int)); //header
      //write ID
      len = m_ReferenceList.at(i)->GetID().length();
      Stream->write((char*) &len, sizeof(unsigned int));
      Stream->write(m_ReferenceList.at(i)->GetID().c_str(), len);

      //write position and rotation
      x = m_ReferenceList.at(i)->GetPosition().x;
      y = m_ReferenceList.at(i)->GetPosition().y;
      z = m_ReferenceList.at(i)->GetPosition().z;
      Stream->write((char*) &x, sizeof(float));
      Stream->write((char*) &y, sizeof(float));
      Stream->write((char*) &z, sizeof(float));
      x = m_ReferenceList.at(i)->GetRotation().x;
      y = m_ReferenceList.at(i)->GetRotation().y;
      z = m_ReferenceList.at(i)->GetRotation().z;
      Stream->write((char*) &x, sizeof(float));
      Stream->write((char*) &y, sizeof(float));
      Stream->write((char*) &z, sizeof(float));
      //scale
      x = m_ReferenceList.at(i)->GetScale();
      Stream->write((char*) &x, sizeof(float));
      //everything OK?
      if (!Stream->good())
      {
        std::cout << "ObjectData::SaveToStream: ERROR while writing reference data.\n";
        return false;
      }
    }//if
  }//for
  return true;
}

bool ObjectData::LoadFromFile(const std::string FileName)
{
  std::ifstream input;
  unsigned int count, i;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "ObjectData::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  unsigned int Header;
  Header = 0;

  //read header "Dusk"
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDusk)
  {
    std::cout << "ObjectData::LoadFromFile: ERROR: File contains invalid "
              << "file header.\n";
    input.close();
    return false;
  }
  count = 0;
  //read how many objects we have in file
  input.read((char*) &count, sizeof(unsigned int));
  for (i=0; i<count; i++)
  {
    if (!LoadFromStream(&input))
    {
      std::cout << "ObjectData::LoadFromFile: ERROR while reading record.\n";
      input.close();
      return false;
    }
  }//for
  input.close();
  return true;
}

bool ObjectData::LoadFromStream(std::ifstream* Stream)
{
  static char ID_Buffer[256];
  float x, y, z, rx, ry, rz, scl;
  DuskObject * objPtr = NULL;
  unsigned int Header, len;

  //read header "RefO"
  Header = 0;
  Stream->read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefO)
  {
    std::cout << "ObjectData::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read ID
  Stream->read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ObjectData::LoadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  Stream->read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!Stream->good())
  {
    std::cout << "ObjectData::LoadFromStream: ERROR while reading data (ID).\n";
    return false;
  }
  //position
  Stream->read((char*) &x, sizeof(float));
  Stream->read((char*) &y, sizeof(float));
  Stream->read((char*) &z, sizeof(float));

  //rotation
  Stream->read((char*) &rx, sizeof(float));
  Stream->read((char*) &ry, sizeof(float));
  Stream->read((char*) &rz, sizeof(float));

  //scale
  Stream->read((char*) &scl, sizeof(float));

  if (!Stream->good())
  {
    std::cout << "ObjectData::LoadFromStream: ERROR while reading data "
              << "(position, rotation or scale).\n";
    return false;
  }

  objPtr = new DuskObject(std::string(ID_Buffer), Ogre::Vector3(x,y,z),
                          Ogre::Vector3(rx,ry,rz), scl);
  m_ReferenceList.push_back(objPtr);
  return true;
}

void ObjectData::EnableAllObjects(Ogre::SceneManager * scm)
{
  unsigned int i;
  if (scm==NULL)
  {
    std::cout << "ObjectData::EnableAllObjects: ERROR: Scene Manager is NULL!\n";
    return;
  }
  for(i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      m_ReferenceList.at(i)->Enable(scm);
    }
  }//for
}

void ObjectData::DisableAllObjects()
{
  unsigned int i;
  for(i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      m_ReferenceList.at(i)->Disable();
    }
  }//for
}

unsigned int ObjectData::deleteReferencesOfObject(const std::string ID)
{
  unsigned int deletedReferences = 0;
  long int i;
  for (i=m_ReferenceList.size()-1; i>=0; i--)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      if (m_ReferenceList.at(i)->GetID()==ID)
      {
        delete m_ReferenceList.at(i);
        deletedReferences++;
        //swap position of deleted element mith one at the end
        // (needed later do do only one call to erase)
        m_ReferenceList.at(i) = m_ReferenceList.at(m_ReferenceList.size()-deletedReferences);
        m_ReferenceList.at(m_ReferenceList.size()-deletedReferences) = NULL;
      }//if
    }//if
  }//for
  if (deletedReferences >0)
  {
    m_ReferenceList.erase(m_ReferenceList.begin()+m_ReferenceList.size()-deletedReferences, m_ReferenceList.end());
  }//if
  return deletedReferences;
}

///method to update all enabled objects of one ID after the mesh path has changed
/// currently only used by Editor
unsigned int ObjectData::reenableReferencesOfObject(const std::string ID, Ogre::SceneManager * scm)
{
  unsigned int re_enabled = 0;
  unsigned int position;

  if (scm==NULL)
  {
    std::cout << "ObjectData::reenableReferencesOfObject: ERROR: Scene Manager is NULL pointer!";
    return 0;
  }
  for (position = 0; position<m_ReferenceList.size(); position++)
  {
    if (m_ReferenceList.at(position)!=NULL)
    {
      if (m_ReferenceList.at(position)->GetID()==ID && m_ReferenceList.at(position)->IsEnabled())
      {
        m_ReferenceList.at(position)->Disable();
        m_ReferenceList.at(position)->Enable(scm);
        re_enabled++;
      }//if
    }//if
  }//for
  return re_enabled;
}

void ObjectData::ClearData()
{
  DuskObject * ObjPtr;
  while(!m_ReferenceList.empty())
  {
    ObjPtr = m_ReferenceList.back();
    ObjPtr->Disable();
    delete ObjPtr;
    m_ReferenceList.pop_back();
  }//while
  m_ReferenceList.clear();
}//clear data

}//namespace
