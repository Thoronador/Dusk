/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Database.h"
#include "ContainerRecord.h"
#include "../DuskConstants.h"
#include "ItemRecord.h"
#include "LightRecord.h"
#include "NPCRecord.h"
#include "ObjectRecord.h"
#include "ProjectileRecord.h"
#include "ResourceRecord.h"
#include "SoundRecord.h"
#include "VehicleRecord.h"
#include "WeaponRecord.h"

namespace Dusk
{

Database::Database()
{
  //empty
}

Database::~Database()
{
  deleteAllRecords();
}

Database& Database::getSingleton()
{
  static Database Instance;
  return Instance;
}

void Database::addRecord(DataRecord* record)
{
  if (record==NULL) return;
  if (record->ID.empty()) return;
  std::map<std::string, DataRecord*>::iterator iter = m_Records.find(record->ID);
  if (iter==m_Records.end())
  {
    //just insert it
    m_Records[record->ID] = record;
  }
  else
  {
    if (record==iter->second) return; //avoid replacing with itself
    //save old record first (for deletion)
    DataRecord * temp = iter->second;
    //replace it by new record
    iter->second = record;
    delete temp;
  }
}

#ifdef DUSK_EDITOR
bool Database::deleteRecord(const std::string& ID)
{
  std::map<std::string, DataRecord*>::iterator iter = m_Records.find(ID);
  if (iter!=m_Records.end())
  {
    //save old record first (for deletion)
    DataRecord * temp = iter->second;
    m_Records.erase(iter);
    delete temp;
    return true;
  }
  //nothing found, nothing deleted
  return false;
}
#endif

bool Database::hasRecord(const std::string& ID) const
{
  return (m_Records.find(ID)!=m_Records.end());
}

const DataRecord& Database::getRecord(const std::string& recordID) const
{
  std::map<std::string, DataRecord*>::const_iterator iter = m_Records.find(recordID);
  if (iter!=m_Records.end())
  {
    return *(iter->second);
  }
  DuskLog() << "Database::getRecord: ERROR: no record with ID \"" << recordID
            << "\" found. Exception will be thrown.\n";
  throw IDNotFound("Database", recordID);
}

void Database::deleteAllRecords()
{
  std::map<std::string, DataRecord*>::iterator iter = m_Records.begin();
  while (!m_Records.empty())
  {
    DataRecord* temp = iter->second;
    m_Records.erase(iter);
    iter = m_Records.begin();
    delete temp;
  }//while
}

unsigned int Database::getNumberOfRecords() const
{
  return m_Records.size();
}

bool Database::saveAllToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "Database::saveAllToStream: ERROR: bad stream.\n";
    return false;
  }

  std::map<std::string, DataRecord*>::const_iterator iter;
  for(iter=m_Records.begin(); iter!=m_Records.end(); ++iter)
  {
    if (!iter->second->saveToStream(outStream))
    {
      DuskLog() << "Database::saveToStream: ERROR while writing data.\n";
      return false;
    }//if
  }//for
  return true;
}

bool Database::loadNextRecordFromStream(std::ifstream& inStream, const uint32_t header)
{
  if (!inStream.good())
  {
    DuskLog() << "Database::loadNextRecordFromStream: ERROR: bad stream.\n";
    return false;
  }//if

  DataRecord* recordPtr = NULL;
  switch (header)
  {
    //TODO: more case labels for different headers
    /*
    case cHeaderConstantValue:
         recordPtr = new SomeRecordType;
         break;
    */
    case cHeaderCont:
         recordPtr = new ContainerRecord;
         break;
    case cHeaderItem:
         recordPtr = new ItemRecord;
         break;
    case cHeaderLight:
         recordPtr = new LightRecord;
         break;
    case cHeaderNPC_:
         recordPtr = new NPCRecord;
         break;
    case cHeaderObjS:
         recordPtr = new ObjectRecord;
         break;
    case cHeaderProj:
         recordPtr = new ProjectileRecord;
         break;
    case cHeaderRsrc:
         recordPtr = new ResourceRecord;
         break;
    case cHeaderSoun:
         recordPtr = new SoundRecord;
         break;
    case cHeaderVehi:
         recordPtr = new VehicleRecord;
         break;
    case cHeaderWeap:
         recordPtr = new WeaponRecord;
         break;
    default:
         DuskLog() << "Database::loadNextRecordFromStream: ERROR: unexpected header.\n";
         return false;
         break;
  }//swi

  if (recordPtr->loadFromStream(inStream))
  {
    addRecord(recordPtr);
    return true;
  }
  delete recordPtr;
  recordPtr = NULL;
  DuskLog() << "Database::loadNextRecordFromStream: ERROR while reading data.\n";
  return false;
}

#ifdef DUSK_EDITOR
Database::Iterator Database::getFirst() const
{
  return m_Records.begin();
}

Database::Iterator Database::getEnd() const
{
  return m_Records.end();
}

void Database::updateProjectilesAfterIDChange(const std::string& oldProjID, const std::string& newProjID)
{
  if ((oldProjID.empty()) or (newProjID.empty()) or (oldProjID==newProjID))
  {
    //We do not want empty IDs, and we do not need to update anything, if old
    //  and new ID are equal.
    return;
  }
  //iterate through all records which are weapons and look for ID of projectile
  std::map<std::string, DataRecord*>::iterator iter = m_Records.begin();
  while (iter!=m_Records.end())
  {
    if (iter->second->getRecordType() == WeaponRecord::RecordType)
    {
      if (static_cast<WeaponRecord*>(iter->second)->ProjectileID == oldProjID)
      {
        static_cast<WeaponRecord*>(iter->second)->ProjectileID = newProjID;
      }//if
    }//if projectile
    ++iter;
  }//while
}
#endif

} //namespace
