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

}//namespace
