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

void NPCBase::addNPC(const std::string& ID, const std::string& Name,
                     const std::string& Mesh, const uint8 Level,
                     const NPCAttributes& Attr, const Inventory& StartingInventory)
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
    iter->second.InventoryAtStart.MakeEmpty();
    StartingInventory.AddAllItemsTo(iter->second.InventoryAtStart);
    return;
  }
  NPCRecord temp;
  temp.Name = Name;
  temp.Mesh = Mesh;
  temp.Level = Level;
  temp.Attributes = Attr;
  temp.InventoryAtStart = StartingInventory;
  m_NPCList[ID] = temp;
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

bool NPCBase::SaveToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "NPCBase::SaveToStream: ERROR: Bad stream.\n";
    return false;
  }
  unsigned int len;
  std::map<std::string,NPCRecord>::const_iterator iter = m_NPCList.begin();
  while (iter != m_NPCList.end())
  {
    //header
    output.write((char*) &cHeaderNPC_, sizeof(unsigned int));
    //ID
    len = iter->first.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->first.c_str(), len);
    //NPC data
    // -- name
    len = iter->second.Name.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Name.c_str(), len);
    // -- mesh
    len = iter->second.Mesh.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Mesh.c_str(), len);
    // -- level
    output.write((char*) &(iter->second.Level), 1);
    // -- attributes
    output.write((char*) &(iter->second.Attributes.Str), 1);
    output.write((char*) &(iter->second.Attributes.Agi), 1);
    output.write((char*) &(iter->second.Attributes.Vit), 1);
    output.write((char*) &(iter->second.Attributes.Int), 1);
    output.write((char*) &(iter->second.Attributes.Will), 1);
    output.write((char*) &(iter->second.Attributes.Cha), 1);
    output.write((char*) &(iter->second.Attributes.Luck), 1);
    //inventory
    if (!(iter->second.InventoryAtStart.SaveToStream(output)))
    {
      std::cout << "NPCBase::SaveToStream: ERROR while writing inventory data "
                << "of NPC \""<<iter->first<<"\" to stream.\n";
      return false;
    }
    iter++;
  } //while
  return output.good();
}

bool NPCBase::LoadNextRecordFromStream(std::ifstream& input)
{
  //not completely implemented yet
  if (!(input.good()))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR: Bad stream.\n";
    return false;
  }
  unsigned int len = 0;
  //header
  input.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderNPC_)
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR: Invalid header.\n";
    return false;
  }
  //ID
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR: NPC ID seems to be"
              << " longer than 255 characters.\n";
    return false;
  }
  char buffer[256];
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR while reading NPC "
              << "ID.\n";
    return false;
  }
  std::string tempID = std::string(buffer);
  //name
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR: NPC name seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR while reading NPC "
              << "name from stream.\n";
    return false;
  }
  NPCRecord temp_rec;
  temp_rec.Name = std::string(buffer);
  //mesh
  len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len > 255)
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR: NPC mesh seems to "
              << "be longer than 255 characters.\n";
    return false;
  }
  buffer[0] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR while reading NPC "
              << "mesh location from stream.\n";
    return false;
  }
  temp_rec.Mesh = std::string(buffer);
  //level
  input.read((char*) &(temp_rec.Level), 1);
  //attributes
  input.read((char*) &(temp_rec.Attributes.Str), 1);
  input.read((char*) &(temp_rec.Attributes.Agi), 1);
  input.read((char*) &(temp_rec.Attributes.Vit), 1);
  input.read((char*) &(temp_rec.Attributes.Int), 1);
  input.read((char*) &(temp_rec.Attributes.Will), 1);
  input.read((char*) &(temp_rec.Attributes.Cha), 1);
  input.read((char*) &(temp_rec.Attributes.Luck), 1);
  if (!(input.good()))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR while reading NPC "
              << "level or attributes from stream.\n";
    return false;
  }
  //inventory
  if (!(temp_rec.InventoryAtStart.LoadFromStream(input)))
  {
    std::cout << "NPCBase::LoadNextRecordFromStream: ERROR while reading NPC "
              << "inventory from stream.\n";
    return false;
  }
  addNPC(tempID, temp_rec.Name, temp_rec.Mesh, temp_rec.Level,
         temp_rec.Attributes, temp_rec.InventoryAtStart);
  return input.good();
}

}//namespace
