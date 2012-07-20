/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011 thoronador

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
 Date:    2010-03-08
 Purpose: QuestLog Singleton class
          holds all quest entries the player actually gets during the game
          (This is not to be confused with the class Journal!)

 History:
     - 2010-02-05 (rev 163) - initial version (by thoronador)
     - 2010-03-08 (rev 178) - getMaximumPresentIndex() added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-02-05 (rev 278) - isQuestFailed() and listFailedQuests() added

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef QUESTLOG_H
#define QUESTLOG_H

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "DuskTypes.h"

namespace Dusk
{
  struct QLogEntry
  {
    std::string questID;
    unsigned int index;
  };

  class QuestLog
  {
    public:
      /* singleton access */
      static QuestLog& getSingleton();

      /* destructor */
      virtual ~QuestLog();

      /* adds the quest entry for quest questID and index index to the QuestLog

         return value:
             returns true, if the entry was added. Returns false, if not. This
             will also return false, if that entry is already present.
      */
      bool addQuestEntry(const std::string& questID, const unsigned int index);

      /* returns true, if at least one entry for quest questID is present */
      bool hasQuest(const std::string& questID) const;

      /* returns true, if the given quest is already finished */
      bool isQuestFinished(const std::string& questID) const;

      /* returns true, if the given quest has already failed */
      bool isQuestFailed(const std::string& questID) const;

      /* returns true, if the entry of quest questID and index index is present */
      bool hasQuestEntry(const std::string& questID, const unsigned int index) const;

      /* returns the index of the quest entry with the highest index present in
         the QuestLog
      */
      unsigned int getMaximumPresentIndex(const std::string& questID) const;

      /* returns the vector of all quests that have been finished yet */
      std::vector<std::string> listFinishedQuests() const;

      /* returns the vector of all quests that have already failed */
      std::vector<std::string> listFailedQuests() const;

      /* returns the vector of all quests that have NOT been finished and have
         NOT failed yet
      */
      std::vector<std::string> listActiveQuests() const;

      /* returns a vector of all quest entries in the QuestLog in chronological
         order, skipping offset entries and returning not more than limit
         entries

         parameters:
             offset - the number of entries to skip (zero, if you want to start
                       at the beginning)
             limit  - the maximum number of entries to return. Default is 30.
                      If limit is set to zero, all available entries will be
                      returned.
      */
      std::vector<QLogEntry> listQuestEntries(const unsigned int offset, unsigned int limit=30) const;

      /* deletes all data, producing an empty QuestLog */
      void clearAllData();

      /* returns the number of quest entries in QuestLog */
      unsigned int getNumberOfQuestEntries() const;

      /* tries to save data to stream and returns true on success, false otherwise

         parameters:
             output - the output stream that is used to save the quest log
      */
      bool saveToStream(std::ofstream& output) const;

      /* tries to load all data from stream and returns true on success, false
         otherwise

         parameters:
             input - the input stream that is used to load the quest log
      */
      bool loadFromStream(std::ifstream& input);
    private:
      /* constructor - private due to singleton pattern */
      QuestLog();

      /* empty, private copy constructor due to singleton */
      QuestLog(const QuestLog& op) {}

      /* vector which keeps track of all recieved quest entries in chronological
         order

         remarks:
             This is the only "real" data of the QuestLog class. All other data
             mambers (m_PresentEntries and m_FinishedQuests) are just for faster
             lookup of certain data bits.
      */
      std::vector<QLogEntry> m_TimeLine;

      /* map to allow fast(er) presence checks without going through the above
         vector

         remarks:
             The second unsigned int in the second map is the index of the
             QLogEntry structure within the vector m_TimeLine.
      */
      std::map<std::string, std::map<unsigned int, unsigned int> > m_PresentEntries;

      /* map for retrieving quest state without iterating through vector */
      std::map<std::string, uint8_t> m_StateOfQuests;

  }; //class Questlog

} //namespace

#endif // QUESTLOG_H
