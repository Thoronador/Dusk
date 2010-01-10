#include "Dialogue.h"
#include "DuskConstants.h"

namespace Dusk
{

const uint8 Dialogue::cGreetingFlag = 1;
const uint8 Dialogue::cDialogueFlag = 2;

bool Dialogue::LineRecord::SaveToStream(std::ofstream& out) const
{
  unsigned int len, i;
  //text
  len = Text.length();
  out.write((char*) &len, sizeof(unsigned int));
  out.write(Text.c_str(), len);
  //conditions
  // -- NPC_ID
  len = Conditions.NPC_ID.length();
  out.write((char*) &len, sizeof(unsigned int));
  out.write(Conditions.NPC_ID.c_str(), len);
  //choices
  len = Choices.size();
  out.write((char*) &len, sizeof(unsigned int));
  for (i=0; i<Choices.size(); i=i+1)
  {
    len = Choices[i].length();
    out.write((char*) &len, sizeof(unsigned int));
    out.write(Choices[i].c_str(), len);
  }
  return out.good();
}

bool Dialogue::LineRecord::LoadFromStream(std::ifstream& inp)
{
  if (!(inp.good()))
  {
    return false;
  }
  unsigned int len;
  //text
  inp.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    std::cout << "LineRecord::LoadFromStream: ERROR: Text seems to be longer "
              << "than 511 characters.\n";
    return false;
  }
  char buffer[512];
  inp.read(buffer, len);
  buffer[len] = '\0'; //ensure NUL-termination
  if (!(inp.good()))
  {
    std::cout << "LineRecord::LoadFromStream: ERROR: Error while reading Text "
              << "from stream.\n";
    return false;
  }
  Text = std::string(buffer);
  //conditions
  // -- NPC_ID
  inp.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    std::cout << "LineRecord::LoadFromStream: ERROR: NPC_ID seems to be longer "
              << "than 511 characters.\n";
    return false;
  }
  inp.read(buffer, len);
  buffer[len] = '\0'; //ensure NUL-termination
  if (!(inp.good()))
  {
    std::cout << "LineRecord::LoadFromStream: ERROR: Error while reading NPC_ID"
              << "from stream.\n";
    return false;
  }
  Conditions.NPC_ID = std::string(buffer);
  //choices
  unsigned int choices_size = 0, i;
  inp.read((char*) choices_size, sizeof(unsigned int));
  //Should do size check?
  Choices.clear();
  for (i=0; i<choices_size; i=i+1)
  {
    inp.read((char*) &len, sizeof(unsigned int));
    if (len>511)
    {
      std::cout << "LineRecord::LoadFromStream: ERROR: ID seems to be longer "
                << "than 511 characters.\n";
      return false;
    }
    inp.read(buffer, len);
    buffer[len] = '\0'; //ensure NUL-termination
    if (!(inp.good()))
    {
      std::cout << "LineRecord::LoadFromStream: ERROR: Error while reading ID"
                << "of dialogue choices from stream.\n";
      return false;
    }
    Choices.push_back(std::string(buffer));
  }//for
  return inp.good();
}

Dialogue::Dialogue()
{
  //empty
}

Dialogue::~Dialogue()
{
  //empty
  ClearData();
}

Dialogue& Dialogue::GetSingleton()
{
  static Dialogue Instance;
  return Instance;
}

void Dialogue::ClearData()
{
  m_DialogueLines.clear();
  m_GreetingLines.clear();
}

std::string Dialogue::GetText(const std::string& LineID) const
{
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter!=m_DialogueLines.end())
  {
    return iter->second.Text;
  }
  return "";
}

Dialogue::ConditionRecord Dialogue::GetCondition(const std::string& LineID) const
{
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter!=m_DialogueLines.end())
  {
    return iter->second.Conditions;
  }
  //nothing found, return empty record
  ConditionRecord cr;
  cr.NPC_ID = "";
  return cr;
}

Dialogue::Handle Dialogue::GetGreetingLine(const NPC* who) const
{
  unsigned int i, j;
  Handle temp;
  temp.Choices.clear();
  for (i=0; i<m_GreetingLines.size(); i=i+1)
  {
    if (ConditionFulfilled(m_GreetingLines.at(i).Conditions, who))
    {
      for (j=0; j<m_GreetingLines.at(i).Choices.size(); j=j+1)
      {
        if (ConditionFulfilled(GetCondition(m_GreetingLines[i].Choices[j]), who))
        {
          temp.Choices.push_back(m_GreetingLines.at(i).Choices.at(j));
        }
      } //for (inner, j)
      temp.LineID = "";
      temp.Text = m_GreetingLines.at(i).Text;
      return temp;
    }
  }//for i
  temp.LineID = "";
  temp.Text = "";
  return temp;
}

