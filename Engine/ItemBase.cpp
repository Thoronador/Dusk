#include "ItemBase.h"
#include <iostream>
#include <fstream>

namespace Dusk
{

ItemBase::ItemBase()
{
  m_ItemList = std::map<std::string,ItemRecord>();
}

ItemBase::~ItemBase()
{
  m_ItemList.clear();
}

ItemBase& ItemBase::GetSingleton()
{
  static ItemBase Instance;
  return Instance;
}

bool ItemBase::hasItem(const std::string ID_of_item) const
{
  return (m_ItemList.find(ID_of_item) != m_ItemList.end());
}

void ItemBase::addItem(const std::string ID, const std::string name,
                       const int value, const float weight,
                       const std::string Mesh)
{
  if (ID=="" or name=="" or Mesh=="")
  {
    std::cout << "ItemBase::addItem: ERROR: ID, name or mesh name is empty.\n";
    return;
  }
  if (value<0 or weight<0.0f)
  {
    std::cout << "ItemBase::addItem: ERROR: invalid weight or value given.\n";
    return;
  }
  m_ItemList[ID] = (ItemRecord){name, value, weight, Mesh};
}

void ItemBase::ClearAllItems()
{
  m_ItemList.clear();
}

unsigned int ItemBase::NumberOfItems() const
{
  return m_ItemList.size();
}

std::string ItemBase::GetItemName(const std::string itemID)
{
  std::map<std::string, ItemRecord>::iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.Name;
  }
  return "";
}

int ItemBase::GetItemValue(const std::string itemID)
{
  std::map<std::string, ItemRecord>::iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.value;
  }
  return -1;
}

float ItemBase::GetItemWeight(const std::string itemID)
{
  std::map<std::string, ItemRecord>::iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.weight;
  }
  return 0.0;
}

std::string ItemBase::GetMeshName(const std::string itemID)
{
  std::map<std::string, ItemRecord>::iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.Mesh;
  }
  return "";
}

bool ItemBase::SaveToFile(const std::string FileName)
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "ItemData::SaveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  unsigned int len;
  std::map<std::string, ItemRecord>::iterator iter;

  len = m_ItemList.size();
  //write header "Dusk"
  output.write("Dusk", 4);
  //number of elements to write (and later to read, on loading)
  output.write((char*) &len, sizeof(unsigned int));

  for(iter=m_ItemList.begin(); iter!=m_ItemList.end(); iter++)
  {
    //write header "Item"
    output.write("Item", 4);
    //write ID
    len = iter->first.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->first.c_str(), len);
    //write name
    len = iter->second.Name.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Name.c_str(), len);
    //write value
    output.write((char*) &(iter->second.value), sizeof(int));
    //write weight
    output.write((char*) &(iter->second.weight), sizeof(float));
    //write Mesh name
    len = iter->second.Mesh.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.Mesh.c_str(), len);

    if (!output.good())
    {
      std::cout << "ItemBase::SaveToFile: ERROR while writing data to file \""
                << FileName << "\".\n";
      output.close();
      return false;
    }//if
  }//for
  output.close();
  return true;
}

bool ItemBase::LoadFromFile(const std::string FileName)
{
  std::ifstream input;
  unsigned int count, i, len;
  char ID_Buffer[256], Name_Buffer[256], Mesh_Buffer[256];
  int value;
  float weight;

  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "ItemBase::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  char Header[4];
  Header[0] = Header[1] = Header[2] = Header[3] = '\0';

  //read header "Dusk"
  input.read(Header, 4);
  if ((Header[0]!='D') || (Header[1]!='u') || (Header[2]!='s') || (Header[3]!='k'))
  {
    std::cout << "ItemBase::LoadFromFile: ERROR: File contains invalid "
              << "file header.\n";
    input.close();
    return false;
  }
  count = 0;
  //How many items do we have in file?
  input.read((char*) &count, sizeof(unsigned int));

  for (i=0; i<count; i++)
  {
    //read header "Item"
    input.read(Header, 4);
    if ((Header[0]!='I') || (Header[1]!='t') || (Header[2]!='e') || (Header[3]!='m'))
    {
      std::cout << "ItemBase::LoadFromFile: ERROR: File contains invalid "
                << "item record header.\n";
      input.close();
      return false;
    }//if

    //read length of ID
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ItemBase::LoadFromFile: ERROR: ID cannot be longer than "
                << "255 characters.\n";
      input.close();
      return false;
    }
    //read ID
    input.read(ID_Buffer, len);
    ID_Buffer[len] = '\0'; //add terminating null character
    if (!input.good())
    {
      std::cout << "ItemBase::LoadFromFile: ERROR while reading data (ID).\n";
      input.close();
      return false;
    }

    //read length of item name
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ItemBase::LoadFromFile: ERROR: item name cannot be longer "
                << "than 255 characters.\n";
      input.close();
      return false;
    }
    //read item name
    input.read(Name_Buffer, len);
    Name_Buffer[len] = '\0'; //add terminating null character
    if (!input.good())
    {
      std::cout << "ItemBase::LoadFromFile: ERROR while reading data (name).\n";
      input.close();
      return false;
    }

    //read value
    input.read((char*) &value, sizeof(int));
    //read weight
    input.read((char*) &weight, sizeof(float));

    //read length of mesh name
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "ItemBase::LoadFromFile: ERROR: mesh name cannot be longer "
                << "than 255 characters.\n";
      input.close();
      return false;
    }
    //read mesh name
    input.read(Mesh_Buffer, len);
    Mesh_Buffer[len] = '\0'; //add terminating null character
    if (!input.good())
    {
      std::cout << "ItemBase::LoadFromFile: ERROR while reading data (mesh).\n";
      input.close();
      return false;
    }
    addItem(std::string(ID_Buffer), std::string(Name_Buffer), value, weight,
            std::string(Mesh_Buffer));
  }//for
  input.close();
  return true;
}

}//namespace
