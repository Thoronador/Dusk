/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

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

#include "LightRecord.h"
#include <cstring>
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

bool LightRecord::isBlack() const
{
  return (red==0.0f && green==0.0f && blue==0.0f);
}

void LightRecord::normalise()
{
   if (red>1.0f)
    {
      red = 1.0f;
    }
    else if (red<0.0f)
    {
      red = 0.0f;
    }
    if (green>1.0f)
    {
      green = 1.0f;
    }
    else if (green<0.0f)
    {
      green = 0.0f;
    }
    if (blue>1.0f)
    {
      blue = 1.0f;
    }
    else if (blue<0.0f)
    {
      blue = 0.0f;
    }
    if (radius<0.0f)
    {
      radius = 0.0f;
    }
}

LightRecord LightRecord::getBlack(const float d)
{
  LightRecord temp;
  temp.red = temp.green = temp.blue = 0.0f;
  temp.radius = (d>=0.0f) ? d : 0.0f;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getRed(const float d)
{
  LightRecord temp;
  temp.red = 1.0f;
  temp.green = temp.blue = 0.0f;
  temp.radius = (d>=0.0f) ? d : 0.0f;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getGreen(const float d)
{
  LightRecord temp;
  temp.red = temp.blue = 0.0f;
  temp.green = 1.0f;
  temp.radius = (d>=0.0f) ? d : 0.0f;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getBlue(const float d)
{
  LightRecord temp;
  temp.red = temp.green = 0.0f;
  temp.blue = 1.0f;
  temp.radius = (d>=0.0f) ? d : 0.0f;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

uint32_t LightRecord::getRecordType() const
{
  return cHeaderLight;
}

bool LightRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "LightRecord::saveToStream: ERROR: bad stream.\n";
    return false;
  }
  //write header Light
  outStream.write((const char*) &cHeaderLight, sizeof(unsigned int));
  //write ID
  const unsigned int len = ID.length();
  outStream.write((const char*) &len, sizeof(unsigned int));
  outStream.write(ID.c_str(), len);
  //write colour data
  outStream.write((const char*) &red, sizeof(float));
  outStream.write((const char*) &green, sizeof(float));
  outStream.write((const char*) &blue, sizeof(float));
  outStream.write((const char*) &radius, sizeof(float));
  //write light type
  outStream.write((const char*) &type, sizeof(Ogre::Light::LightTypes));
  //check
  if (!outStream.good())
  {
    DuskLog() << "LightRecord::saveToStream: ERROR: while writing light "
              << "data. Current ID is \""<<ID<<"\".\n";
    return false;
  }
  return outStream.good();
}

bool LightRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "LightRecord::loadFromStream: ERROR: stream contains errors.\n";
    return false;
  }
  char ID_Buffer[256];
  uint32_t Header = 0;
  inStream.read((char*) &Header, sizeof(uint32_t));
  if (Header != cHeaderLight)
  {
    DuskLog() << "LightRecord::loadFromStream: ERROR: invalid record header.\n";
    return false;
  }
  //read ID length
  Header = 0;
  inStream.read((char*) &Header, sizeof(uint32_t));
  if (Header>255)
  {
    DuskLog() << "LightRecord::loadFromStream: ERROR: ID is longer than 255"
              << "characters.\n";
    return false;
  }
  //read ID
  memset(ID_Buffer, 0, Header+1);
  inStream.read(ID_Buffer, Header);
  if (!inStream.good())
  {
    DuskLog() << "LightRecord::loadFromStream: ERROR while reading light's"
              << " ID from stream.\n";
    return false;
  }
  ID_Buffer[Header] = '\0'; //make sure it's NUL-terminated
  ID = std::string(ID_Buffer);
  //read RGB and radius
  inStream.read((char*) &red, sizeof(float));
  inStream.read((char*) &green, sizeof(float));
  inStream.read((char*) &blue, sizeof(float));
  inStream.read((char*) &radius, sizeof(float));
  //read light type
  inStream.read((char*) &type, sizeof(Ogre::Light::LightTypes));
  if (!inStream.good())
  {
    DuskLog() << "LightRecord::loadFromStream: ERROR while reading light's"
              << " colour values from stream.\n";
    return false;
  }
  //normalise values, just to be safe
  normalise();
  return true;
}

bool operator==(const LightRecord& l, const LightRecord& r)
{
  return (l.red == r.red && l.green == r.green && l.blue == r.blue
          && l.type == r.type && l.radius == r.radius);
}

}//namespace
