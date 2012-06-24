/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "DataLoader.h"
#include <fstream>
#include "InjectionManager.h"
#include "ContainerBase.h"
#include "Dialogue.h"
#include "ItemBase.h"
#include "Journal.h"
#include "Landscape.h"
#include "LightBase.h"
#include "NPCBase.h"
#include "ObjectBase.h"
#include "ObjectManager.h"
#include "Player.h"
#include "QuestLog.h"
#include "ProjectileBase.h"
#include "ResourceBase.h"
#include "SoundBase.h"
#include "VehicleBase.h"
#include "WeaponBase.h"
#include "DuskConstants.h"
#include "Messages.h"

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

DataLoader& DataLoader::getSingleton()
{
  static DataLoader Instance;
  return Instance;
}

bool DataLoader::saveToFile(const std::string& FileName, const unsigned int bits) const
{
  std::ofstream output;
  unsigned int data_records;

  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    DuskLog() << "DataLoader::saveToFile: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if

  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //determine number of records
  data_records = 0;

  if ((bits & CONTAINER_BIT) !=0)
  {
    data_records += ContainerBase::getSingleton().numberOfContainers();
  }
  if ((bits & DIALOGUE_BIT) !=0)
  {
    data_records += Dialogue::getSingleton().numberOfLines();
  }
  if ((bits & INJECTION_BIT) !=0)
  {
    //animated objects
    data_records += InjectionManager::getSingleton().getNumberOfReferences();
    //player object
    data_records += 1;
  }
  if ((bits & ITEM_BIT) !=0)
  {
    data_records += ItemBase::getSingleton().getNumberOfItems();
  }
  if ((bits & JOURNAL_BIT) !=0)
  {
    data_records += Journal::getSingleton().numberOfDistinctQuests();
  }
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    data_records += Landscape::getSingleton().getNumberOfRecordsAvailable();
  }
  if ((bits & LIGHT_BIT) !=0)
  {
    data_records += LightBase::getSingleton().getNumberOfLights();
  }
  if ((bits & NPC_BIT) !=0)
  {
    data_records += NPCBase::getSingleton().getNumberOfNPCs();
  }
  if ((bits & OBJECT_BIT) !=0)
  {
    data_records += ObjectBase::getSingleton().getNumberOfObjects();
  }
  if ((bits & PROJECTILE_BIT) !=0)
  {
    data_records += ProjectileBase::getSingleton().getNumberOfProjectiles();
  }
  if ((bits & QUEST_LOG_BIT) !=0)
  {
    data_records += 1;
  }
  if ((bits & REFERENCE_BIT) !=0)
  {
    //static objects
    data_records += ObjectManager::getSingleton().getNumberOfReferences();
  }
  if ((bits & RESOURCE_BIT) !=0)
  {
    data_records += ResourceBase::getSingleton().getNumberOfResources();
  }
  if ((bits & SOUND_BIT) !=0)
  {
    data_records += SoundBase::getSingleton().getNumberOfSounds();
  }
  if ((bits & VEHICLE_BIT) !=0)
  {
    data_records += VehicleBase::getSingleton().getVehicleNumber();
  }
  if ((bits & WEAPON_BIT) !=0)
  {
    data_records += WeaponBase::getSingleton().getNumberOfWeapons();
  }
  //write number of records
  output.write((char*) &data_records, sizeof(unsigned int));


  //save containers
  if ((bits & CONTAINER_BIT)!=0)
  {
    if (!ContainerBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Container "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//containers

  //save projectiles
  if ((bits & PROJECTILE_BIT)!=0)
  {
    if (!ProjectileBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Projectile "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//projectiles

  //save vehicles
  if ((bits & VEHICLE_BIT)!=0)
  {
    if (!VehicleBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Vehicle "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//vehicles

  //save weapons
  if ((bits & WEAPON_BIT)!=0)
  {
    if (!WeaponBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Weapon "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//weapons

  //save dialogues
  if ((bits & DIALOGUE_BIT)!=0)
  {
    if (!Dialogue::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Dialogue "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//dialogue

  //save items
  if ((bits & ITEM_BIT) !=0)
  {
    if (!ItemBase::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write item data to"
                << " file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if items

  //save journal entries
  if ((bits & JOURNAL_BIT)!=0)
  {
    if (!Journal::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write basic "
                << "Journal data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//journal entries

  //save quest log
  if ((bits & QUEST_LOG_BIT)!=0)
  {
    if (!QuestLog::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write QuestLog "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//quest log entries

  //save landscape
  if ((bits & LANDSCAPE_BIT) !=0)
  {
    if (!(Landscape::getSingleton().saveAllToStream(output)))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write landscape "
                << "records to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if landscape

  //save lights
  if ((bits & LIGHT_BIT) !=0)
  {
    if(!LightBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Light data "
                << "to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }

  //save NPCs
  if ((bits & NPC_BIT) !=0)
  {
    if (!NPCBase::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write NPC data to"
                << " file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if NPCs

  //save objects
  if ((bits & OBJECT_BIT) !=0)
  {
    if (!ObjectBase::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write object data "
                << "to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if objects

  //save resources
  if ((bits & RESOURCE_BIT)!=0)
  {
    if (!ResourceBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write resource "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//resources

  //save sound data
  if ((bits & SOUND_BIT) !=0)
  {
    if (!SoundBase::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write sound "
                << "data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if SoundBase

  //save animated objects/ injection objects
  if ((bits & INJECTION_BIT)!=0)
  {
    //animated objects
    if (!InjectionManager::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Injection "
                << "reference data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
    //save player object, too
    if (!Player::getSingleton().saveToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write Player "
                << "reference data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }
  }//AnimatedObjects

  //save object references
  if ((bits & REFERENCE_BIT) !=0)
  {
    if (!ObjectManager::getSingleton().saveAllToStream(output))
    {
      DuskLog() << "DataLoader::saveToFile: ERROR: could not write object "
                << "reference data to file \""<<FileName<<"\".\n";
      output.close();
      return false;
    }//if
  }//if obj. references
  output.close();
  return true;
}

bool DataLoader::loadFromFile(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    DuskLog() << "DataLoader::loadFromFile: Could not open file \""<<FileName
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
    DuskLog() << "DataLoader::loadFromFile: ERROR: File \""<<FileName
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
           success = ContainerBase::getSingleton().loadNextContainerFromStream(input);
           break;
      case cHeaderDial:
           success = Dialogue::getSingleton().loadNextRecordFromStream(input);
           break;
      case cHeaderItem:
           success = ItemBase::getSingleton().loadFromStream(input);
           break;
      case cHeaderJour:
           success = Journal::getSingleton().loadNextFromStream(input);
           break;
      case cHeaderLand:
           land_rec = Landscape::getSingleton().createRecord();
           success = land_rec->loadFromStream(input);
           if (!success)
           {
             Landscape::getSingleton().destroyRecord(land_rec);
           }
           break;
      case cHeaderLight:
           success = LightBase::getSingleton().loadRecordFromStream(input);
           break;
      case cHeaderNPC_:
           success = NPCBase::getSingleton().loadNextRecordFromStream(input);
           break;
      case cHeaderObjS:
           success = ObjectBase::getSingleton().loadFromStream(input);
           break;
      case cHeaderPlay:
           success = Player::getSingleton().loadFromStream(input);
           break;
      case cHeaderQLog:
           success = QuestLog::getSingleton().loadFromStream(input);
           break;
      case cHeaderRefC: //Container
      case cHeaderRefI: //Item
      case cHeaderRefL: //Light
      case cHeaderRefO: //DuskObject
      case cHeaderRfWe: //Weapon
           success = ObjectManager::getSingleton().loadNextFromStream(input, Header);
           break;
      case cHeaderRefA:  //AnimatedObject
      case cHeaderRefN:  //NPC
      case cHeaderRefP:  //Projectiles
      case cHeaderRefV:  //Vehicles
      case cHeaderRfWP:  //WaypointObject
           success = InjectionManager::getSingleton().loadNextFromStream(input, Header);
           break;
      case cHeaderRsrc:
           success = ResourceBase::getSingleton().loadNextResourceFromStream(input);
           break;
      case cHeaderSoun:
           success = SoundBase::getSingleton().loadNextSoundFromStream(input);
           break;
      case cHeaderVehi:
           success = VehicleBase::getSingleton().loadNextVehicleFromStream(input);
           break;
      case cHeaderWeap:
           success = WeaponBase::getSingleton().loadNextWeaponFromStream(input);
           break;
      default:
          DuskLog() << "DataLoader::loadFromFile: ERROR: Got unexpected header "
                    <<Header << " in file \""<<FileName<<"\" at position "
                    <<input.tellg()<<".\n";
          success = false;
          break;
    }//switch

    if(!success or !input.good())
    {
      DuskLog() << "DataLoader::loadFromFile: ERROR while reading data.\n"
                << "Position: "<<input.tellg() << " bytes.\n"
                << "Records read: "<<records_done<<" (excluding failure)\n";
      input.close();
      return false;
    }
    records_done = records_done+1;
  }//while
  input.close();
  DuskLog() << "DataLoader::loadFromFile: Info: "<<records_done<<" records "
            << "loaded from file \""<<FileName<<"\".\n";
  m_LoadedFiles.push_back(FileName);
  return true;
}

void DataLoader::clearData(const unsigned int bits)
{
  if (bits==ALL_BITS)
  {
    m_LoadedFiles.clear();
  }
  if ((bits & REFERENCE_BIT) != 0)
  {
    ObjectManager::getSingleton().clearData();
  }//object references

  if ((bits & OBJECT_BIT) != 0)
  {
    ObjectBase::getSingleton().clearAllObjects();
  }//Object information

  if ((bits & INJECTION_BIT) != 0)
  {
    InjectionManager::getSingleton().clearData();
  }//animated object and NPC references

  if ((bits & CONTAINER_BIT)!=0)
  {
    ContainerBase::getSingleton().deleteAllContainers();
  }//container data

  if ((bits & DIALOGUE_BIT)!=0)
  {
    Dialogue::getSingleton().clearData();
  }//dialogue data

  if ((bits & ITEM_BIT)!=0)
  {
    ItemBase::getSingleton().clearAllItems();
  }//item data

  if ((bits & JOURNAL_BIT) !=0)
  {
    Journal::getSingleton().clearAllEntries();
  }//journal

  if ((bits & LANDSCAPE_BIT) !=0)
  {
    Landscape::getSingleton().clearAllRecords();
  }//landscape

  if ((bits & LIGHT_BIT) !=0)
  {
    LightBase::getSingleton().clearAllData();
  }//lights

  if ((bits & NPC_BIT)!=0)
  {
    NPCBase::getSingleton().clearAllNPCs();
  }//NPC data

  if ((bits & PROJECTILE_BIT) != 0)
  {
    ProjectileBase::getSingleton().clearAll();
  }//projectile information

  if ((bits & QUEST_LOG_BIT) !=0)
  {
    QuestLog::getSingleton().clearAllData();
  }//quest log

  if ((bits & RESOURCE_BIT) != 0)
  {
    ResourceBase::getSingleton().clearAll();
  }//resource data

  if ((bits & SOUND_BIT) != 0)
  {
    SoundBase::getSingleton().clearAll();
  }//sound data

  if ((bits & VEHICLE_BIT) !=0)
  {
    VehicleBase::getSingleton().clearAll();
  }//vehicles

  if ((bits & WEAPON_BIT) !=0)
  {
    WeaponBase::getSingleton().clearAll();
  }//weapons

}//clear data function

bool DataLoader::loadSaveGame(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if(!input)
  {
    DuskLog() << "DataLoader::loadSaveGame: Could not open file \""<<FileName
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
    DuskLog() << "DataLoader::loadSaveGame: file \""<<FileName << "\" is to "
              << "small to contain a real save game.\n";
    return false;
  }

  //read header "Dusk"
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDusk)
  {
    DuskLog() << "DataLoader::loadSaveGame: ERROR: File \""<<FileName
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
    DuskLog() << "DataLoader::loadSaveGame: ERROR: File \""<<FileName
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
    DuskLog() << "DataLoader::loadSaveGame: ERROR: File \""<<FileName
              <<"\" does not contain a valid save game format.\n";
    input.close();
    return false;
  }
  //read dependencies
  Header = 0;
  input.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderDeps)
  {
    DuskLog() << "DataLoader::loadSaveGame: ERROR: File \""<<FileName
              <<"\" does not contain a list of required data files.\n";
    input.close();
    return false;
  }
  //read their number
  unsigned int depCount = 0;
  input.read((char*) &depCount, sizeof(unsigned int));
  if (depCount == 0 or depCount>255)
  { //no reasonable limits given
    DuskLog() << "DataLoader::loadSaveGame: ERROR: File \""<<FileName
              << "\" has a list of required data files which is to long or to"
              << " short (length: "<<depCount<<").\n";
    input.close();
    return false;
  }
  clearData(ALL_BITS);
  m_LoadedFiles.clear();
  unsigned int len;
  char buffer[256];
  for (Header=0; Header<depCount; Header=Header+1)
  {
    len = 0;
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      DuskLog() << "DataLoader::loadSaveGame: ERROR while loading file \""
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
      DuskLog() << "DataLoader::loadSaveGame: ERROR while reading name of "
                << "required data file.\n";
      input.close();
      return false;
    }
    //got name, so load it
    const std::string dataFileName = std::string(buffer);
    if (dataFileName==FileName)
    {
      DuskLog() << "DataLoader::loadSaveGame: ERROR: SaveGame file cannot be a"
                << "required data file of itself.\n";
      input.close();
      return false;
    }
    if (!loadFromFile(dataFileName))
    {
      DuskLog() << "DataLoader::loadSaveGame: ERROR while loading required "
                << "data file \""<<dataFileName<<"\" of save \""<<FileName
                <<"\".\n";
      input.close();
      return false;
    }
    else
    {
      DuskLog() << "DataLoader::loadSaveGame: Info: data file \""<<dataFileName
                <<"\" of save \""<<FileName<<"\" successfully loaded.\n";
      m_LoadedFiles.push_back(dataFileName);
    }
  }//for

  //dependencies are loaded, now clear unneeded data
  clearData(SAVE_MEAN_BITS);
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
           success = InjectionManager::getSingleton().loadNextFromStream(input, Header);
           break;
      case cHeaderRefC: //Container
      case cHeaderRefI: //Item
      case cHeaderRefL: //Light
      case cHeaderRefO: //DuskObject
      case cHeaderRfWe: //Weapon
           success = ObjectManager::getSingleton().loadNextFromStream(input, Header);
           break;
      case cHeaderQLog: //questlog
           success = QuestLog::getSingleton().loadFromStream(input);
           break;
      case cHeaderPlay:
           success = Player::getSingleton().loadFromStream(input);
           break;
      default:
           DuskLog() <<"DataLoader::loadSaveGame: ERROR: Got unexpected header "
                     <<Header << " in file \""<<FileName<<"\" at position "
                     <<input.tellg()<<".\n";
           success = false;
           break;
    }//swi
    if(!success or !input.good())
    {
      DuskLog() << "DataLoader::loadSaveGame: ERROR while reading data.\n"
                << "Position: "<<input.tellg() << " bytes.\n"
                << "Records read: "<<records_done<<" (excluding failure)\n";
      input.close();
      return false;
    }
    records_done = records_done+1;
  }//while
  input.close();
  DuskLog() << "DataLoader::loadSaveGame: Info: "<<records_done<<" out of "
            << data_records<< " expected records loaded.\n";
  return true;
}

bool DataLoader::saveGame(const std::string& FileName) const
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if(!output)
  {
    DuskLog() << "DataLoader::saveGame: Could not open file \""<<FileName
              << "\" for writing in binary mode.\n";
    return false;
  }//if
  //write header "Dusk"
  output.write((char*) &cHeaderDusk, sizeof(unsigned int));
  //determine and write number of records
  unsigned int data_records = 1 /*QuestLog*/ + ObjectManager::getSingleton().getNumberOfReferences()
                              + InjectionManager::getSingleton().getNumberOfReferences()
                              +1 /* Player */;
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
    DuskLog() << "DataLoader::saveGame: ERROR while writing header data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  //write the data
  if (!ObjectManager::getSingleton().saveAllToStream(output))
  {
    DuskLog() << "DataLoader::saveGame: ERROR while writing object data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!InjectionManager::getSingleton().saveAllToStream(output))
  {
    DuskLog() << "DataLoader::saveGame: ERROR while writing animation data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!Player::getSingleton().saveToStream(output))
  {
    DuskLog() << "DataLoader::saveGame: ERROR while writing player data to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  if (!QuestLog::getSingleton().saveToStream(output))
  {
    DuskLog() << "DataLoader::saveGame: ERROR while writing quest log to "
              << "file \""<<FileName<<"\".\n";
    output.close();
    return false;
  }
  output.close();
  return true;
}

}//namespace
