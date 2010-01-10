/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-10
 Purpose: Dialogue Singleton class
          holds all information about dialogues with NPCs

 History:
     - 2010-01-10 (rev 151) - initial version

 ToDo list:
     - extend class for more conditions
     - extension for result scripts
        - execution of result scripts on demand
     - extension for audio files of speech
     - ???

 Bugs:
     - No known bugs, but class is currently untested. If you find one (or more)
       bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "NPC.h"
#include "DuskTypes.h"

namespace Dusk
{

class Dialogue
{
  public:
    struct ConditionRecord
    {
       std::string NPC_ID;
       //more to come
    };

    struct LineRecord
    {
       std::string Text;
       //std::string SpeechFile; //maybe used later for audio
       ConditionRecord Conditions; //conditions for showing that line
       std::vector<std::string> Choices; //IDs of options the player can choose
       //Script ResultScript; //maybe used later for the script which is
                              // executed when option was chosen/ displayed
      bool SaveToStream(std::ofstream& out) const;
      bool LoadFromStream(std::ifstream& inp);
    };

    struct Handle
    {
      std::string LineID;
      std::string Text;
      std::vector<std::string> Choices;
    };

    virtual ~Dialogue();
    static Dialogue& GetSingleton();
    void ClearData();
    Handle GetGreetingLine(const NPC* who) const;
    Handle GetDialogueLine(const std::string& LineID, const NPC* who) const;
    std::string GetText(const std::string& LineID) const;
    ConditionRecord GetCondition(const std::string& LineID) const;

    void AddGreeting(const LineRecord& lr);
    void AddLine(const std::string& LineID, const LineRecord& lr);

    bool SaveToStream(std::ofstream& output) const;
    bool LoadNextRecordFromStream(std::ifstream& input);

    unsigned int NumberOfLines() const;
  private:
    Dialogue();
    Dialogue(const Dialogue& op) {}
    bool ConditionFulfilled(const ConditionRecord& cond, const NPC* who) const;

    static const uint8 cGreetingFlag;
    static const uint8 cDialogueFlag;

    std::vector<LineRecord> m_GreetingLines;
    std::map<std::string, LineRecord> m_DialogueLines;
}; //class

} //namespace

#endif // DIALOGUE_H
