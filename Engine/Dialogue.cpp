/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "Dialogue.h"
#include "DuskConstants.h"
#include "LuaEngine.h"
#include "Messages.h"

namespace Dusk
{

const uint8 Dialogue::cGreetingFlag = 1;
const uint8 Dialogue::cDialogueFlag = 2;
const std::string Dialogue::LuaDialogueConditionFunction = "DialogueConditional";

bool Dialogue::LineRecord::saveToStream(std::ofstream& out) const
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
  // -- ItemID
  len = Conditions.ItemID.length();
  out.write((char*) &len, sizeof(unsigned int));
  out.write(Conditions.ItemID.c_str(), len);
  if (len>0)
  {
    out.write((char*) &(Conditions.ItemOp), sizeof(CompareOperation));
    out.write((char*) &(Conditions.ItemAmount), sizeof(unsigned int));
  }
  // -- ScriptedCondition
  if (Conditions.ScriptedCondition==NULL)
  {
    len = 0;
    out.write((char*) &len, sizeof(unsigned int));
  }
  else
  { //there is a script, write it out
    len = Conditions.ScriptedCondition->getStringRepresentation().length();
    out.write(Conditions.ScriptedCondition->getStringRepresentation().c_str(),
              len);
  }
  //choices
  len = Choices.size();
  out.write((char*) &len, sizeof(unsigned int));
  for (i=0; i<Choices.size(); i=i+1)
  {
    len = Choices[i].length();
    out.write((char*) &len, sizeof(unsigned int));
    out.write(Choices[i].c_str(), len);
  }
  //result script
  if (ResultScript==NULL)
  {
    len = 0;
    out.write((char*) &len, sizeof(unsigned int));
  }
  else
  {
    len = ResultScript->getStringRepresentation().length();
    out.write((char*) &len, sizeof(unsigned int));
    out.write(ResultScript->getStringRepresentation().c_str(), len);
  }
  return out.good();
}

bool Dialogue::LineRecord::loadFromStream(std::ifstream& inp)
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
    DuskLog() << "LineRecord::loadFromStream: ERROR: Text seems to be longer "
              << "than 511 characters.\n";
    return false;
  }
  char buffer[512];
  inp.read(buffer, len);
  buffer[len] = '\0'; //ensure NUL-termination
  if (!(inp.good()))
  {
    DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading Text "
              << "from stream.\n";
    return false;
  }
  Text = std::string(buffer);
  //conditions
  // -- NPC_ID
  inp.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    DuskLog() << "LineRecord::loadFromStream: ERROR: NPC_ID seems to be longer "
              << "than 511 characters.\n";
    return false;
  }
  inp.read(buffer, len);
  buffer[len] = '\0'; //ensure NUL-termination
  if (!(inp.good()))
  {
    DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading NPC_ID"
              << "from stream.\n";
    return false;
  }
  Conditions.NPC_ID = std::string(buffer);
  // -- ItemID
  inp.read((char*) &len, sizeof(unsigned int));
  if (len>511)
  {
    DuskLog() << "LineRecord::loadFromStream: ERROR: ItemID seems to be longer "
              << "than 511 characters.\n";
    return false;
  }
  inp.read(buffer, len);
  buffer[len] = '\0'; //ensure NUL-termination
  if (!(inp.good()))
  {
    DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading ItemID"
              << "from stream.\n";
    return false;
  }
  Conditions.ItemID = std::string(buffer);
  if (len>0)
  {
    inp.read((char*) &(Conditions.ItemOp), sizeof(CompareOperation));
    inp.read((char*) &(Conditions.ItemAmount), sizeof(unsigned int));
    if (!(inp.good()))
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading "
                << "ItemOp and ItemAmount from stream.\n";
      return false;
    }//if
  }
  else
  {
    Conditions.ItemOp = copGreaterEqual;
    Conditions.ItemAmount = 0;
  }
  // -- ScriptedCondition
  inp.read((char*) &len, sizeof(unsigned int));
  if (len==0)
  {
    Conditions.ScriptedCondition = NULL;
  }
  else
  {
    //length check to avoid allocation of to much memory and running out of memory
    if (len>100 /*maximum lines*/ * 80 /*characters per line*/)
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Condition Script is to "
                << "long ("<<len<<" characters). Maximum is 8000 characters.\n";
      return false;
    }
    char* scriptData = new char[len+1];
    inp.read(scriptData, len);
    scriptData[len] = '\0';
    if (!(inp.good()))
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading "
                << "Script text from stream.\n";
      delete[] scriptData;
      return false;
    }//if
    Conditions.ScriptedCondition = new Script(scriptData);
    delete[] scriptData;
  }

  //choices
  unsigned int choices_size = 0, i;
  inp.read((char*) &choices_size, sizeof(unsigned int));
  //Should do size check?
  Choices.clear();
  for (i=0; i<choices_size; i=i+1)
  {
    inp.read((char*) &len, sizeof(unsigned int));
    if (len>511)
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: ID seems to be longer "
                << "than 511 characters.\n";
      return false;
    }
    inp.read(buffer, len);
    buffer[len] = '\0'; //ensure NUL-termination
    if (!(inp.good()))
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading ID"
                << "of dialogue choices from stream.\n";
      return false;
    }
    Choices.push_back(std::string(buffer));
  }//for

  //result script
  inp.read((char*) &len, sizeof(unsigned int));
  if (len==0)
  {
    ResultScript = NULL;
  }
  else
  {
    //length check to avoid allocation of to much memory + running out of memory
    if (len>100 /*maximum lines*/ * 80 /*characters per line*/)
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Result Script is to "
                << "long ("<<len<<" characters). Maximum is 8000 characters.\n";
      return false;
    }
    char* scriptData = new char[len+1];
    scriptData[0] = '\0';
    inp.read(scriptData, len);
    scriptData[len] = '\0';
    if (!(inp.good()))
    {
      DuskLog() << "LineRecord::loadFromStream: ERROR: Error while reading "
                << "Result Script text from stream.\n";
      delete[] scriptData;
      return false;
    }//if
    ResultScript = new Script(scriptData);
    delete[] scriptData;
  }
  return inp.good();
}

