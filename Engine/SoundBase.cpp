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
#include "Messages.h"

namespace Dusk
{

uint32_t SoundRecord::getRecordType() const
{
  return cHeaderSoun;
}

//record type identifier (usually the value returned by the above function)
const uint32_t SoundRecord::RecordType = cHeaderSoun;

bool SoundRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
     DuskLog() << "SoundRecord::saveToStream: ERROR: Stream contains errors!\n";
     return false;
  }

  //write header "Soun"
  outStream.write((const char*) &cHeaderSoun, 4); //Sound
  //write ID
  uint32_t len = ID.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(ID.c_str(), len);
  //write path
  len = filePath.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(filePath.c_str(), len);
  //check
  if (!outStream.good())
  {
    DuskLog() << "SoundRecord::saveToStream: Error while writing data to "
              << "stream. Current object is \""<<ID<<"\".\n";
    return false;
  }

  return outStream.good();
}

bool SoundRecord::loadFromStream(std::ifstream& inStream)
{
  uint32_t len = 0;
  //read header "Soun" (Sound)
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len!=cHeaderSoun)
  {
    DuskLog() << "SoundRecord::loadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "SoundRecord::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  char buffer[256];
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "SoundRecord::loadFromStream: ERROR while reading ID.\n";
    return false;
  }
  ID = std::string(buffer);
  //read length of path
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "SoundRecord::loadFromStream: ERROR: File path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "SoundRecord::loadFromStream: ERROR while reading path data.\n";
    return false;
  }
  filePath = std::string(buffer);

  return (!filePath.empty() and !ID.empty());
}

} //namespace
