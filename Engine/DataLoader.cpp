#include "DataLoader.h"
#include <fstream>
#include "AnimationData.h"
#include "ContainerBase.h"
#include "Dialogue.h"
#include "ItemBase.h"
#include "Journal.h"
#include "Landscape.h"
#include "LightBase.h"
#include "NPCBase.h"
#include "ObjectBase.h"
#include "ObjectData.h"
#include "QuestLog.h"
#include "ProjectileBase.h"
#include "WeaponBase.h"
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

bool DataLoader::SaveToFile(const std::string& FileName, const unsigned int bits) const
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


  if ((bits & ANIMATED_BIT) !=0)
  {
    data_records += AnimationData::GetSingleton().NumberOfReferences();
  }
  if ((bits & CONTAINER_BIT) !=0)
  {
    data_records += ContainerBase::GetSingleton().NumberOfContainers();
  }
  if ((bits & DIALOGUE_BIT) !=0)
  {
    data_records += Dialogue::GetSingleton().NumberOfLines();
  }
  if ((bits & ITEM_BIT) !=0)
  {
    data_records += ItemBase::GetSingleton().NumberOfItems();
  }
  if ((bits & JOURNAL_BIT) !=0)
  {
    data_records += Journal::GetSingleton().NumberOfDistinctQuests();
  }
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    data_records += Landscape::GetSingleton().RecordsAvailable();
  }
  if ((bits & LIGHT_BIT) !=0)
  {
    data_records += LightBase::GetSingleton().NumberOfLights();
  }
  if ((bits & NPC_BIT) !=0)
  {
    data_records += NPCBase::GetSingleton().NumberOfNPCs();
  }
  if ((bits & OBJECT_BIT) !=0)
  {
    data_records += ObjectBase::GetSingleton().NumberOfObjects();
  }
  if ((bits & PROJECTILE_BIT) !=0)
  {
    data_records += ProjectileBase::GetSingleton().GetNumberOfProjectiles();
  }
  if ((bits & QUEST_LOG_BIT) !=0)
  {
    data_records += 1;
  }
  if ((bits & REFERENCE_BIT) !=0)
  {
    data_records += ObjectData::GetSingleton().NumberOfReferences();
  }
  if ((bits & WEAPON_BIT) !=0)
  {
    data_records += WeaponBase::GetSingleton().NumberOfWeapons();
  }
  //write number of records
  output.write((char*) &data_records, sizeof(unsigned int));


  //save containers
  if ((bits & CONTAINER_BIT)!=0)
  {
    if (!ContainerBase::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Container "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//containers

  //save projectiles
  if ((bits & PROJECTILE_BIT)!=0)
  {
    if (!ProjectileBase::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Projectile "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//projectiles

  //save weapons
  if ((bits & WEAPON_BIT)!=0)
  {
    if (!WeaponBase::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Weapon "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//weapons

  //save dialogues
  if ((bits & DIALOGUE_BIT)!=0)
  {
    if (!Dialogue::GetSingleton().SaveToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Dialogue "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//dialogue

  //save items
  if ((bits & ITEM_BIT) !=0)
  {
    if (!ItemBase::GetSingleton().SaveToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write item data to"
                << " file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if items

  //save journal entries
  if ((bits & JOURNAL_BIT)!=0)
  {
    if (!Journal::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write basic "
                << "Journal data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//journal entries

  //save quest log
  if ((bits & QUEST_LOG_BIT)!=0)
  {
    if (!QuestLog::GetSingleton().SaveToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write QuestLog "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//quest log entries

  //save landscape
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    if (!(Landscape::GetSingleton().SaveAllToStream(output)))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write landscape "
                << "records to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if landscape

  //save lights
  if ((bits & LIGHT_BIT) !=0)
  {
    if(!LightBase::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Light data "
                << "to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }

  //save NPCs
  if ((bits & NPC_BIT) !=0)
  {
    if (!NPCBase::GetSingleton().SaveToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write NPC data to"
                << " file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if NPCs

  //save objects
  if ((bits & OBJECT_BIT) !=0)
  {
    if (!ObjectBase::GetSingleton().SaveToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write object data "
                << "to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if objects

  //save animated objects
  if ((bits & ANIMATED_BIT)!=0)
  {
    if (!AnimationData::GetSingleton().SaveAllToStream(output))
    {
      std::cout << "DataLoader::SaveToFile: ERROR: could not write Animation "
                << "reference data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//AnimatedObjects

  //save object references
  if ((bits & REFERENCE_BIT) !=0)
  {
    if (!ObjectData::GetSingleton().SaveAllToStream(output))
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

bool DataLoader::LoadFromFile(const std::string& FileName)
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
      case cHeaderCont:
           success = ContainerBase::GetSingleton().LoadNextContainerFromStream(input);
           break;
      case cHeaderDial:
           success = Dialogue::GetSingleton().LoadNextRecordFromStream(input);
           break;
      case cHeaderItem:
           success = ItemBase::GetSingleton().LoadFromStream(input);
           break;
      case cHeaderJour:
           success = Journal::GetSingleton().LoadNextFromStream(input);
           break;
      case cHeaderLand:
           land_rec = Landscape::GetSingleton().CreateRecord();
           success = land_rec->LoadFromStream(input);
           if (!success)
           {
             Landscape::GetSingleton().DestroyRecord(land_rec);
           }
           break;
      case cHeaderLight:
           success = LightBase::GetSingleton().LoadRecordFromStream(input);
           break;
      case cHeaderNPC_:
           success = NPCBase::GetSingleton().LoadNextRecordFromStream(input);
           break;
      case cHeaderObjS:
           success = ObjectBase::GetSingleton().LoadFromStream(input);
           break;
      case cHeaderQLog:
           success = QuestLog::GetSingleton().LoadFromStream(input);
           break;
      case cHeaderRefC: //Container
      case cHeaderRefI: //Item
      case cHeaderRefL: //Light
      case cHeaderRefO: //DuskObject
      case cHeaderRfWe: //Weapon
           success = ObjectData::GetSingleton().LoadNextFromStream(input, Header);
           break;
      case cHeaderRefA:  //AnimatedObject
      case cHeaderRefN:  //NPC
      case cHeaderRefP:  //Projectiles
      case cHeaderRfWP:  //WaypointObject
           success = AnimationData::GetSingleton().LoadNextFromStream(input, Header);
           break;
      case cHeaderWeap:
           success = WeaponBase::GetSingleton().LoadNextWeaponFromStream(input);
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
    records_done = records_done+1;
  }//while
  input.close();
  std::cout << "DataLoader::LoadFromFile: Info: "<<records_done<<" records "
            << "loaded from file \""<<FileName<<"\".\n";
  m_LoadedFiles.push_back(FileName);
  return true;
}

void DataLoader::ClearData(const unsigned int bits)
{
  if (bits==ALL_BITS)
  {
    m_LoadedFiles.clear();
  }
  if ((bits & REFERENCE_BIT) != 0)
  {
    ObjectData::GetSingleton().ClearData();
  }//object references

  if ((bits & OBJECT_BIT) != 0)
  {
    ObjectBase::GetSingleton().ClearAllObjects();
  }//Object information

  if ((bits & ANIMATED_BIT) != 0)
  {
    AnimationData::GetSingleton().ClearData();
  }//animated object and NPC references

  if ((bits & CONTAINER_BIT)!=0)
  {
    ContainerBase::GetSingleton().DeleteAllContainers();
  }//container data

  if ((bits & DIALOGUE_BIT)!=0)
  {
    Dialogue::GetSingleton().ClearData();
  }//dialogue data

  if ((bits & ITEM_BIT)!=0)
  {
    ItemBase::GetSingleton().ClearAllItems();
  }//item data

  if ((bits & JOURNAL_BIT) !=0)
  {
    Journal::GetSingleton().ClearAllEntries();
  }//journal

  if ((bits & LANDSCAPE_BIT) !=0)
  {
    Landscape::GetSingleton().ClearAllRecords();
  }//landscape

  if ((bits & LIGHT_BIT) !=0)
  {
    LightBase::GetSingleton().ClearAllData();
  }//lights

  if ((bits & NPC_BIT)!=0)
  {
    NPCBase::GetSingleton().ClearAllNPCs();
  }//NPC data

  if ((bits & PROJECTILE_BIT) != 0)
  {
    ProjectileBase::GetSingleton().ClearAll();
  }//projectile information

  if ((bits & QUEST_LOG_BIT) !=0)
  {
    QuestLog::GetSingleton().ClearAllData();
  }//quest log

  if ((bits & WEAPON_BIT) !=0)
  {
    WeaponBase::GetSingleton().ClearAll();
  }//weapons

}//clear data function

bool DataLoader::LoadSaveGame(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    std::cout << "DataLoader::LoadSaveGame: Could not open file \""<<FileName
              << "\" for reading in binary mode.\n";
    return false;
  }//if

  unsigned int Header, data_records;

  //determine file size
  input.seekg(0, std::ios::end);
  const unsigned int file_size = input.tellg();
  input.seekg(0, std::ios::beg);

  if (file_size<16)
  {
    std::cout << "DataLoader::LoadSaveGame: file \""<<FileName << "\" is to "
              << "small to contain a real save game.\n";
    return false;
  }

  //read header "Dusk"
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDusk)
  {
    std::cout << "DataLoader::LoadSaveGame: ERROR: File \""<<FileName
              <<"\" contains invalid file header.\n";
    input.close();
    return false;
  }
  //determine number of records
  data_records = 0;
  input.read((char*) &data_records, sizeof(unsigned int));
  //read header "Save"
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderSave)
  {
    std::cout << "DataLoader::LoadSaveGame: ERROR: File \""<<FileName
              <<"\" does not seem to be a valid save game.\n";
    input.close();
    return false;
  }
  /*read header "Mean" (identifies save game "version", because this should
    be improved and get a different version later on) */
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderMean)
  {
    std::cout << "DataLoader::LoadSaveGame: ERROR: File \""<<FileName
              <<"\" does not contain a valid save game format.\n";
    input.close();
    return false;
  }
  //read dependencies
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDeps)
  {
    std::cout << "DataLoader::LoadSaveGame: ERROR: File \""<<FileName
              <<"\" does not contain a list of required data files.\n";
    input.close();
    return false;
  }
  //read their number
  unsigned int depCount = 0;
  input.read((char*) &depCount, sizeof(unsigned int));
  if (depCount == 0 or depCount>255)
  { //no reasonable limits given
    std::cout << "DataLoader::LoadSaveGame: ERROR: File \""<<FileName
              << "\" has a list of required data files which is to long or to"
              << " short (length: "<<depCount<<").\n";
    input.close();
    return false;
  }
  ClearData(ALL_BITS);
  m_LoadedFiles.clear();
  unsigned int len;
  char buffer[256];
  for (Header=0; Header<depCount; Header=Header+1)
  {
    len = 0;
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "DataLoader::LoadSaveGame: ERROR while loading file \""
                <<FileName<< "\": name of required data file is longer than 255"
                <<" characters.\n";
      input.close();
      return false;
    }//if
    buffer[0]='\0';
    input.read(buffer, len);
    buffer[len] = '\0';
    if (!input.good())
    {
      std::cout << "DataLoader::LoadSaveGame: ERROR while reading name of "
                << "required data file.\n";
      input.close();
      return false;
    }
    //got name, so load it
    const std::string dataFileName = std::string(buffer);
    if (dataFileName==FileName)
    {
      std::cout << "DataLoader::LoadSaveGame: ERROR: SaveGame file cannot be a"
                << "required data file of itself.\n";
      input.close();
      return false;
    }
    if (!LoadFromFile(dataFileName))
    {
      std::cout << "DataLoader::LoadSaveGame: ERROR while loading required "
                << "data file \""<<dataFileName<<"\" of save \""<<FileName
                <<"\".\n";
      input.close();
      return false;
    }
    else
    {
      std::cout << "DataLoader::LoadSaveGame: Info: data file \""<<dataFileName
                <<"\" of save \""<<FileName<<"\" successfully loaded.\n";
      m_LoadedFiles.push_back(dataFileName);
    }
  }//for

  //dependencies are loaded, now clear unneeded data
  ClearData(SAVE_MEAN_BITS);
  //go on loading
  bool success = true;
  unsigned int records_done = 0;
  while ((records_done<data_records) && (input.tellg()<file_size))
  {
    Header = 0;
    //read next record header
    input.read((char*) &Header, sizeof(unsigned int));
    input.seekg(-4, std::ios::cur);
    switch(Header)
    {
      case cHeaderRefA:  //AnimatedObject
      case cHeaderRefN:  //NPC
      case cHeaderRefP:  //Projectiles
      case cHeaderRfWP:  //WaypointObject
           success = AnimationData::GetSingleton().LoadNextFromStream(input, Header);
           break;
      case cHeaderRefC: //Container
      case cHeaderRefI: //Item
      case cHeaderRefL: //Light
      case cHeaderRefO: //DuskObject
      case cHeaderRfWe: //Weapon
           success = ObjectData::GetSingleton().LoadNextFromStream(input, Header);
           break;
      case cHeaderQLog: //questlog
           success = QuestLog::GetSingleton().LoadFromStream(input);
           break;
      default:
           std::cout <<"DataLoader::LoadSaveGame: ERROR: Got unexpected header "
                     <<Header << " in file \""<<FileName<<"\" at position "
                     <<input.tellg()<<".\n";
           success = false;
           break;
    }//swi
    if(!success or !input.good())
    {
      std::cout << "DataLoader::LoadSaveGame: ERROR while reading data.\n"
                << "Position: "<<input.tellg() << " bytes.\n"
                << "Records read: "<<records_done<<" (excluding failure)\n";
      input.close();
      return false;
    }
    records_done = records_done+1;
  }//while
  input.close();
  std::cout << "DataLoader::LoadSaveGame: Info: "<<records_done<<" out of "
            << data_records<< " expected records loaded.\n";
  return true;
}

bool DataLoader::SaveGame(const std::string& FileName) const
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    std::cout << "DataLoader::SaveGame: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if
  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //determine and write number of records
  unsigned int data_records = 1 /*QuestLog*/ + ObjectData::GetSingleton().NumberOfReferences()
                              + AnimationData::GetSingleton().NumberOfReferences();
  output.write((char*) &data_records, sizeof(unsigned int));
  //write headers to identify file as save game
  output.write((char*) &cHeaderSave, sizeof(unsigned int));
  output.write((char*) &cHeaderMean, sizeof(unsigned int));
  //dependencies
  output.write((char*) &cHeaderDeps, sizeof(unsigned int));
  data_records = m_LoadedFiles.size();
  output.write((char*) &data_records, sizeof(unsigned int));
  unsigned int i;
  for (i=0; i<m_LoadedFiles.size(); i=i+1)
  {
    data_records = m_LoadedFiles[i].length();
    output.write((char*) &data_records, sizeof(unsigned int));
    output.write(m_LoadedFiles[i].c_str(), data_records);
  }//for
  if(!output.good())
  {
    std::cout << "DataLoader::SaveGame: ERROR while writing header data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  //write the data
  if (!ObjectData::GetSingleton().SaveAllToStream(output))
  {
    std::cout << "DataLoader::SaveGame: ERROR while writing object data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!AnimationData::GetSingleton().SaveAllToStream(output))
  {
    std::cout << "DataLoader::SaveGame: ERROR while writing animation data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!QuestLog::GetSingleton().SaveToStream(output))
  {
    std::cout << "DataLoader::SaveGame: ERROR while writing quest log to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  output.close();
  return true;
}

}//namespace
