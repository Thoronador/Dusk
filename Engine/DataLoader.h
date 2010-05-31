/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-08
 Purpose: DataLoader Singleton class
          central class which wraps all load/save processes for the game

 History:
     - 2009-07-13 (rev 104) - initial version, saves only (by thoronador)
     - 2009-07-15 (rev 105) - function for loading data implemented
     - 2009-07-24 (rev 111) - ClearData() added
     - 2009-09-13 (rev 128) - update for LightBase class
     - 2009-09-22 (rev 130) - update to work with extended functionality of
                              ObjectData class (lights)
     - 2009-09-27 (rev 132) - update for containers
     - 2010-01-01 (rev 148) - documentation update
     - 2010-01-10 (rev 151) - update for Dialogue
     - 2010-01-14 (rev 153) - update for NPCBase
     - 2010-01-16 (rev 154) - update for AnimationData
     - 2010-01-16 (rev 155) - bug fixed
     - 2010-01-28 (rev 160) - update for Journal
     - 2010-02-05 (rev 163) - update for QuestLog
     - 2010-02-05 (rev 164) - possibility to save game and load saved games
                              and not only the whole data
     - 2010-02-06 (rev 165) - update for Item class
     - 2010-05-08 (rev 200) - way of saving landscape updated
     - 2010-05-20 (rev 205) - update for WaypointObject
     - 2010-05-21 (rev 206) - missing switch-case for WaypointObject added
     - 2010-05-31 (rev 211) - update for Projectiles and ProjectileBase
     - 2010-05-31 (rev 212) - update for WeaponBase

 ToDo list:
     - extend class when further classes for data management are added
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>

namespace Dusk
{
  //Flags to indicate which portions to load/ save
  const unsigned int ANIMATED_BIT = 1;
  const unsigned int CONTAINER_BIT = 2;
  const unsigned int DIALOGUE_BIT = 4;
  const unsigned int ITEM_BIT = 8;
  const unsigned int JOURNAL_BIT = 16;
  const unsigned int LANDSCAPE_BIT = 32;
  const unsigned int LIGHT_BIT = 64;
  const unsigned int NPC_BIT = 128;
  const unsigned int OBJECT_BIT = 256;
  const unsigned int PROJECTILE_BIT = 512;
  const unsigned int QUEST_LOG_BIT = 1024;
  const unsigned int REFERENCE_BIT = 2048;
  const unsigned int WEAPON_BIT = 4096;

  const unsigned int ALL_BITS = ANIMATED_BIT | CONTAINER_BIT | DIALOGUE_BIT |
                                ITEM_BIT | JOURNAL_BIT | LANDSCAPE_BIT |
                                LIGHT_BIT | NPC_BIT | OBJECT_BIT | PROJECTILE_BIT
                                | QUEST_LOG_BIT | REFERENCE_BIT | WEAPON_BIT;

  const unsigned int SAVE_MEAN_BITS = ANIMATED_BIT | QUEST_LOG_BIT | REFERENCE_BIT;

/*class DataLoader:
        This class is (or will be) the main entry point for loading game data
        from files and saving data to files. It calls the individual classes for
        the different types of data encountered in a file and dispatches reading
        to them as needed. Thus, all one will need to do later is to call the
        corresponding method (LoadFromFile/ SaveToFile?) for every data file to
        load everything one needs.
*/
class DataLoader
{
  public:
    virtual ~DataLoader();

    /* Singleton retrieval method */
    static DataLoader& GetSingleton();

    /* Tries to save all data to the file FileName. The parameter bits is a
       bitmask that indicates, what should be saved. Returns true on success,
       false on failure.
    */
    bool SaveToFile(const std::string& FileName, const unsigned int bits = ALL_BITS) const;

    /* Tries to load all data from file FileName. Returns true on success, false
       on failure.
    */
    bool LoadFromFile(const std::string& FileName);

    /* Clears all loaded data, or, if bitmask bits is not set to ALL_BITS, the
       data specified through that bitmask. Only call this, if you know what you
       are doing.
    */
    void ClearData(const unsigned int bits = ALL_BITS);

    /* tries to load a saved game from file FileName and returns true on success */
    bool LoadSaveGame(const std::string& FileName);

    /* tries to save a game to the file FileName and returns true on success */
    bool SaveGame(const std::string& FileName) const;
  private:
    /* private constructor (singleton pattern) */
    DataLoader();
    /* private, empty copy constructor (due to singleton pattern) */
    DataLoader(const DataLoader& op){}

    std::vector<std::string> m_LoadedFiles;
};//class

}//namespace

#endif //DATALOADER_H
