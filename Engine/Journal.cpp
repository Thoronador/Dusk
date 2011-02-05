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

#include "Journal.h"
#include "Messages.h"
#include "DuskConstants.h"

namespace Dusk
{

const uint8 JournalRecord::FinishedFlag = 1;
const uint8 JournalRecord::FailedFlag = 2;
const uint8 JournalRecord::DeletedFlag = 128;

bool JournalRecord::isDeleted() const
{
  return ((Flags& DeletedFlag)>0);
}

bool JournalRecord::isFailure() const
{
  return ((Flags& FailedFlag)>0);
}

bool JournalRecord::isFinisher() const
{
  return ((Flags&FinishedFlag)>0);
}

std::string JournalRecord::flagsToString(const uint8 theFlags)
{
  std::string result = "";
  if ((theFlags&DeletedFlag)>0) result = result +"D";
  if ((theFlags&FinishedFlag)>0) result = result +"F";
  if ((theFlags&FailedFlag)>0) result = result +"X";
  return result;
}

std::string JournalRecord::flagsToString() const
{
  return flagsToString(Flags);
}

const std::string Journal::cUnnamedQuest = "(quest has no name)";

Journal::Journal()
{
  m_TotalEntries = 0;
}

Journal::~Journal()
{
  //empty
}

Journal& Journal::getSingleton()
{
  static Journal Instance;
  return Instance;
}

bool Journal::setQuestName(const std::string& JID, const std::string& qName)
{
  if (JID=="" or qName=="")
  {
    DuskLog() << "Journal::setQuestName: ERROR: ID or new quest name is empty "
              << "string. No data will be added or changed.\n";
    return false;
  }
  std::map<const std::string, QuestRecord>::iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    m_Entries[JID].Indices = std::map<const unsigned int, JournalRecord>();
    m_Entries[JID].QuestName = qName;
    return true;
  }
  iter->second.QuestName = qName;
  return true;
}

bool Journal::addEntry(const std::string& JID, const unsigned int jIndex,
                       const JournalRecord& jData)
{
  if (JID=="" or jIndex==0)
  {
    DuskLog() << "Journal::addEntry: ERROR: empty ID string or zero index "
              << "given. No data will be added.\n";
    return false;
  }
  std::map<const std::string, QuestRecord>::iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    m_Entries[JID].Indices = std::map<const unsigned int, JournalRecord>();
    m_Entries[JID].QuestName = cUnnamedQuest;
    iter = m_Entries.find(JID);
  }
  if (iter->second.Indices.find(jIndex)==iter->second.Indices.end())
  {
    m_TotalEntries = m_TotalEntries+1;
  }
  iter->second.Indices[jIndex] = jData;
  return true;
}

bool Journal::addEntry(const std::string& JID, const unsigned int jIndex,
                       const std::string& jText, const uint8 jFlags)
{
  JournalRecord temp;
  temp.Text = jText;
  temp.Flags = jFlags;
  return addEntry(JID, jIndex, temp);
}

bool Journal::hasEntry(const std::string& JID, const unsigned int jIndex) const
{
  if (JID=="" or jIndex==0)
  { //we don't allow empty IDs or index zero, so we don't have them
    return false;
  }
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return false;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.Indices.find(jIndex);
  if (index_iter == iter->second.Indices.end())
  {
    return false;
  }
  return (!(index_iter->second.isDeleted()));
}

bool Journal::hasQuest(const std::string& questID) const
{
  if (questID=="")
  { //we don't allow empty IDs, that's why we don't have them
    return false;
  }
  return (m_Entries.find(questID)!=m_Entries.end());
}

#ifdef DUSK_EDITOR
bool Journal::changeQuestID(const std::string& oldID, const std::string& newID)
{
  if (oldID=="" or newID=="" or hasQuest(newID))
  {
    //we don't want empty IDs or change it to an existing quest ID
    return false;
  }
  std::map<const std::string, QuestRecord>::iterator old_iter;
  old_iter = m_Entries.find(oldID);
  if (old_iter==m_Entries.end())
  { //there is nothing we can change
    return false;
  }
  setQuestName(newID, old_iter->second.QuestName);
  std::map<const std::string, QuestRecord>::iterator new_iter;
  new_iter = m_Entries.find(newID); /*new_iter will not be end(), because the
  call to setQuestName(newID,...) above creates that entry */
  while (!old_iter->second.Indices.empty())
  {
    //insert entry to new quest ID
    new_iter->second.Indices[old_iter->second.Indices.begin()->first]
       =  old_iter->second.Indices.begin()->second;
    //delete it from old quest ID
    old_iter->second.Indices.erase(old_iter->second.Indices.begin());
  }
  //delete the old quest entry
  m_Entries.erase(old_iter);
  return true;
}
#endif