Dialogue::Dialogue()
{
  //empty
}

Dialogue::~Dialogue()
{
  //empty
  clearData();
}

Dialogue& Dialogue::getSingleton()
{
  static Dialogue Instance;
  return Instance;
}

void Dialogue::clearData()
{
  m_DialogueLines.clear();
  m_GreetingLines.clear();
}

std::string Dialogue::getText(const std::string& LineID) const
{
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter!=m_DialogueLines.end())
  {
    return iter->second.Text;
  }
  DuskLog() << "Dialogue::getText: ERROR: no line with ID \""<<LineID
            << "\" found. Returning empty string.\n";
  return "";
}

Dialogue::ConditionRecord Dialogue::getCondition(const std::string& LineID) const
{
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter!=m_DialogueLines.end())
  {
    return iter->second.Conditions;
  }
  //nothing found, return empty record
  DuskLog() << "Dialogue::getCondition: ERROR: no line with ID \""<<LineID
            << "\" found. Returning empty record.\n";
  ConditionRecord cr;
  cr.NPC_ID = "";
  return cr;
}

bool Dialogue::hasDialogueLine(const std::string& LineID) const
{
  return (m_DialogueLines.find(LineID) != m_DialogueLines.end());
}

Dialogue::Handle Dialogue::getGreetingLine(const NPC* who) const
{
  unsigned int i;
  Handle temp;
  temp.Choices.clear();
  temp.Text = "";
  temp.LineID = "";

  std::map<std::string, std::vector<std::string> >::const_iterator iter;
  if (who != NULL)
  {
    iter = m_GreetingLines.find(who->getID());
    if (iter == m_GreetingLines.end())
    { //if there's no entry for that NPC, get the generic one
      iter = m_GreetingLines.find("");
    }
  }
  else
  { //get general entry
    iter = m_GreetingLines.find("");
  }

  if (iter == m_GreetingLines.end())
  { //nothing found?
    return temp;
  }

  //now check each entry for conditions, and if they are met, return this entry
  for (i=0; i<iter->second.size(); i=i+1)
  {
    if (isConditionFulfilled(getCondition(iter->second.at(i)), who))
    {
      temp.LineID = iter->second.at(i);
      break;
    }
  } //for

  if (temp.LineID == "")
  { //no matching entry found
    return temp;
  }

  //now get the dialogue line and the choices
  std::map<std::string, LineRecord>::const_iterator dial_iter;
  dial_iter = m_DialogueLines.find(temp.LineID);
  //no matching dialogue entry found; should not happen in a properly set up
  // dialogue.
  if (dial_iter == m_DialogueLines.end())
  {
    DuskLog() << "Dialogue::getGreetingLine: Hint: found no line entry for ID "
              << "\""<<temp.LineID<<"\". This should not happen here, check "
              << "your dialogue and add the line with this ID.\n";
    temp.LineID = "";
    return temp; //return empty record
  }
  //text
  temp.Text = dial_iter->second.Text;

  //now check the conditions of the choices to display and add them
  for (i=0; i< dial_iter->second.Choices.size(); i=i+1)
  {
    if (isConditionFulfilled(getCondition(dial_iter->second.Choices.at(i)), who))
    {
      temp.Choices.push_back(dial_iter->second.Choices.at(i));
    }//if
  } //for

  //we are done here, return the data
  return temp;
}

