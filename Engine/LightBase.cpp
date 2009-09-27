#include "LightBase.h"
#include "../Engine/DuskConstants.h"
#include <iostream>

namespace Dusk
{

bool LightRecord::IsBlack() const
{
  return (red==0.0f && green==0.0f && blue==0.0f);
}

void LightRecord::Normalise()
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

LightRecord LightRecord::GetBlack(const float d)
{
  LightRecord temp;
  temp.red = temp.green = temp.blue = 0.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::GetRed(const float d)
{
  LightRecord temp;
  temp.red = 1.0f;
  temp.green = temp.blue = 0.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::GetGreen(const float d)
{
  LightRecord temp;
  temp.red = temp.blue = 0.0f;
  temp.green = 1.0f;
  temp.radius = d;
  temp.type = Ogre::Light::LT_POINT;
  return temp;
}

LightRecord LightRecord::GetBlue(const float d)
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

LightBase& LightBase::GetSingleton()
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
    rec.Normalise();
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

LightRecord LightBase::getLightData(const std::string& ID) const
{
  std::map<std::string, LightRecord>::const_iterator l_iter = m_LightList.find(ID);
  if (l_iter != m_LightList.end())
  {
    return l_iter->second;
  }
  //nothing found, so we give them a black light
  return LightRecord::GetBlack(0.0f);
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

void LightBase::ClearAllData()
{
  m_LightList.clear();
}

unsigned int LightBase::NumberOfLights() const
{
  return m_LightList.size();
}

bool LightBase::SaveAllToStream(std::ofstream& out_stream) const
{
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
    l_iter++;
  }//while
  return out_stream.good();
}

bool LightBase::LoadRecordFromStream(std::ifstream& in_stream)
{
  unsigned int Header = 0;
  if (!in_stream.good())
  {
    std::cout << "LightBase::LoadRecordFromStream: ERROR: stream contains errors.\n";
    return false;
  }
  static LightRecord temp;
  static char ID_Buffer[256];
  in_stream.read((char*) &Header, sizeof(unsigned int));
  if (Header != cHeaderLight)
  {
    std::cout << "LightBase::LoadRecordFromStream: ERROR: invalid record header.\n";
    return false;
  }
  //read ID length
  Header = 0;
  in_stream.read((char*) &Header, sizeof(unsigned int));
  if (Header>255)
  {
    std::cout << "LightBase::LoadRecordFromStream: ERROR: ID is longer than 255"
              << "characters.\n";
    return false;
  }
  //read ID
  in_stream.read(ID_Buffer, Header);
  if (!in_stream.good())
  {
    std::cout << "LightBase::LoadRecordFromStream: ERROR while reading light's"
              << "ID from stream.\n";
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
    std::cout << "LightBase::LoadRecordFromStream: ERROR while reading light's"
              << "colour values from stream.\n";
    return false;
  }
  addLight(std::string(ID_Buffer), temp);
  return true;
}

std::map<std::string, LightRecord>::const_iterator LightBase::GetFirst() const
{
  return m_LightList.begin();
}

std::map<std::string, LightRecord>::const_iterator LightBase::GetEnd() const
{
  return m_LightList.end();
}

}//namespace
