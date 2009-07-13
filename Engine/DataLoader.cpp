#include "DataLoader.h"
#include <fstream>
#include "Landscape.h"
#include "ItemBase.h"
#include "ObjectBase.h"
#include "ObjectData.h"

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
  output.write("Dusk", 4);
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

}//namespace