std::string Journal::getText(const std::string& JID, const unsigned int jIndex) const
{
  if (JID=="" or jIndex==0)
  { //we don't have empty IDs or index zero, so return "nothing"
    return "";
  }
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return "";
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.Indices.find(jIndex);
  if (index_iter!=iter->second.Indices.end())
  {
    return index_iter->second.Text;
  }
  return "";
}

uint8 Journal::getFlags(const std::string& JID, const unsigned int jIndex) const
{
  if (JID=="" or jIndex==0)
  { //we don't have empty IDs or index zero, so return zero
    return 0;
  }
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return 0;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.Indices.find(jIndex);
  if (index_iter!=iter->second.Indices.end())
  {
    return index_iter->second.Flags;
  }
  return 0;
}

std::string Journal::getQuestName(const std::string& questID) const
{
  if (questID=="")
  { //we don't have quests with empy string as ID
    return "";
  }
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(questID);
  if (iter==m_Entries.end())
  {
    return ""; //nothing found
  }
  return iter->second.QuestName;
}

#ifdef DUSK_EDITOR
bool Journal::deleteQuest(const std::string& questID)
{
  std::map<const std::string, QuestRecord>::iterator iter;
  iter = m_Entries.find(questID);
  if (iter==m_Entries.end())
  {
    return false; //nothing to delete found
  }
  m_Entries.erase(iter);
  return true;
}

bool Journal::deleteEntry(const std::string& questID, const unsigned int jIndex)
{
  std::map<const std::string, QuestRecord>::iterator iter;
  iter = m_Entries.find(questID);
  if (iter==m_Entries.end())
  {
    DuskLog() << "Journal::deleteEntry: Hint: no quest named \""<<questID
              << "\" found.\n";
    return false; //nothing to delete found
  }
  std::map<const unsigned int, JournalRecord>::iterator indexIter;
  indexIter = iter->second.Indices.find(jIndex);
  if (indexIter==iter->second.Indices.end())
  {
    DuskLog() << "Journal::deleteEntry: Hint: quest \""<<questID<< "\" has no "
              << "entry with index "<<jIndex<<".\n";
    return false;
  }
  iter->second.Indices.erase(indexIter);
  --m_TotalEntries;
  return true;
}
#endif

unsigned int Journal::numberOfEntries() const
{
  return m_TotalEntries;
}

unsigned int Journal::numberOfDistinctQuests() const
{
  return m_Entries.size();
}

std::vector<std::string> Journal::listAllQuestIDs() const
{
  std::vector<std::string> sv;
  sv.clear();
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.begin();
  while (iter!=m_Entries.end())
  {
    sv.push_back(iter->first);
    ++iter;
  }//while
  return sv;
}

std::vector<unsigned int> Journal::listAllIndicesOfQuest(const std::string& jID, const bool listDeleted) const
{
  std::vector<unsigned int> uintVec;
  uintVec.clear();
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(jID);
  if (iter==m_Entries.end())
  {
    return uintVec;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.Indices.begin();
  while (index_iter!=iter->second.Indices.end())
  {
    if (!(index_iter->second.isDeleted()) or listDeleted)
    {
      uintVec.push_back(index_iter->first);
    }
    ++index_iter;
  }//while
  return uintVec;
}

unsigned int Journal::getMaximumAvailabeIndex(const std::string& jID) const
{
  std::map<const std::string, QuestRecord>::const_iterator iter;
  iter = m_Entries.find(jID);
  if (iter==m_Entries.end())
  {
    return 0;
  }
  if (iter->second.Indices.empty())
  {
    return 0;
  }
  std::map<const unsigned int, JournalRecord>::const_reverse_iterator index_iter;
  index_iter = iter->second.Indices.rbegin();
  return index_iter->first;
}

bool Journal::saveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    DuskLog() << "Journal::saveAllToStream: ERROR: bad stream!\n";
    return false;
  }
  std::map<const std::string, QuestRecord>::const_iterator iter;
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  unsigned int len;
  iter = m_Entries.begin();
  while (iter!=m_Entries.end())
  {
    //write header
    output.write((char*) &cHeaderJour, sizeof(cHeaderJour));
    //write quest ID
    len = iter->first.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->first.c_str(), len);
    if (!(output.good()))
    {
      DuskLog() << "Journal::saveAllToStream: ERROR while writing quest ID!\n";
      return false;
    }
    //write quest name
    len = iter->second.QuestName.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.QuestName.c_str(), len);
    if (!(output.good()))
    {
      DuskLog() << "Journal::saveAllToStream: ERROR while writing quest name!\n";
      return false;
    }

    //now write all subordinated index records
    // --- write number of records
    len = iter->second.Indices.size();
    output.write((char*) &len, sizeof(unsigned int));
    // --- write records
    index_iter = iter->second.Indices.begin();
    while (index_iter!=iter->second.Indices.end())
    {
      //write index
      len = index_iter->first;
      output.write((char*) &len, sizeof(unsigned int));
      //write text
      len = index_iter->second.Text.length();
      output.write((char*) &len, sizeof(unsigned int));
      output.write(index_iter->second.Text.c_str(), len);
      //write flags
      output.write((char*) &(index_iter->second.Flags), 1);
      if (!(output.good()))
      {
        DuskLog() << "Journal::saveAllToStream: ERROR while writing "
                  << "JournalRecord!\n";
        return false;
      }
      ++index_iter;
    } //while (inner loop)
    ++iter;
  } //while (outer loop)
  return output.good();
}

