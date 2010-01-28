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

bool Journal::addEntry(const std::string& JID, const unsigned int jIndex,
                       const JournalRecord& jData)
{
  if (JID=="" or jIndex==0)
  {
    std::cout << "Journal::addEntry: ERROR: empty ID string or zero index "
              << "given. No data will be added.\n";
    return false;
  }
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    m_Entries[JID] = std::map<const unsigned int, JournalRecord>();
    iter = m_Entries.find(JID);
  }
  if (iter->second.find(jIndex)==iter->second.end())
  {
    m_TotalEntries = m_TotalEntries+1;
  }
  iter->second[jIndex] = jData;
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
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return false;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.find(jIndex);
  if (index_iter == iter->second.end())
  {
    return false;
  }
  return (!(index_iter->second.isDeleted()));
}

std::string Journal::getText(const std::string& JID, const unsigned int jIndex) const
{
  if (JID=="" or jIndex==0)
  { //we don't have empty IDs or index zero, so return "nothing"
    return "";
  }
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return "";
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.find(jIndex);
  if (index_iter!=iter->second.end())
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
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
  iter = m_Entries.find(JID);
  if (iter==m_Entries.end())
  {
    return 0;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.find(jIndex);
  if (index_iter!=iter->second.end())
  {
    return index_iter->second.Flags;
  }
  return 0;
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
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
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
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
  iter = m_Entries.find(jID);
  if (iter==m_Entries.end())
  {
    return uintVec;
  }
  std::map<const unsigned int, JournalRecord>::const_iterator index_iter;
  index_iter = iter->second.begin();
  while (index_iter!=iter->second.end())
  {
    if (!(index_iter->second.isDeleted()) or listDeleted)
    {
      uintVec.push_back(index_iter->first);
    }
    ++index_iter;
  }//while
  return uintVec;
}

bool Journal::SaveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "Journal::SaveAllToStream: ERROR: bad stream!\n";
    return false;
  }
  std::map<const std::string, std::map<const unsigned int, JournalRecord> >::const_iterator iter;
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

    //now write all subordinated records
    // --- write number of records
    len = iter->second.size();
    output.write((char*) &len, sizeof(unsigned int));
    // --- write records
    index_iter = iter->second.begin();
    while (index_iter!=iter->second.end())
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
  //not implemented yet
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
  //now read the subrecords
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