Dialogue::Handle Dialogue::getDialogueLine(const std::string& LineID, const NPC* who) const
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
      if (isConditionFulfilled(getCondition(iter->second.Choices[i]), who))
      {
        temp.Choices.push_back(iter->second.Choices.at(i));
      }
    } //for
    return temp;
  }//if
  //nothing found
  DuskLog() << "Dialogue::getDialogueLine: ERROR: no line with ID \""<<LineID
            << "\" found. Returning empty handle.\n";
  temp.Text = "";
  return temp;
}

void Dialogue::addGreeting(const std::string& NPC_ID, const std::vector<std::string>& Choices)
{
  m_GreetingLines[NPC_ID] = Choices;
}

void Dialogue::addLine(const std::string& LineID, const LineRecord& lr)
{
  if (LineID != "")
  {
    m_DialogueLines[LineID] = lr;
  }
}

bool Dialogue::processResultScript(const std::string& LineID)
{
  std::map<std::string, LineRecord>::const_iterator iter;
  iter = m_DialogueLines.find(LineID);
  if (iter==m_DialogueLines.end())
  {
    DuskLog() << "Dialogue::processResultScript: ERROR: there is no line with "
              << "ID \""<<LineID<<"\". No script code is executed.\n";
    return false;
  }
  if (iter->second.ResultScript==NULL)
  {
    //no script present, done :)
    return true;
  }
  return LuaEngine::getSingleton().runString( iter->second.ResultScript->getStringRepresentation());
}

