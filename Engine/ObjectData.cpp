#include "ObjectData.h"

namespace Dusk
{

ObjectData::ObjectData()
{
  m_ReferenceList.clear();
}

ObjectData::~ObjectData()
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
  output.write("Dusk", 4);
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
      Stream->write("RefO", 4); //header
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
  unsigned int count, i, len;
  char ID_Buffer[256];
  float x, y, z, rx, ry, rz, scl;
  DuskObject * objPtr = NULL;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "ObjectData::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  char Header[4];
  Header[0] = Header[1] = Header[2] = Header[3] = '\0';

  //read header "Dusk"
  input.read(Header, 4);
  if ((Header[0]!='D') || (Header[1]!='u') || (Header[2]!='s') || (Header[3]!='k'))
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
    //read header "Refr"
    input.read(Header, 4);
    if ((Header[0]!='R') || (Header[1]!='e') || (Header[2]!='f') || (Header[3]!='O'))
    {
      std::cout << "ObjectData::LoadFromFile: ERROR: File contains invalid "
                << "reference header.\n";
      input.close();
      return false;
    }
    //read ID
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ObjectData::LoadFromFile: ERROR: ID cannot be longer than "
                << "255 characters.\n";
      input.close();
      return false;
    }
    input.read(ID_Buffer, len);
    ID_Buffer[len] = '\0';
    if (!input.good())
    {
      std::cout << "ObjectData::LoadFromFile: ERROR while reading data (ID).\n";
      input.close();
      return false;
    }
    //position
    input.read((char*) &x, sizeof(float));
    input.read((char*) &y, sizeof(float));
    input.read((char*) &z, sizeof(float));

    //rotation
    input.read((char*) &rx, sizeof(float));
    input.read((char*) &ry, sizeof(float));
    input.read((char*) &rz, sizeof(float));

    //scale
    input.read((char*) &scl, sizeof(float));

    if (!input.good())
    {
      std::cout << "ObjectData::LoadFromFile: ERROR while reading data "
                << "(position, rotation or scale).\n";
      input.close();
      return false;
    }

    objPtr = new DuskObject(std::string(ID_Buffer), Ogre::Vector3(x,y,z),
                            Ogre::Vector3(rx,ry,rz), scl);
    m_ReferenceList.push_back(objPtr);
  }//for
  input.close();
  return false;
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


}//namespace
