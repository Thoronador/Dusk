#include "NPCBase.h"
#include <iostream>
#include "DuskConstants.h"

namespace Dusk
{

NPCAttributes NPCAttributes::GetNullAttributes()
{
  NPCAttributes temp;
  temp.Str=temp.Agi=temp.Vit=temp.Int=temp.Will=temp.Cha=temp.Luck=0;
  return temp;
}

NPCBase::NPCBase()
{
  m_NPCList = std::map<std::string,NPCRecord>();
}

NPCBase::~NPCBase()
{
  m_NPCList.clear();
}

NPCBase& NPCBase::GetSingleton()
{
  static NPCBase Instance;
  return Instance;
}

void NPCBase::addNPC(const std::string& ID, const std::string& Name, const std::string& Mesh, const uint8 Level, const NPCAttributes& Attr)
{
  if (ID=="" or Name=="" or Mesh=="")
  {
    std::cout << "NPCBase::addNPC: ERROR: ID, name or mesh name is empty.\n";
    return;
  }
  std::map<std::string, NPCRecord>::iterator iter = m_NPCList.find(ID);
  if (iter!= m_NPCList.end())
  {
    iter->second.Name = Name;
    iter->second.Mesh = Mesh;
    iter->second.Level = Level;
    iter->second.Attributes = Attr;
    return;
  }
  m_NPCList[ID] = (NPCRecord){Name, Mesh, Level, Attr};
}

bool NPCBase::deleteNPC(const std::string& NPC_ID)
{
  std::map<std::string, NPCRecord>::iterator iter = m_NPCList.find(NPC_ID);
  if (iter==m_NPCList.end())
  {
    return false;
  }
  m_NPCList.erase(iter);
  return true;
}

bool NPCBase::hasNPC(const std::string& NPC_ID) const
{
  return (m_NPCList.find(NPC_ID) != m_NPCList.end());
}

void NPCBase::ClearAllNPCs()
{
  m_NPCList.clear();
}

unsigned int NPCBase::NumberOfNPCs() const
{
  return m_NPCList.size();
}

std::string NPCBase::getNPCName(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Name;
  }
  return "";
}

std::string NPCBase::getNPCMesh(const std::string& NPC_ID, const bool UseMarkerOnError) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Mesh;
  }
  //item with given ID is not present
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  return "";
}

uint8 NPCBase::getLevel(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Level;
  }
  return 0;
}

NPCAttributes NPCBase::getAttributes(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.Attributes;
  }
  return NPCAttributes::GetNullAttributes();
}

const Inventory& NPCBase::getNPCInventory(const std::string& NPC_ID) const
{
  std::map<std::string, NPCRecord>::const_iterator iter = m_NPCList.find(NPC_ID);
  if (iter!=m_NPCList.end())
  {
    return iter->second.InventoryAtStart;
  }
  return Inventory::GetEmptyInventory();
}

}//namespace