bool Dialogue::isConditionFulfilled(const ConditionRecord& cond, const NPC* who) const
{
  //only check if ID is set. Unset ID matches every NPC.
  if (cond.NPC_ID!="")
  {
    if (who!=NULL)
    {
      if (who->getID()!=cond.NPC_ID)
      {
        return false;
      }
    }
    else
    { //no NPC given, assume no match
      return false;
    }
  }//NPC_ID

  //ItemID
  if (cond.ItemID!="")
  {
    if (who==NULL)
    {
      return false;
    }
    switch (cond.ItemOp)
    {
      case copLess: //inventory count has to be less, so check for greater/equal
                    // to return false;
           if (who->getConstInventory().getItemCount(cond.ItemID)>=cond.ItemAmount)
           {
             return false;
           }
           break;
      case copLessEqual:
           if (who->getConstInventory().getItemCount(cond.ItemID)>cond.ItemAmount)
           {
             return false;
           }
           break;
      case copEqual:
           if (who->getConstInventory().getItemCount(cond.ItemID)!=cond.ItemAmount)
           {
             return false;
           }
           break;
      case copGreaterEqual:
           if (who->getConstInventory().getItemCount(cond.ItemID)<cond.ItemAmount)
           {
             return false;
           }
           break;
      case copGreater:
           if (who->getConstInventory().getItemCount(cond.ItemID)<=cond.ItemAmount)
           {
             return false;
           }
           break;
      default:
           DuskLog() << "Dialogue::isConditionFulfilled: ERROR: invalid or "
                     << "unknown enumeration value ("<<(unsigned int)(cond.ItemOp)
                     << "encountered. Will return false.\n";
           return false;
    }//swi
  }//ItemID

  //Script
  if (cond.ScriptedCondition!=NULL)
  {
    if (!cond.ScriptedCondition->isEmpty())
    {
      LuaEngine& Lua = LuaEngine::getSingleton();
      // set function to nil to prevent call to an earlier version
      lua_pushstring(Lua, LuaDialogueConditionFunction.c_str());
      lua_pushnil(Lua);
      lua_settable(Lua, LUA_GLOBALSINDEX);
      //run the script (and thus get a new function)
      std::string errorString = "";
      if (Lua.runString(cond.ScriptedCondition->getStringRepresentation(), &errorString))
      {
        //now get the function
        lua_getglobal(Lua, LuaDialogueConditionFunction.c_str());
        if(lua_type(Lua,-1) != LUA_TFUNCTION)
        {
          lua_pop(Lua,1);
          DuskLog() << "Dialogue::isConditionFulfilled: ERROR: Lua does not "
                    << "have a function named \""<<LuaDialogueConditionFunction
                    << "\"! Condition cannot be checked, assuming failure.\n";
          return false;
        }
        //run the function - zero arguments, one result, no specific errorfunc
        const int errorCode = lua_pcall(Lua, 0, 1, 0);
        if (errorCode!=0)
        {
          switch (errorCode)
          {
            case LUA_ERRERR:
                 DuskLog() << "Dialogue::isConditionFulfilled: ERROR while "
                           << "calling the Lua error handler function.\n";
                 break;
            case LUA_ERRMEM:
                 DuskLog() << "Dialogue::isConditionFulfilled: ERROR: memory "
                           << "allocation failed while trying to run the Lua "
                           << "function.\n";
                 break;
            case LUA_ERRRUN:
                 DuskLog() << "Dialogue::isConditionFulfilled: ERROR: Lua "
                           << "runtime error during lua_pcall()!\n";
                 break;
            default: //should never happen
                 DuskLog() << "Dialogue::isConditionFulfilled: unknown ERROR "
                           << "occured during lua_pcall().\n"; break;
          }//swi
          DuskLog() << "Lua error message: " << lua_tostring(Lua, -1) << "\n";
          return false;
        }//if error occured
        //now get the result
        if (lua_type(Lua, -1) != LUA_TBOOLEAN)
        {
          //function did not return a boolean
          DuskLog() << "Dialogue::isConditionFulfilled: script function did not "
                    << "return a boolean value.\n";
          lua_pop(Lua, 1);
          return false;
        }
        const bool scriptResult = lua_toboolean(Lua, -1);
        lua_pop(Lua, 1);
        if (!scriptResult)
        {
          return false;
        }
      }
      else
      {
        DuskLog() << "Dialogue::isConditionFulfilled: ERROR while executing "
                  << "script!\nLua's error message is: "<< errorString <<"\n";
        return false;
      }
    } //if
  } //if
  //Script

  return true;
}

