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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2012-07-01
 Purpose: Database singleton class
          Represents a (sort of) lookup table for basic data of distinct objects
          in the game

 History:
     - 2012-07-01 (rev 309) - initial version (by thoronador)
     - 2012-07-02 (rev 310) - update for ItemRecord, LightRecord and ObjectRecord
     - 2012-07-05 (rev 313) - update for ContainerRecord
     - 2012-07-05 (rev 314) - new version of addRecord()

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_DATABASE_H
#define DUSK_DATABASE_H

#include <map>
#include "DataRecord.h"
#include "DuskExceptions.h"
#include "Messages.h"

namespace Dusk
{

  class Database
  {
    public:
      /* destructor */
      virtual ~Database();

      /* singleton access method */
      static Database& getSingleton();

      /* Adds a record to the database.

         parameters:
             record - pointer to the record (should not be NULL)

         remarks:
             If a record with the same ID already exists, the old data will be
             replaced by the new data.
             If the record pointer is NULL, no data will be added. This also
             holds true, if the record's ID is an empty string.

             The record pointer must NOT be freed by the application, the
             Database class will take care of this, when neccessary.
      */
      void addRecord(DataRecord* record);

      /* Adds a record to the database.

         parameters:
             record - the record

         remarks:
             If a record with the same ID already exists, the old data will be
             replaced by the new data.
             If the record's ID is an empty string, no data will be added.
      */
      template<typename recT>
      void addRecord(const recT& record);

      #ifdef DUSK_EDITOR
      /* Deletes record with given ID. Returns true, if such a record was
         present (and thus deleted), false otherwise.

         parameter:
             ID - ID of the record that should be deleted
      */
      bool deleteRecord(const std::string& ID);
      #endif

      /* Returns true, if a record with the given ID is present, false
         otherwise.

         parameters:
             ID - ID of the record which will be checked
      */
      bool hasRecord(const std::string& ID) const;

      /* Returns true, if a record with the given ID and type is present, false
         otherwise.

         parameters:
             ID - ID of the record which will be checked
      */
      bool hasTypedRecord(const std::string& ID, const uint32_t type) const;

      template<typename recT>
      bool hasTypedRecord(const std::string& ID) const;

      /* returns the record with the given ID. If no such record is present, the
         function will throw an exception.

         parameters:
             recordID - ID of the requested record
      */
      const DataRecord& getRecord(const std::string& recordID) const;

      /* returns the record with the given ID. If no such record is present, the
         function will throw an exception. The difference to getRecord() is that
         this function can return the derived, actual record type, not just the
         abstract DataRecord type. However, if the getRecordType() function of
         the found record does not return the value cHead, the function will
         throw an exception. Otherwise the record is cast to recT type and will
         be returned.

         parameters:
             recordID - ID of the requested record
      */
      template<typename recT>
      const recT& getTypedRecord(const std::string& recordID) const;

      /* Removes all records. */
      void deleteAllRecords();

      /* Returns number of currently available records */
      unsigned int getNumberOfRecords() const;

      /* Saves all records to stream; returns true on success

         parameters:
             outStream - the output stream to which the records will be saved
      */
      bool saveAllToStream(std::ofstream& outStream) const;

      /* Loads one(!) single record from the stream; returns true on success,
         false otherwise. The data of the last loaded record is probably
         inconsistent after that function failed, so don't rely on it in that
         case.

         parameters:
             inStream - the input stream from which the record will be read
             header   - header of the record
      */
      bool loadNextRecordFromStream(std::ifstream& inStream, const uint32_t header);

      #ifdef DUSK_EDITOR
      //iterator type for iterating through the records
      typedef std::map<std::string, DataRecord*>::const_iterator Iterator;

      /* helper functions to access internal map iterators - not used in-game,
         only used by Editor application.
      */
      Iterator getFirst() const;
      Iterator getEnd() const;
      #endif

    private:
      /* constructor - private due to singleton pattern */
      Database();
      /* copy constructor */
      Database(const Database& op) {}
      std::map<std::string, DataRecord*> m_Records;
  };//class

  template<typename recT>
  void Database::addRecord(const recT& record)
  {
    if (record.ID.empty()) return;
    std::map<std::string, DataRecord*>::iterator iter = m_Records.find(record.ID);
    recT * recPtr = new recT;
    *recPtr = record;
    if (iter==m_Records.end())
    {
      //just insert it
      m_Records[record.ID] = recPtr;
    }
    else
    {
      //save old record first (for deletion)
      DataRecord * temp = iter->second;
      //replace it by new record
      iter->second = recPtr;
      delete temp;
    }
  }

  template<typename recT>
  bool Database::hasTypedRecord(const std::string& ID) const
  {
    return hasTypedRecord(ID, recT::RecordType);
  }

  template<typename recT>
  const recT& Database::getTypedRecord(const std::string& recordID) const
  {
    std::map<std::string, DataRecord*>::const_iterator iter = m_Records.find(recordID);
    if (iter==m_Records.end())
    {
      DuskLog() << "Database::getTypedRecord: ERROR: no record with ID \""
                << recordID << "\" found. Exception will be thrown.\n";
      throw IDNotFound("Database", recordID);
    }
    if (iter->second->getRecordType()==recT::RecordType)
    {
      return static_cast<const recT&>(*(iter->second));
    }
    DuskLog() << "Database::getTypedRecord: ERROR: record with ID \""
              << recordID << "\" does not have the specified type. Exception "
              << "will be thrown.\n";
    throw IDNotFound("Database", recordID);
  }

} //namespace

#endif // DUSK_DATABASE_H
