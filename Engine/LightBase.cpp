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

#include "LightBase.h"
#include "DuskConstants.h"
#include "DuskExceptions.h"
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
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getRed(const float d)
{
  LightRecord temp;
  temp.red = 1.0f;
  temp.green = temp.blue = 0.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getGreen(const float d)
{
  LightRecord temp;
  temp.red = temp.blue = 0.0f;
  temp.green = 1.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::getBlue(const float d)
{
  LightRecord temp;
  temp.red = temp.green = 0.0f;
  temp.blue = 1.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

bool operator==(const LightRecord& l, const LightRecord& r)
{
  return (l.red == r.red && l.green == r.green && l.blue == r.blue
          && l.type == r.type && l.radius == r.radius);
}

LightBase::LightBase()
{
  //constructor
  m_LightList.clear();
}

LightBase::~LightBase()
{
  //destructor
  m_LightList.clear();
}

LightBase& LightBase::getSingleton()
{
  static LightBase Instance;
  return Instance;
}

void LightBase::addLight(const std::string& ID, const float r, const float g,
         const float b, const float radius, const Ogre::Light::LightTypes _type)
{
  if (ID != "")
  {
    LightRecord rec;
    rec.red = r;
    rec.green = g;
    rec.blue = b;
    rec.radius = radius;
    rec.type = _type;
    rec.normalise();
    m_LightList[ID] = rec;
  }
}

void LightBase::addLight(const std::string& ID, const LightRecord& record)
{
  addLight(ID, record.red, record.green, record.blue, record.radius, record.type);
}

bool LightBase::hasLight(const std::string& ID) const
{
  return (m_LightList.find(ID)!=m_LightList.end());
}

const LightRecord& LightBase::getLightData(const std::string& ID) const
{
  std::map<std::string, LightRecord>::const_iterator l_iter = m_LightList.find(ID);
  if (l_iter != m_LightList.end())
  {
    return l_iter->second;
  }
  //nothing found, so we give them a black light
  DuskLog() << "LightBase::getLightData: ERROR: no light with ID \""<<ID
            << "\" found. Throwing exception instead.\n";
  throw IDNotFound("LightBase", ID);
}

bool LightBase::deleteLight(const std::string& ID)
{
  std::map<std::string, LightRecord>::iterator iter;
  iter = m_LightList.find(ID);
  if (iter == m_LightList.end())
  {
    return false;
  }
  m_LightList.erase(iter);
  return true;
}

void LightBase::clearAllData()
{
  m_LightList.clear();
}

unsigned int LightBase::getNumberOfLights() const
{
  return m_LightList.size();
}

bool LightBase::saveAllToStream(std::ofstream& out_stream) const
{
  if (!out_stream.good())
  {
    DuskLog() << "LightBase::saveAllToStream: ERROR: bad stream.\n";
    return false;
  }
  unsigned int len = 0;
  std::map<std::string, LightRecord>::const_iterator l_iter = m_LightList.begin();
  while (l_iter != m_LightList.end() && out_stream.good())
  {
    //write header Light
    out_stream.write((char*) &cHeaderLight, sizeof(unsigned int));
    //write ID
    len = l_iter->first.length();
    out_stream.write((char*) &len, sizeof(unsigned int));
    out_stream.write(l_iter->first.c_str(), len);
    //write colour data
    out_stream.write((char*) &(l_iter->second.red), sizeof(float));
    out_stream.write((char*) &(l_iter->second.green), sizeof(float));
    out_stream.write((char*) &(l_iter->second.blue), sizeof(float));
    out_stream.write((char*) &(l_iter->second.radius), sizeof(float));
    //write light type
    out_stream.write((char*) &(l_iter->second.type), sizeof(Ogre::Light::LightTypes));
    //check
    if (!out_stream.good())
    {
      DuskLog() << "LightBase::saveAllToStream: ERROR: while writing light "
                << "data. Current ID is \""<<l_iter->first<<"\".\n";
      return false;
    }
    ++l_iter;
  }//while
  return out_stream.good();
}

bool LightBase::loadRecordFromStream(std::ifstream& in_stream)
{
  unsigned int Header = 0;
  if (!in_stream.good())
  {
    DuskLog() << "LightBase::loadRecordFromStream: ERROR: stream contains errors.\n";
    return false;
  }
  static LightRecord temp;
  static char ID_Buffer[256];
  in_stream.read((char*) &Header, sizeof(unsigned int));
  if (Header != cHeaderLight)
  {
    DuskLog() << "LightBase::loadRecordFromStream: ERROR: invalid record header.\n";
    return false;
  }
  //read ID length
  Header = 0;
  in_stream.read((char*) &Header, sizeof(unsigned int));
  if (Header>255)
  {
    DuskLog() << "LightBase::loadRecordFromStream: ERROR: ID is longer than 255"
              << "characters.\n";
    return false;
  }
  //read ID
  in_stream.read(ID_Buffer, Header);
  if (!in_stream.good())
  {
    DuskLog() << "LightBase::loadRecordFromStream: ERROR while reading light's"
              << " ID from stream.\n";
    return false;
  }
  ID_Buffer[Header] = '\0'; //make sure it's NUL-terminated
  //read RGB and radius
  in_stream.read((char*) &(temp.red), sizeof(float));
  in_stream.read((char*) &(temp.green), sizeof(float));
  in_stream.read((char*) &(temp.blue), sizeof(float));
  in_stream.read((char*) &(temp.radius), sizeof(float));
  //read light type
  in_stream.read((char*) &(temp.type), sizeof(Ogre::Light::LightTypes));
  if (!in_stream.good())
  {
    DuskLog() << "LightBase::loadRecordFromStream: ERROR while reading light's"
              << " colour values from stream.\n";
    return false;
  }
  addLight(std::string(ID_Buffer), temp);
  return true;
}

#ifdef DUSK_EDITOR
LightBase::Iterator LightBase::getFirst() const
{
  return m_LightList.begin();
}

LightBase::Iterator LightBase::getEnd() const
{
  return m_LightList.end();
}
#endif

}//namespace