bool Dialogue::saveToStream(std::ofstream& output) const
{
   if (!(output.good()))
   {
     DuskLog() << "Dialogue::saveToStream: ERROR: Bad stream.\n";
     return false;
   }

   unsigned int len, count;
   //write greetings
   std::map<std::string, std::vector<std::string> >::const_iterator gr_iter;
   gr_iter = m_GreetingLines.begin();
   while (gr_iter != m_GreetingLines.end())
   {
     output.write((char*) &cHeaderDial, sizeof(unsigned int));
     output.write((char*) &cGreetingFlag, 1);
     //ID
     len = gr_iter->first.length();
     output.write((char*) &len, sizeof(unsigned int));
     output.write(gr_iter->first.c_str(), len);
     //number of choices
     len = gr_iter->second.size();
     output.write((char*) &len, sizeof(unsigned int));
     //choices
     for (count=0; count<gr_iter->second.size(); count=count+1)
     {
       len = gr_iter->second.at(count).length();
       output.write((char*) &len, sizeof(unsigned int));
       output.write(gr_iter->second.at(count).c_str(), len);
     } // for

     if (!(output.good()))
     {
       DuskLog() << "Dialogue::saveToStream: ERROR while writing greeting "
                 << "lines for \""<<gr_iter->first<<"\".\n";
       return false;
     }
     ++gr_iter;
   }//while

   //write dialogue lines
   std::map<std::string, LineRecord>::const_iterator iter = m_DialogueLines.begin();
   while (iter!=m_DialogueLines.end())
   {
     output.write((char*) &cHeaderDial, sizeof(unsigned int));
     output.write((char*) &cDialogueFlag, 1);
     len = iter->first.length();
     output.write((char*) &len, sizeof(unsigned int));
     output.write(iter->first.c_str(), len);
     if (!(iter->second.saveToStream(output)))
     {
       DuskLog() << "Dialogue::saveToStream: ERROR while writing dialogue "
                 << "line \""<<iter->first<<"\".\n";
       return false;
     }
     ++iter;
   }//while
   return output.good();
}

bool Dialogue::loadNextRecordFromStream(std::ifstream& input)
{
  if (!(input.good()))
  {
    DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: Bad stream.\n";
    return false;
  }

  unsigned int i;
  input.read((char*) &i, sizeof(unsigned int));
  if (i != cHeaderDial)
  {
    DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: Unexpected header.\n";
    return false;
  }

  LineRecord temp_lr;
  uint8 flag;
  char buffer[256];
  input.read((char*) &flag, 1);
  if (flag == cDialogueFlag)
  {
    i = 0;
    input.read((char*) &i, sizeof(unsigned int));
    if (i>255)
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: ID seems to be "
                << "longer than 255 characters.\n";
      return false;
    }
    input.read(buffer, i);
    buffer[i] = '\0';
    if (!(input.good()))
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR while reading ID "
                << "from stream.\n";
      return false;
    }
    //load it
    if (!(temp_lr.loadFromStream(input)))
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR while loading "
                << "line record.\n";
      return false;
    }
    //add data
    addLine(std::string(buffer), temp_lr);
  } //if cDialogueFlag
  else if (flag==cGreetingFlag)
  {
    //load it
    i = 0;
    input.read((char*) &i, sizeof(unsigned int));
    if (i>255)
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: NPC_ID seems to"
                << " be longer than 255 characters.\n";
      return false;
    }
    input.read(buffer, i);
    buffer[i] = '\0';
    if (!(input.good()))
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR while reading "
                << "NPC ID from stream.\n";
      return false;
    }
    std::string NPC_ID = std::string(buffer);
    //read number of choices
    unsigned int ChoiceCount = 0;
    input.read((char*) &ChoiceCount, sizeof(unsigned int));
    if (ChoiceCount>100) //unlikely there's so much -> error
    {
      DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: got more than"
                << " 100 choices for greeting list. Corrupt file?\n";
      return false;
    }
    //read choices
    unsigned int len;
    std::vector<std::string> temp_vec;
    temp_vec.clear();
    for (i=0; i<ChoiceCount; i=i+1)
    {
      //read length
      len = 0;
      input.read((char*) &len, sizeof(unsigned int));

      if (len>255)
      {
        DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: LineID seems "
                  << "to be longer than 255 characters.\n";
        return false;
      }
      input.read(buffer, len);
      buffer[len] = '\0';
      if (!(input.good()))
      {
        DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR while reading "
                  << "LineID from stream.\n";
        return false;
      }
      temp_vec.push_back(std::string(buffer));
    } //for
    addGreeting(NPC_ID, temp_vec);
  }
  else
  {
    DuskLog() << "Dialogue::loadNextRecordFromStream: ERROR: unrecognized flag"
                << "("<<flag<<") in stream.\n";
    return false;
  }
  return input.good();
}

unsigned int Dialogue::numberOfLines() const
{
  return m_GreetingLines.size() + m_DialogueLines.size();
}

} //namespace
