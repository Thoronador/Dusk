#include "QuestLog.h"
#include "Journal.h"
#include "DuskConstants.h"
#include <iostream>

namespace Dusk
{

QuestLog& QuestLog::GetSingleton()
{
  static QuestLog Instance;
  return Instance;
}

QuestLog::~QuestLog()
{
  //empty
}

QuestLog::QuestLog()
{
  //empty
}

bool QuestLog::addQuestEntry(const std::string& questID, const unsigned int index)
{
  if (questID=="" or index==0)
  {
    return false;
  }
  std::map<std::string, std::map<unsigned int, unsigned int> >::iterator iter;
  iter = m_PresentEntries.find(questID);
  if (iter==m_PresentEntries.end())
  {
    //add it
    m_PresentEntries[questID] = std::map<unsigned int, unsigned int>();
    iter = m_PresentEntries.find(questID);
  }
  std::map<unsigned int, unsigned int>::iterator index_iter;
  index_iter = iter->second.find(index);
  if (index_iter == iter->second.end())
  {
    //not present -> add it
    iter->second[index] = m_TimeLine.size();
    QLogEntry temp;
    temp.questID = questID;
    temp.index = index;
    m_TimeLine.push_back(temp);
    m_FinishedQuests[questID] = m_FinishedQuests[questID] or
         ((Journal::GetSingleton().getFlags(questID, index)&JournalRecord::FinishedFlag)>0);
    return true;
  }
  //entry was already present, so we add nothing
  return false;
}

bool QuestLog::hasQuest(const std::string& questID) const
{
  return (m_PresentEntries.find(questID) != m_PresentEntries.end());
}

bool QuestLog::isQuestFinished(const std::string& questID) const
{
  std::map<std::string, bool>::const_iterator iter = m_FinishedQuests.find(questID);
  if (iter==m_FinishedQuests.end())
  {
    return false;
  }
  return iter->second;
}

bool QuestLog::hasQuestEntry(const std::string& questID, const unsigned int index) const
{
  std::map<std::string, std::map<unsigned int, unsigned int> >::const_iterator iter;
  iter = m_PresentEntries.find(questID);
  if (iter==m_PresentEntries.end())
  {
    return false;
  }
  return (iter->second.find(index)!=iter->second.end());
}

unsigned int QuestLog::getMaximumPresentIndex(const std::string& questID) const
{
  std::map<std::string, std::map<unsigned int, unsigned int> >::const_iterator iter;
  iter = m_PresentEntries.find(questID);
  if (iter==m_PresentEntries.end())
  {
    return 0; //no entry found
  }
  std::map<unsigned int, unsigned int>::const_reverse_iterator index_iter;
  index_iter = iter->second.rbegin();
  if (index_iter == iter->second.rend())
  {
    return 0; //no entries -> should not normally happen
  }
  return index_iter->first;
}

std::vector<std::string> QuestLog::listFinishedQuests() const
{
  std::vector<std::string> sv;
  sv.clear();
  std::map<std::string, bool>::const_iterator cIter = m_FinishedQuests.begin();
  while (cIter!=m_FinishedQuests.end())
  {
    if (cIter->second)
    {
      sv.push_back(cIter->first);
    }
    ++cIter;
  }//while
  return sv;
}

std::vector<std::string> QuestLog::listActiveQuests() const
{
  std::vector<std::string> sv;
  sv.clear();
  std::map<std::string, bool>::const_iterator cIter = m_FinishedQuests.begin();
  while (cIter!=m_FinishedQuests.end())
  {
    if (!(cIter->second))
    {
      sv.push_back(cIter->first);
    }
    ++cIter;
  }//while
  return sv;
}

std::vector<QLogEntry> QuestLog::listQuestEntries(const unsigned int offset, unsigned int limit) const
{
  std::vector<QLogEntry> qVec;
  qVec.clear();
  if (offset>=m_TimeLine.size())
  {
    return qVec; //return empty vector
  }
  if (limit==0)
  {
    limit = m_TimeLine.size();
  }
  unsigned int maxIdx = offset+limit-1;
  if (maxIdx>=m_TimeLine.size())
  {
    maxIdx = m_TimeLine.size()-1;
  }
  unsigned int i;
  for (i=offset; i<=maxIdx; i=i+1)
  {
    qVec.push_back(m_TimeLine[i]);
  }//for
  return qVec;
}

void QuestLog::ClearAllData()
{
  m_PresentEntries.clear();
  m_TimeLine.clear();
  m_FinishedQuests.clear();
}

unsigned int QuestLog::NumberOfQuestEntries() const
{
  return m_TimeLine.size();
}

bool QuestLog::SaveToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "QuestLog::SaveToStream: ERROR: bad stream!\n";
    return false;
  }
  //write herader
  output.write((char*) &cHeaderQLog, sizeof(cHeaderQLog));
  //write length
  unsigned int len = 0;
  len = m_TimeLine.size();
  output.write((char*) &len, sizeof(unsigned int));
  unsigned int i;
  //write data
  for (i=0; i<m_TimeLine.size(); i=i+1)
  {
    //write questID
    len = m_TimeLine[i].questID.length();
    output.write((char*) &len, sizeof(unsigned int));
    output.write(m_TimeLine[i].questID.c_str(), len);
    //write index
    output.write((char*) &(m_TimeLine[i].index), sizeof(unsigned int));
    if (!(output.good()))
    {
      std::cout << "QuestLog::SaveToStream: ERROR while writing entries!\n";
      return false;
    }
  }//for
  return output.good();
}

bool QuestLog::LoadFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    std::cout << "QuestLog::LoadFromStream: ERROR: bad stream!\n";
    return false;
  }
  unsigned int len = 0;
  //read header
  input.read((char*) &len, sizeof(unsigned int));
  if (len!=cHeaderQLog)
  {
    std::cout << "QuestLog::LoadFromStream: ERROR: invalid record header!\n";
    return false;
  }
  unsigned int count = 0;
  input.read((char*) &count, sizeof(unsigned int));
  unsigned int i;
  char buffer[256];
  for (i=0; i<count; i=i+1)
  {
    //read string length
    len = 0;
    input.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "QuestLog::LoadFromStream: ERROR: questID seems to be longer"
                << " than 255 characters!\n";
      return false;
    }
    buffer[0] = '\0';
    input.read(buffer, len);
    buffer[len] = '\0';
    if (!(input.good()))
    {
      std::cout << "QuestLog::LoadFromStream: ERROR while reading questID!\n";
      return false;
    }
    //read index
    input.read((char*) &len, sizeof(unsigned int));
    if (!(input.good()))
    {
      std::cout << "QuestLog::LoadFromStream: ERROR while reading quest index!\n";
      return false;
    }
    //add it
    addQuestEntry(std::string(buffer), len);
  }//for
  return input.good();
}

} //namespace