Dialogue::Handle Dialogue::GetDialogueLine(const std::string& LineID, const NPC* who) const
{
  unsigned int i;
  Handle temp;
  temp.Choices.clear();
  temp.LineID = LineID;
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter!=m_DialogueLines.end())
  {
    //got it
    temp.Text = iter->second.Text;
    //add choices
    for (i=0; i<iter->second.Choices.size(); i=i+1)
    {
      if (ConditionFulfilled(GetCondition(iter->second.Choices[i]), who))
      {
        temp.Choices.push_back(iter->second.Choices.at(i));
      }
    } //for
    return temp;
  }//if
  //nothing found
  temp.Text = "";
  return temp;
}

void Dialogue::AddGreeting(const LineRecord& lr)
{
  m_GreetingLines.push_back(lr);
}

void Dialogue::AddLine(const std::string& LineID, const LineRecord& lr)
{
  if (LineID != "")
  {
    m_DialogueLines[LineID] = lr;
  }
}

bool Dialogue::ConditionFulfilled(const ConditionRecord& cond, const NPC* who) const
{
  //only check if ID is set. Unset ID matches every NPC.
  if (cond.NPC_ID!="")
  {
    if (who!=NULL)
    {
      if (who->GetID()!=cond.NPC_ID)
      {
        return false;
      }
    }
    else
    { //no NPC given, assume no match
      return false;
    }
  }//NPC_ID
  return true;
}

bool Dialogue::SaveToStream(std::ofstream& output) const
{
   if (!(output.good()))
   {
     std::cout << "Dialogue::SaveToStream: ERROR: Bad stream.\n";
     return false;
   }

   unsigned int i;
   //write greetings
   for (i=0; i< m_GreetingLines.size(); i=i+1)
   {
     output.write((char*) &cHeaderDial, sizeof(unsigned int));
     output.write((char*) &cGreetingFlag, 1);
     if (!(m_GreetingLines.at(i).SaveToStream(output)))
     {
       std::cout << "Dialogue::SaveToStream: ERROR while writing greeting "
                 << "line "<<i<<".\n";
       return false;
     }
   }//for

   //write dialogue lines
   std::map<std::string, LineRecord>::const_iterator iter = m_DialogueLines.begin();
   while (iter!=m_DialogueLines.end())
   {
     output.write((char*) &cHeaderDial, sizeof(unsigned int));
     output.write((char*) &cDialogueFlag, 1);
     i = iter->first.length();
     output.write((char*) &i, sizeof(unsigned int));
     output.write(iter->first.c_str(), i);
     if (!(iter->second.SaveToStream(output)))
     {
       std::cout << "Dialogue::SaveToStream: ERROR while writing dialogue "
                 << "line \""<<iter->first<<"\".\n";
       return false;
     }
   }//while
   return output.good();
}

bool Dialogue::LoadNextRecordFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    std::cout << "Dialogue::LoadNextRecordFromStream: ERROR: Bad stream.\n";
    return false;
  }

  unsigned int i;
  input.read((char*) &i, sizeof(unsigned int));
  if (i != cHeaderDial)
  {
    std::cout << "Dialogue::LoadNextRecordFromStream: ERROR: Unexpected header.\n";
    return false;
  }

  LineRecord temp_lr;
  uint8 flag;
  input.read((char*) &flag, 1);
  if (flag == cDialogueFlag)
  {
    i = 0;
    input.read((char*) &i, sizeof(unsigned int));
    if (i>255)
    {
      std::cout << "Dialogue::LoadNextRecordFromStream: ERROR: ID seems to be "
                << "longer than 255 characters.\n";
      return false;
    }
    char buffer[256];
    input.read(buffer, i);
    buffer[i] = '\0';
    if (!(input.good()))
    {
      std::cout << "Dialogue::LoadNextRecordFromStream: ERROR while reading ID "
                << "from stream.\n";
      return false;
    }
    //load it
    if (!(temp_lr.LoadFromStream(input)))
    {
      std::cout << "Dialogue::LoadNextRecordFromStream: ERROR while loading "
                << "line record.\n";
      return false;
    }
    //add data
    AddLine(std::string(buffer), temp_lr);
  } //if cDialogueFlag
  else if (flag==cGreetingFlag)
  {
    //load it
    if (!(temp_lr.LoadFromStream(input)))
    {
      std::cout << "Dialogue::LoadNextRecordFromStream: ERROR while loading "
                << "greeting record.\n";
      return false;
    }
    AddGreeting(temp_lr);
  }
  else
  {
    std::cout << "Dialogue::LoadNextRecordFromStream: ERROR: unrecognized flag"
                << "("<<flag<<") in stream.\n";
    return false;
  }
  return input.good();
}

unsigned int Dialogue::NumberOfLines() const
{
  return m_GreetingLines.size() + m_DialogueLines.size();
}

} //namespace
