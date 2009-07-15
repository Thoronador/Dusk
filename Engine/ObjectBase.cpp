#include "ObjectBase.h"
#include "DuskConstants.h"
#include <iostream>

namespace Dusk
{

ObjectBase::ObjectBase()
{
  m_ObjectList = std::map<std::string,std::string>();
}

ObjectBase::~ObjectBase()
{
  m_ObjectList.clear();
}

ObjectBase& ObjectBase::GetSingleton()
{
  static ObjectBase Instance;
  return Instance;
}

bool ObjectBase::hasObject(const std::string NameOfObject) const
{
  return (m_ObjectList.find(NameOfObject) != m_ObjectList.end());
}

void ObjectBase::addObject(const std::string ID, const std::string Mesh)
{
  if (ID=="" or Mesh=="")
  {
    std::cout << "ObjectBase::addObject: ERROR: ID or Mesh is empty string.\n";
    return;
  }
  m_ObjectList[ID] = Mesh;
}

void ObjectBase::ClearAllObjects()
{
  m_ObjectList.clear();
}

unsigned int ObjectBase::NumberOfObjects() const
{
  return m_ObjectList.size();
}

std::string ObjectBase::GetMeshName(const std::string ID, const bool UseMarkerOnError) const
{
  if (hasObject(ID))
  {
    return m_ObjectList.find(ID)->second;
  }
  //if we get to this point, object is not present in list
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  else
  {
    return "";
  }
}

bool ObjectBase::SaveToFile(const std::string FileName)
{
  std::ofstream output;
  bool success = false;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "ObjectBase::SaveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  unsigned int i;
  i = m_ObjectList.size();

  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //number of elements to write (and later to read, on loading)
  output.write((char*) &i, sizeof(unsigned int));
  success = SaveToStream(&output);
  output.close();
  return success;
}

bool ObjectBase::SaveToStream(std::ofstream* Stream)
{
  std::map<std::string, std::string>::iterator iter;
  unsigned int len;

  for (iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
  {
    //write header "ObjS"
    Stream->write((char*) &cHeaderObjS, sizeof(unsigned int)); //Object, Static
    len = iter->first.length();
    Stream->write((char*) &len, sizeof(unsigned int));
    Stream->write(iter->first.c_str(), len);
    len = iter->second.length();
    Stream->write((char*) &len, sizeof(unsigned int));
    Stream->write(iter->second.c_str(), len);
    if (!Stream->good())
    {
      std::cout << "ObjectBase::SaveToStream: Error while writing data to stream.\n";
      return false;
    }
  }//for
  return Stream->good();
}

bool ObjectBase::LoadFromFile(const std::string FileName)
{
  std::ifstream input;
  unsigned int count, i;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "ObjectBase::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  unsigned int Header;
  Header = 0;

  //read header "Dusk"
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDusk)
  {
    std::cout << "ObjectBase::LoadFromFile: ERROR: File contains invalid "
              << "file header.\n";
    input.close();
    return false;
  }
  count = 0;
  //read how many objects we have in file
  input.read((char*) &count, sizeof(unsigned int));
  for (i=0; i<count; i++)
  {
    if (!LoadFromStream(&input));
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR: while reading data.\n";
      input.close();
      return false;
    }//if
  }//for
  input.close();
  return true;
}

bool ObjectBase::LoadFromStream(std::ifstream* Stream)
{
  unsigned int len;
  unsigned int Header = 0;
  static char ID_Buffer[256], Mesh_Buffer[256];

  //read header "ObjS" (Object, Static)
  Stream->read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderObjS)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  Stream->read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  Stream->read(ID_Buffer, len);
  ID_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream->good()))
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR while reading data.\n";
    return false;
  }
  //read length of mesh name
  Stream->read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: Name of Mesh cannot be "
              << "longer than 255 characters.\n";
    return false;
  }
  //read ID
  Stream->read(Mesh_Buffer, len);
  Mesh_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream->good()))
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR while reading data.\n";
    return false;
  }
  //now add it to the data
  addObject(std::string(ID_Buffer), std::string(Mesh_Buffer));
  return true;
}

}//namespace
