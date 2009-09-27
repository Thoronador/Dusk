#include "ContainerBase.h"
#include "DuskConstants.h"
#include <iostream>

namespace Dusk{

ContainerBase::ContainerBase()
{ //constructor
  m_ContainerList.clear();
}

ContainerBase::~ContainerBase()
{
  //destructor
  DeleteAllContainers();
}

ContainerBase& ContainerBase::GetSingleton()
{
  static ContainerBase Instance;
  return Instance;
}

void ContainerBase::AddContainer(const std::string& ID, const std::string& _mesh, const Inventory& contents)
{
  if (ID=="" or _mesh=="")
  {
    std::cout << "ContainerBase::AddContainer: ERROR: ID or Mesh is empty string!\n";
    return;
  }
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter==m_ContainerList.end())
  {
    //not present, so create it
    m_ContainerList[ID].Mesh = _mesh;
    m_ContainerList[ID].ContainerInventory = Inventory();
    contents.AddAllTo(m_ContainerList[ID].ContainerInventory);
    return;
  }//if
  iter->second.Mesh = _mesh;
  iter->second.ContainerInventory.MakeEmpty();
  contents.AddAllTo(iter->second.ContainerInventory);
  return;
}

bool ContainerBase::DeleteContainer(const std::string& ID)
{
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter != m_ContainerList.end())
  {
    iter->second.ContainerInventory.MakeEmpty();
    m_ContainerList.erase(iter);
    return true;
  }
  return false;
}

bool ContainerBase::HasContainer(const std::string& ID) const
{
  return (m_ContainerList.find(ID)!=m_ContainerList.end());
}

std::string ContainerBase::GetContainerMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  std::map<std::string, ContainerRecord>::const_iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter!=m_ContainerList.end())
  {
    return iter->second.Mesh;
  }
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  return "";
}

const Inventory& ContainerBase::GetContainerInventory(const std::string& ID) const
{
  std::map<std::string, ContainerRecord>::const_iterator iter;
  iter = m_ContainerList.find(ID);
  if (iter!=m_ContainerList.end())
  {
    return iter->second.ContainerInventory;
  }
  return Inventory::GetEmptyInventory();
}

void ContainerBase::DeleteAllContainers()
{
  std::map<std::string, ContainerRecord>::iterator iter;
  iter = m_ContainerList.begin();
  while (iter != m_ContainerList.end())
  {
    iter->second.ContainerInventory.MakeEmpty();
  }//while
  m_ContainerList.clear();
}

unsigned int ContainerBase::NumberOfContainers() const
{
  return m_ContainerList.size();
}

bool ContainerBase::SaveAllToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "ContainerBase::SaveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  std::map<std::string, ContainerRecord>::const_iterator traverse;
  traverse = m_ContainerList.begin();
  while (traverse != m_ContainerList.end())
  {
    //header "Cont"
    OutStream.write((char*) &cHeaderCont, sizeof(unsigned int));
    //ID
    len = traverse->first.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(traverse->first.c_str(), len);
    //Mesh
    len = traverse->second.Mesh.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(traverse->second.Mesh.c_str(), len);
    //Inventory
    if (!(traverse->second.ContainerInventory.SaveToStream(OutStream)))
    {
      std::cout << "ContainerBase::SaveAllToStream: ERROR while writing "
                << "container's inventory.\n";
      return false;
    }//if
    traverse++;
  }//while
  return OutStream.good();
}

bool ContainerBase::LoadNextContainerFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR: stream "
              << "contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderCont)
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR: stream "
              << "contains unexpected header!\n";
    return false;
  }
  char ID_Buffer[256];
  ID_Buffer[0] = ID_Buffer[255] = '\0';
  //read ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR: ID is "
              << "longer than 255 characters!\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  if (!InStream.good())
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR while "
              << "reading ID from stream!\n";
    return false;
  }
  //read Mesh
  char Mesh_Buffer[256];
  Mesh_Buffer[0] = Mesh_Buffer[255] = '\0';
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR: mesh path "
              << "is longer than 255 characters!\n";
    return false;
  }
  InStream.read(Mesh_Buffer, len);
  if (!InStream.good())
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR while "
              << "reading mesh path from stream!\n";
    return false;
  }
  Inventory temp;
  if (!temp.LoadFromStream(InStream))
  {
    std::cout << "ContainerBase::LoadNextContainerFromStream: ERROR while "
              << "reading inventory contens from stream!\n";
    return false;
  }
  //all right so far, add new container
  AddContainer(std::string(ID_Buffer), std::string(Mesh_Buffer), temp);
  return InStream.good();
}

}//namespace
