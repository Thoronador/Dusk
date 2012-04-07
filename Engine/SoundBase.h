/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012 thoronador

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
 Date:    2012-04-07
 Purpose: SoundBase Singleton class
          Represents a look-up table for all sound "objects" within the game.

 History:
     - 2012-04-07 (rev 305) - initial version (by thoronador)

 ToDo list:
     - ???

 Known bugs:
     - None. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SOUNDBASE_H
#define SOUNDBASE_H

#include <string>
#include <map>

namespace Dusk
{

//record type for SoundBase
struct SoundRecord
{
  std::string sID;
  std::string filePath;
}; //struct


/* class SoundBase
         The purpose of this class is to hold the IDs of all distinct
         sounds in the game, as well as their sound paths.

         Think of SoundBase as a sort of look-up table, e.g.:

             ID      |  path
           ----------+----------------------
           gunShot   | bang.wav
           stepSound | moves/feet.ogg
           windy     | blowingInTheWind.ogg
            ...      | ...

*/
class SoundBase
{
  public:
    /* destructor */
    ~SoundBase();

    /* singleton access method */
    static SoundBase& getSingleton();

    /* adds a sound with the given ID and data.

       remarks:
           This function always succeeds, except for empty ID or file path. In
           that case, nothing is added. If a record with the same ID already
           exists, it is overwritten by the new data.
    */
    void addSound(const std::string& ID, const std::string& filePath);

    /* returns true, if a sound with the given ID is present

       parameters:
           ID - the ID of the sound
    */
    bool hasSound(const std::string& ID) const;

    /* Returns the file path of the sound with given ID. If no sound with the
       given ID is present, the function will throw an exception.

       parameters:
           sID - ID of the sound
    */
    const std::string& getSoundPath(const std::string& sID) const;

    /* deletes a sound by ID and returns true, if a sound was deleted */
    bool deleteSound(const std::string& ID);

    /* deletes all present sounds */
    void clearAll();

    /* returns the number of present sounds in the list */
    unsigned int getNumberOfSounds() const;

    /* tries to save all sounds to the stream and returns true on success

       parameters:
           outStream - the output stream that is used to save the sound data
    */
    bool saveAllToStream(std::ofstream& outStream) const;

    /* Loads one(!) single sound record from stream; returns true on success,
       false otherwise. The data of the last loaded sound is probably
       inconsistent after that function failed, so don't rely on it in that
       case.

       parameters:
           inStream - the input stream that is used to load the sound data
    */
    bool loadNextSoundFromStream(std::ifstream& inStream);


    #ifdef DUSK_EDITOR
    //constant iterator type for iterating through the sounds
    typedef std::map<std::string, SoundRecord>::const_iterator Iterator;

    /* returns constant iterator to first element in sound list*/
    Iterator getFirst() const;

    /* returns constant iterator to end of sound list*/
    Iterator getEnd() const;
    #endif
  private:
    /* constructor - singleton */
    SoundBase();

    /* private, empty copy constructor - singleton pattern */
    SoundBase(const SoundBase& op) {}

    //internal list of sounds
    std::map<std::string, SoundRecord> m_Sounds;
};//class

} //namespace

#endif // SOUNDBASE_H
