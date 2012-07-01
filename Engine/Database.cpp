/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

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

#include "Database.h"
#include "DuskExceptions.h"
#include "Messages.h"

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

const DataRecord& Database::getRecord(const std::string& ID) const
{
  std::map<std::string, DataRecord*>::const_iterator iter = m_Records.find(ID);
  if (iter!=m_Records.end())
  {
    return *(iter->second);
  }
  DuskLog() << "Dataase::getRecord: ERROR: no record with ID \"" << ID
            << "\" found. Exception will be thrown.\n";
  throw IDNotFound("Database", ID);
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
  m_Records.end();
}
#endif

} //namespace
