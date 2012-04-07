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

#include "SoundBase.h"
#include <cstring>
#include "DuskConstants.h"
#include "DuskExceptions.h"
#include "Messages.h"

namespace Dusk
{

SoundBase::~SoundBase()
{
  m_Sounds.clear();
}

SoundBase& SoundBase::getSingleton()
{
  static SoundBase instance;
  return instance;
}

void SoundBase::addSound(const std::string& ID, const std::string& filePath)
{
  if (ID.empty() or filePath.empty())
  {
    DuskLog() << "SoundBase::addSound: ERROR: ID or path is empty string.\n";
    return;
  }
  SoundRecord temp;
  temp.sID = ID;
  temp.filePath = filePath;
  m_Sounds[ID] = temp;
}

bool SoundBase::hasSound(const std::string& ID) const
{
  return (m_Sounds.find(ID)!=m_Sounds.end());
}

const std::string& SoundBase::getSoundPath(const std::string& sID) const
{
  const std::map<std::string, SoundRecord>::const_iterator iter = m_Sounds.find(sID);
  if (iter!=m_Sounds.end())
  {
    return iter->second.filePath;
  }
  DuskLog() << "SoundBase::getSoundPath: ERROR! No object with ID \""<<sID
            << "\" found. Throwing exception.\n";
  throw IDNotFound("SoundBase", sID);
}

bool SoundBase::deleteSound(const std::string& ID)
{
  std::map<std::string, SoundRecord>::iterator iter = m_Sounds.find(ID);
  if (iter!=m_Sounds.end())
  {
    m_Sounds.erase(iter);
    return true;
  }
  return false;
}

void SoundBase::clearAll()
{
  m_Sounds.clear();
}

unsigned int SoundBase::getNumberOfSounds() const
{
  return m_Sounds.size();
}

bool SoundBase::saveAllToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
     DuskLog() << "SoundBase::saveAllToStream: ERROR: Stream contains errors!\n";
     return false;
  }

  std::map<std::string, SoundRecord>::const_iterator iter;
  unsigned int len;
  iter = m_Sounds.begin();
  while (iter != m_Sounds.end())
  {
    //write header "Soun"
    outStream.write((char*) &cHeaderSoun, 4); //Sound
    //write ID
    len = iter->first.length();
    outStream.write((char*) &len, sizeof(unsigned int));
    outStream.write(iter->first.c_str(), len);
    //write path
    len = iter->second.filePath.length();
    outStream.write((char*) &len, sizeof(unsigned int));
    outStream.write(iter->second.filePath.c_str(), len);
    //check
    if (!outStream.good())
    {
      DuskLog() << "SoundBase::saveAllToStream: Error while writing data to "
                << "stream. Current object is \""<<iter->first<<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return outStream.good();
}

bool SoundBase::loadNextSoundFromStream(std::ifstream& inStream)
{
  unsigned int len;
  unsigned int Header = 0;
  char bufferID[256], bufferPath[256];

  //read header "Soun" (Sound)
  inStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderSoun)
  {
    DuskLog() << "SoundBase::loadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ObjectBase::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  memset(bufferID, 0, 256);
  inStream.read(bufferID, len);
  if (!(inStream.good()))
  {
    DuskLog() << "SoundBase::loadNextSoundFromStream: ERROR while reading data.\n";
    return false;
  }
  //read length of path
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "SoundBase::loadNextSoundFromStream: ERROR: File path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh
  memset(bufferPath, 0, 256);
  inStream.read(bufferPath, len);
  if (!(inStream.good()))
  {
    DuskLog() << "SoundBase::loadNextSoundFromStream: ERROR while reading data.\n";
    return false;
  }
  //now add it to the data
  addSound(std::string(bufferID), std::string(bufferPath));
  return true;
}


#ifdef DUSK_EDITOR
SoundBase::Iterator SoundBase::getFirst() const
{
  return m_Sounds.begin();
}

SoundBase::Iterator SoundBase::getEnd() const
{
  return m_Sounds.end();
}
#endif

SoundBase::SoundBase()
{
  //empty
}

} //namespace
