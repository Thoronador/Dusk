#include "DataLoader.h"
#include <fstream>
#include "Landscape.h"
#include "ItemBase.h"
#include "ObjectBase.h"
#include "ObjectData.h"
#include "DuskConstants.h"

namespace Dusk
{

DataLoader::DataLoader()
{
  //empty
}

DataLoader::~DataLoader()
{
  //empty
}

DataLoader& DataLoader::GetSingleton()
{
  static DataLoader Instance;
  return Instance;
}

bool DataLoader::SaveToFile(const std::string FileName, const unsigned int bits)
{
  std::ofstream output;
  unsigned int data_records;

  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "DataLoader::SaveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //determine number of records
  data_records = 0;
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    data_records += Landscape::GetSingleton().RecordsAvailable();
  }
  if ((bits & ITEM_BIT) !=0)
  {
    data_records += ItemBase::GetSingleton().NumberOfItems();
  }
  if ((bits & OBJECT_BIT) !=0)
  {
    data_records += ObjectBase::GetSingleton().NumberOfObjects();
  }
  if ((bits & OBJECT_REF_BIT) !=0)
  {
    data_records += ObjectData::GetSingleton().NumberOfReferences();
  }
  //write number of records
  output.write((char*) &data_records, sizeof(unsigned int));

  //save landscape
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    LandscapeRecord * land;
    data_records = Landscape::GetSingleton().RecordsAvailable();
    unsigned int i;
    for (i=0; i<data_records; i++)
    {
      land = Landscape::GetSingleton().GetRecordByPosition(i);
      if (!(land->SaveToStream(&output)))
      {
        std::cout << "DataLoader::SaveToFile: ERROR: could not write landscape "
                  << "record "<<i<<" to file \""<<FileName<<"\".\n";
        output.close();
        return false;
      }//if
    }//for
  }//if landscape

  //save items
  if ((bits & ITEM_BIT) !=0)
  {
    if (!ItemBase::GetSingleton().SaveToStream(&output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write item data to"
                << " file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if items

  //save objects
  if ((bits & OBJECT_BIT) !=0)
  {
    if (!ObjectBase::GetSingleton().SaveToStream(&output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write object data "
                << "to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if objects

  //save object references
  if ((bits & OBJECT_REF_BIT) !=0)
  {
    if (!ObjectData::GetSingleton().SaveToStream(&output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write object "
                << "reference data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if obj. references
  output.close();
  return true;
}

bool DataLoader::LoadFromFile(const std::string FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "DataLoader::LoadFromFile: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  unsigned int file_size, Header, data_records, records_done;

  //determine file size
  input.seekg(0, std::ios::end);
  file_size = input.tellg();
  input.seekg(0, std::ios::beg);

  //read header "Dusk"
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDusk)
  {
    std::cout << "DataLoader::LoadFromFile: ERROR: File \""<<FileName
              <<"\" contains invalid file header.\n";
    input.close();
    return false;
  }

  //determine number of records
  data_records = 0;
  input.read((char*) &data_records, sizeof(unsigned int));

  //read loop
  LandscapeRecord* land_rec = NULL;
  bool success = true;
  records_done = 0;
  while ((records_done<data_records) && (input.tellg()<file_size))
  {
    Header = 0;
    //read next record header
    input.read((char*) &Header, sizeof(unsigned int));
    input.seekg(-4, std::ios::cur);
    switch (Header)
    {
      case cHeaderItem:
           success = ItemBase::GetSingleton().LoadFromStream(&input);
           break;
      case cHeaderLand:
           land_rec = Landscape::GetSingleton().CreateRecord();
           success = land_rec->LoadFromStream(&input);
           if (!success)
           {
             Landscape::GetSingleton().DestroyRecord(land_rec);
           }
           break;
      case cHeaderObjS:
           success = ObjectBase::GetSingleton().LoadFromStream(&input);
           break;
      case cHeaderRefO:
           success = ObjectData::GetSingleton().LoadFromStream(&input);
           break;
      default:
          std::cout << "DataLoader::LoadFromFile: ERROR: Got unexpected header "
                    <<Header << " in file \""<<FileName<<"\" at position "
                    <<input.tellg()<<".\n";
          success = false;
          break;
    }//switch

    if(!success or !input.good())
    {
      std::cout << "DataLoader::LoadFromFile: ERROR while reading data.\n"
                << "Position: "<<input.tellg() << " bytes.\n"
                << "Records read: "<<records_done<<" (excluding failure)\n";
      input.close();
      return false;
    }
    records_done++;
  }//while
  input.close();
  std::cout << "DataLoader::LoadFromFile: Info: "<<records_done<<" records loaded.\n";
  return true;
}

void DataLoader::ClearData(const unsigned int bits)
{
  if ((bits & OBJECT_REF_BIT) != 0)
  {
    ObjectData::GetSingleton().ClearData();
  }//Object references

  if ((bits & OBJECT_BIT) != 0)
  {
    ObjectBase::GetSingleton().ClearAllObjects();
    if (ObjectData::GetSingleton().NumberOfReferences()>0)
    { //kill object data, it's useless with an empty object base
      ObjectData::GetSingleton().ClearData();
    }
  }//Object information

  if ((bits & ITEM_BIT)!=0)
  {
    ItemBase::GetSingleton().ClearAllItems();
  }//item data

  if ((bits & LANDSCAPE_BIT) !=0)
  {
    Landscape::GetSingleton().ClearAllRecords();
  }//landscape

}//clear data function



}//namespace
