#include "Journal.h"
#include <iostream>
#include "DuskConstants.h"

namespace Dusk
{

const uint8 JournalRecord::FinishedFlag = 1;
const uint8 JournalRecord::DeletedFlag = 128;

bool JournalRecord::isDeleted() const
{
  return ((Flags& DeletedFlag)>0);
}

bool JournalRecord::isFinisher() const
{
  return ((Flags&FinishedFlag)>0);
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

Journal& Journal::GetSingleton()
{
  static Journal Instance;
  return Instance;
}

bool Journal::setQuestName(const std::string& JID, const std::string& qName)
{
  if (JID=="" or qName=="")
  {
    std::cout << "Journal::setQuestName: ERROR: ID or new quest name is empty "
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
    std::cout << "Journal::addEntry: ERROR: empty ID string or zero index "
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
    return false; //nothing to delete found
  }
  std::map<const unsigned int, JournalRecord>::iterator indexIter;
  indexIter = iter->second.Indices.find(jIndex);
  if (indexIter==iter->second.Indices.end())
  {
    return false;
  }
  iter->second.Indices.erase(indexIter);
  --m_TotalEntries;
  return true;
}

unsigned int Journal::NumberOfEntries() const
{
  return m_TotalEntries;
}

unsigned int Journal::NumberOfDistinctQuests() const
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

bool Journal::SaveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "Journal::SaveAllToStream: ERROR: bad stream!\n";
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
      std::cout << "Journal::SaveAllToStream: ERROR while writing quest ID!\n";
      return false;
    }
    //write quest name
    len = iter->second.QuestName.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(iter->second.QuestName.c_str(), len);
    if (!(output.good()))
    {
      std::cout << "Journal::SaveAllToStream: ERROR while writing quest name!\n";
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
        std::cout << "Journal::SaveAllToStream: ERROR while writing "
                  << "JournalRecord!\n";
        return false;
      }
      ++index_iter;
    } //while (inner loop)
    ++iter;
  } //while (outer loop)
  return output.good();
}

bool Journal::LoadNextFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    std::cout << "Journal::LoadNextFromStream: ERROR: bad stream!\n";
    return false;
  }
  unsigned int len = 0;
  input.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderJour)
  {
    std::cout << "Journal::LoadNextFromStream: ERROR: invalid record header!\n";
    return false;
  }
  //read quest ID
  input.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    std::cout << "Journal::LoadNextFromStream: ERROR: quest ID seems to be "
              << "longer than 511 characters!\n";
    return false;
  }
  char buffer[512];
  buffer[0] = buffer[511] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    std::cout << "Journal::LoadNextFromStream: ERROR while reading quest ID!\n";
    return false;
  }
  const std::string QuestID = std::string(buffer);
  //read quest name
  input.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    std::cout << "Journal::LoadNextFromStream: ERROR: quest name seems to be "
              << "longer than 511 characters!\n";
    return false;
  }
  buffer[0] = buffer[511] = '\0';
  input.read(buffer, len);
  buffer[len] = '\0';
  if (!(input.good()))
  {
    std::cout << "Journal::LoadNextFromStream: ERROR while reading quest name!\n";
    return false;
  }
  setQuestName(QuestID, std::string(buffer));
  //now read the index subrecords
  // -- read their number
  unsigned int indexCount = 0;
  input.read((char*) &indexCount, sizeof(unsigned int));
  if (indexCount==0)
  {
    std::cout << "Journal::LoadNextFromStream: Hint: there are no records! "
              << "Aborting.\n";
    return true;
  }
  if (indexCount>100) //can there really be so much for one single(!) quest?
  {
    std::cout << "Journal::LoadNextFromStream: ERROR: there seem to be more "
              << "than 100 subrecords for quest \""<<QuestID<<"\", which is "
              << "most likely to much. Aborting.\n";
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
      std::cout << "Journal::LoadNextFromStream: ERROR: got zero as journal "
                << "index for one subrecord of quest \""<<QuestID<<"\", but "
                << "zero is not a valid index. Aborting.\n";
      return false;
    }
    //read text
    len = 0;
    input.read((char*) &len, sizeof(unsigned int));
    if (len>511)
    {
      std::cout << "Journal::LoadNextFromStream: ERROR: text for quest \""
                << QuestID<<"\", index "<<curIndex<<", seems to be longer than"
                << " 511 characters!\n";
      return false;
    }
    input.read(buffer, len);
    buffer[len] = '\0';
    if (!(input.good()))
    {
      std::cout << "Journal::LoadNextFromStream: ERROR while reading journal "
                << "text for quest \""<<QuestID<<"\", index "<<curIndex<<".\n";
      return false;
    }
    tempRec.Text = std::string(buffer);
    //read flags
    input.read((char*) &(tempRec.Flags), 1);
    if (!(input.good()))
    {
      std::cout << "Journal::LoadNextFromStream: ERROR while reading journal "
                << "flags of quest \""<<QuestID<<"\", index "<<curIndex<<".\n";
      return false;
    }
    addEntry(QuestID, curIndex, tempRec);
  }//for
  return input.good();
}

void Journal::ClearAllEntries()
{
  m_Entries.clear();
  m_TotalEntries = 0;
}

} //namespace