bool Journal::loadNextFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR: bad stream!\n";
    return false;
  }
  unsigned int len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderJour)
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR: invalid record header!\n";
    return false;
  }
  //read quest ID
  input.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR: quest ID seems to be "
              << "longer than 511 characters!\n";
    return false;
  }
  char buffer[512];
  buffer[0] = buffer[511] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR while reading quest ID!\n";
    return false;
  }
  const std::string QuestID = std::string(buffer);
  //read quest name
  input.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR: quest name of quest \""
              <<QuestID <<"\" seems to be longer than 511 characters!\n";
    return false;
  }
  buffer[0] = buffer[511] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR while reading quest name!\n";
    return false;
  }
  setQuestName(QuestID, std::string(buffer));
  //now read the index subrecords
  // -- read their number
  unsigned int indexCount = 0;
  input.read((char*) &indexCount, sizeof(unsigned int));
  if (indexCount==0)
  {
    DuskLog() << "Journal::loadNextFromStream: Hint: there are no records! "
              << "Aborting.\n";
    return true;
  }
  if (indexCount>100) //can there really be so much for one single(!) quest?
  {
    DuskLog() << "Journal::loadNextFromStream: ERROR: there seem to be more "
              << "than 100 subrecords for quest \""<<QuestID<<"\", which is "
              << "most likely too much. Aborting.\n";
    return false;
  }
  //now read the records
  JournalRecord tempRec;
  unsigned int i, curIndex=0;
  for (i=0; i<indexCount; i=i+1)
  {
    //read index
    input.read((char*) &curIndex, sizeof(unsigned int));
    if (curIndex==0)
    {
      DuskLog() << "Journal::loadNextFromStream: ERROR: got zero as journal "
                << "index for one subrecord of quest \""<<QuestID<<"\", but "
                << "zero is not a valid index. Aborting.\n";
      return false;
    }
    //read text
    len = 0;
    input.read((char*) &len, sizeof(unsigned int));
    if (len>511)
    {
      DuskLog() << "Journal::loadNextFromStream: ERROR: text for quest \""
                << QuestID<<"\", index "<<curIndex<<", seems to be longer than"
                << " 511 characters!\n";
      return false;
    }
    input.read(buffer, len);
    buffer[len] = '\0';
    if (!(input.good()))
    {
      DuskLog() << "Journal::loadNextFromStream: ERROR while reading journal "
                << "text for quest \""<<QuestID<<"\", index "<<curIndex<<".\n";
      return false;
    }
    tempRec.Text = std::string(buffer);
    //read flags
    input.read((char*) &(tempRec.Flags), 1);
    if (!(input.good()))
    {
      DuskLog() << "Journal::loadNextFromStream: ERROR while reading journal "
                << "flags of quest \""<<QuestID<<"\", index "<<curIndex<<".\n";
      return false;
    }
    addEntry(QuestID, curIndex, tempRec);
  }//for
  return input.good();
}

void Journal::clearAllEntries()
{
  m_Entries.clear();
  m_TotalEntries = 0;
}

} //namespace
