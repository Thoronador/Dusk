#include "ObjectBase.h"
#include "DuskTypes.h"
#include <fstream>
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
  output.write("Dusk", 4);
  //number of elements to write (and later to read, on loading)
  output.write((char*) &i, sizeof(unsigned int));

  std::map<std::string, std::string>::iterator iter;

  for (iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
  {
    output.write("ObjSta", 6); //Object, Static
    i = iter->first.length();
    output.write((char*) &i, sizeof(unsigned int));
    output.write(iter->first.c_str(), i);
    i = iter->second.length();
    output.write((char*) &i, sizeof(unsigned int));
    output.write(iter->second.c_str(), i);
    if (!output.good())
    {
      std::cout << "ObjectBase::SaveToFile: Error while writing data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//for
  output.close();
  return output.good();
}

bool ObjectBase::LoadFromFile(const std::string FileName)
{
  std::ifstream input;
  unsigned int count, i, len;
  char ID_Buffer[256], Mesh_Buffer[256];

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "ObjectBase::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  char Header[6];
  Header[0] = Header[1] = Header[2] = Header[3] = Header[4] = Header[5] = '\0';

  //read header "Dusk"
  input.read(Header, 4);
  if ((Header[0]!='D') || (Header[1]!='u') || (Header[2]!='s') || (Header[3]!='k'))
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
    //read header "ObjSta" (Object, Static)
    input.read(Header, 6);
    if ((Header[0]!='O') || (Header[1]!='b') || (Header[2]!='j') || (Header[3]!='S')
        || (Header[4]!='t') || (Header[5]!='a'))
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR: File contains invalid "
                << "record header.\n";
      input.close();
      return false;
    }//if
    //read length of ID
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR: ID cannot be longer than "
                << "255 characters.\n";
      input.close();
      return false;
    }
    //read ID
    input.read(ID_Buffer, len);
    ID_Buffer[len] = '\0'; //add terminating null character
    if (!input.good())
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR while reading data.\n";
      input.close();
      return false;
    }
    //read length of mesh name
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR: Name of Mesh cannot be "
                << "longer than 255 characters.\n";
      input.close();
      return false;
    }
    //read ID
    input.read(Mesh_Buffer, len);
    Mesh_Buffer[len] = '\0'; //add terminating null character
    if (!input.good())
    {
      std::cout << "ObjectBase::LoadFromFile: ERROR while reading data.\n";
      input.close();
      return false;
    }
    //now add it to the data
    addObject(std::string(ID_Buffer), std::string(Mesh_Buffer));
  }//for
  input.close();
  return true;
}

}//namespace
