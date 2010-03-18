/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-18
 Purpose: Journal Singleton class
          holds all possible journal entries the player can get during the game

 History:
     - 2010-01-28 (rev 160) - initial version (by thoronador)
     - 2010-01-30 (rev 161) - update to allow quest names (I guess players do
                              prefer meaningful names over IDs. ;-) )
                            - hasQuest(), setQuestName(), getQuestName() added
     - 2010-02-10 (rev 172) - fixed a bug in hasQuest()
     - 2010-02-18 (rev 173) - deleteQuest() and deleteEntry() added
     - 2010-02-25 (rev 175) - getMaximumAvailabeIndex() added
     - 2010-03-13 (rev 183) - FlagsToString() added to JournalRecord
     - 2010-03-18 (rev 185) - changeQuestID() added

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef JOURNAL_H
#define JOURNAL_H

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "DuskTypes.h"

namespace Dusk
{

 struct JournalRecord
{
  std::string Text; //the text of the journal entry
  uint8 Flags; //bitwise or-ed combination of flags (see below)

  /* flag to indicate that this entry finishes the quest*/
  static const uint8 FinishedFlag;

  /* flag to indicate that this entry was deleted */
  static const uint8 DeletedFlag;

  /* returns true, if DeletedFlag is set*/
  bool isDeleted() const;

  /* returns true, if FinishedFlag is set*/
  bool isFinisher() const;

  /* returns a string indicating the set flags */
  static std::string FlagsToString(const uint8 theFlags);

  /* returns a string indicating the set flags */
  std::string FlagsToString() const;
}; //struct


/* Journal Singleton class

   This class holds all data about the distinct quest entries the player can get
   during the game. Each entry will by identified by a quest ID (string) and an
   index (integer). Entries with the same quest ID belong to the same quest, the
   index will indicate the progress of that quest. It is recommended that
   entries have ascending indices, but the choice of indices is up to you, as
   long as every entry within the same quest has a different index.

   Here's an example to get a rough idea how the data could look like:

   questID | index | text                                            | finished
   --------+-------+-------------------------------------------------+---------
   tool    |   1   | Mr. XYZ asked me to bring him a new tool kit    | false
           |       |   which he needs to repair my skidoo.           |
   --------+-------+-------------------------------------------------+---------
   tool    |  20   | I've found a intact tool kit in the abandoned   | false
           |       |   factory southeast of the town.                |
   --------+-------+-------------------------------------------------+---------
   tool    |  100  | I returned the tool kit to Mr. XYZ, and he was  |  true
           |       |   able to get my skidoo working again.          |
   --------+-------+-------------------------------------------------+---------
     ...   |  ...  |   ...                                           | ...

*/

class Journal
{
  public:
    /* singleton access */
    static Journal& GetSingleton();

    /* destructor */
    virtual ~Journal();

    /* sets the name of the quest with ID JID to qName and returns true on
       success, false otherwise

      remarks:
          Quests have the name given in cUnnnameQuest, if no name was set yet.
          If setting a quest name for an already existing quest, that name will
          be overwritten (of course). However, if JID or qName is an empty
          string, the function call will return false and the name will not be
          changed.
    */
    bool setQuestName(const std::string& JID, const std::string& qName);

    /* adds a new journal entry and returns true on success

      parameters:
          JID    - ID of the quest line
          jIndex - index of the quest entry in that line
          jData  - the data to add for that entry

      remarks:
          If an entry with the given ID and index already exists, it will be
          overwritten by the new data. Thus, the function will return true in
          almost all cases, except when JID is empty string or jIndex is zero.
    */
    bool addEntry(const std::string& JID, const unsigned int jIndex,
                  const JournalRecord& jData);

    /* adds a new journal entry and returns true on success

      parameters:
          JID    - ID of the quest line
          jIndex - index of the quest entry in that line
          jText  - journal text for that entry
          jFlags - flags for that entry (defaults to 0, i.e. no flags are set)

      remarks:
          See previous version of addEntry() above this one.
    */
    bool addEntry(const std::string& JID, const unsigned int jIndex,
                  const std::string& jText, const uint8 jFlags=0);

    /* returns true, if an entry with the given quest ID and index exists */
    bool hasEntry(const std::string& JID, const unsigned int jIndex) const;

    /* returns true, if a quest with the given quest ID exists */
    bool hasQuest(const std::string& questID) const;

    /* changes the quest ID of quest oldID to newID and returns true on success

       Remarks:
         Will fail/return false, if oldID or newID is an empty string, if the
         quest oldID does not exist or if the quest newID already exists.
    */
    bool changeQuestID(const std::string& oldID, const std::string& newID);

    /* returns the text of the given entry, or an empty string if no such entry
       is present
    */
    std::string getText(const std::string& JID, const unsigned int jIndex) const;

    /* returns the flags of the given entry, or zero if no such entry is present
    */
    uint8 getFlags(const std::string& JID, const unsigned int jIndex) const;

    /* returns the quest name of quest with ID questID, or an empty string if
       no quest with that ID is present
    */
    std::string getQuestName(const std::string& questID) const;

    /* tries to delete the quest with the given quest ID and returns true, if
       such a quest was deleted. Otherwise, false is returned.
    */
    bool deleteQuest(const std::string& questID);

    /* tries to delete the entry with the given quest ID and index. Returns true, if
       such an entry was deleted. Otherwise, false is returned.
    */
    bool deleteEntry(const std::string& questID, const unsigned int jIndex);

    /* returns the number of present journal entries (for statistics only) */
    unsigned int NumberOfEntries() const;

    /* returns the number of distinct quests, i.e. the number of different IDs */
    unsigned int NumberOfDistinctQuests() const;

    /* returns a vector of all present IDs

      remarks:
          Not useful in game, but might be used later in Editor application.
    */
    std::vector<std::string> listAllQuestIDs() const;

    /* returns a vector of all present journal indices for quest jID

      remarks:
          Not useful in game, but might be used later in Editor application.
          If you want to check for the presence of a certain index, use the
          function hasEntry() instead.

      parameters:
          jID         - ID of the quest line
          listDeleted - indicates, whether entries which are marked as deleted
                        should be included in the list (true) or not (false)
    */
    std::vector<unsigned int> listAllIndicesOfQuest(const std::string& jID, const bool listDeleted=false) const;

    /* returns the highest index among the entries for quest jID, or zero if no
       quest with that ID is present
    */
    unsigned int getMaximumAvailabeIndex(const std::string& jID) const;

    /* tries to save all data to the stream and returns true on success */
    bool SaveAllToStream(std::ofstream& output) const;

    /* tries to load the next Journal entries from the stream and returns true
       on success, false on failure
    */
    bool LoadNextFromStream(std::ifstream& input);

    /* deletes ALL entries - use with caution */
    void ClearAllEntries();

    /* static member holding the predefined name every quest has as long as no
       name has been set
    */
    static const std::string cUnnamedQuest;
  private:
    /* constructor - private, because we use singleton pattern */
    Journal();

    /* empty copy constructor */
    Journal(const Journal& op) {}

    //internal structure for quest data
    struct QuestRecord
    {
      std::string QuestName;
      std::map<const unsigned int, JournalRecord> Indices;
    }; //struct

    /* map holding the journal entries */
    std::map<const std::string, QuestRecord> m_Entries;
    unsigned int m_TotalEntries;
}; //class

} //namespace

#endif // JOURNAL_H
