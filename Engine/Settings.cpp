#include "Settings.h"
#include "DuskFunctions.h"
#include <fstream>
#include <iostream>

namespace Dusk
{
  const std::string Settings::CharacterConfigurationFile = "character_rules.conf";

Settings::Settings()
{
  m_StringSettings.clear();
  m_FloatSettings.clear();
  m_uintSettings.clear();
  InitialSettings();
  if (FileExists(CharacterConfigurationFile))
  {
    LoadFromFile(CharacterConfigurationFile);
  }
}

void Settings::InitialSettings()
{
  addSetting_uint("BaseEncumbrance", 40);
  addSetting_uint("EncumbranceStrengthCoefficient", 8);
  addSetting_uint("HealthBase", 10);
  addSetting_uint("HealthVitalityFactor", 3);
  addSetting_uint("HealthLevelFactor", 2);
}

Settings::~Settings()
{
  //empty
}

Settings& Settings::GetSingleton()
{
   static Settings Instance;
   return Instance;
}

void Settings::addSetting_string(const std::string& setting_name, const std::string& str_value)
{
  m_StringSettings[setting_name] = str_value;
}

void Settings::addSetting_float(const std::string& setting_name, const float float_value)
{
  m_FloatSettings[setting_name] = float_value;
}

void Settings::addSetting_uint(const std::string& setting_name, const unsigned int uint_value)
{
  m_uintSettings[setting_name] = uint_value;
}

Settings::SettingType Settings::hasSetting(const std::string& setting_name) const
{
  if (m_uintSettings.find(setting_name) != m_uintSettings.end())
  {
    return stInt;
  }
  if (m_FloatSettings.find(setting_name) != m_FloatSettings.end())
  {
    return stFloat;
  }
  if (m_StringSettings.find(setting_name) != m_StringSettings.end())
  {
    return stString;
  }
  return stNone;
}

unsigned int Settings::getSetting_uint(const std::string& setting_name) const
{
  std::map<std::string, unsigned int>::const_iterator iter;
  iter = m_uintSettings.find(setting_name);
  if (iter != m_uintSettings.end())
  {
    return iter->second;
  }
  //nothing found
  return 0;
}

float Settings::getSetting_float(const std::string& setting_name) const
{
  std::map<std::string, float>::const_iterator iter;
  iter = m_FloatSettings.find(setting_name);
  if (iter != m_FloatSettings.end())
  {
    return iter->second;
  }
  //nothing found
  return 0.0f;
}

std::string Settings::getSetting_string(const std::string& setting_name) const
{
  std::map<std::string, std::string>::const_iterator iter;
  iter = m_StringSettings.find(setting_name);
  if (iter != m_StringSettings.end())
  {
    return iter->second;
  }
  //nothing found
  return "";
}

bool Settings::LoadFromFile(const std::string& FileName)
{
  std::ifstream input;
  input.open(FileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    return false;
  }
  SettingType input_type = stNone;
  const unsigned int cMaxLine = 256;
  char buffer[cMaxLine];
  std::string line = "";
  size_t sep_pos = 0;
  while (input.getline(buffer, cMaxLine-1))
  {
    buffer[255] = '\0';
    line = std::string(buffer);
    if (line == "[uint]")
    {
      input_type = stInt;
    }
    else if (line == "[float]")
    {
      input_type = stFloat;
    }
    else if (line == "[string]")
    {
      input_type = stString;
    }
    else
    {
      //got data for new setting
      sep_pos = line.find('=');
      if (sep_pos == std::string::npos || sep_pos == 0)
      {
        std::cout << "Settings::LoadFromFile: ERROR: Invalid line found: \""
                  << line <<"\".\nGeneral format: \"Name of Setting=value\"\n";
        input.close();
        return false;
      }
      switch (input_type)
      {
        case stInt:
             addSetting_uint(line.substr(0, sep_pos), StringToInt(line.substr(sep_pos+1), 0));
             break;
        case stFloat:
             addSetting_float(line.substr(0, sep_pos), StringToFloat(line.substr(sep_pos+1), 0.0f));
             break;
        case stString:
             addSetting_string(line.substr(0, sep_pos), line.substr(sep_pos+1));
             break;
        default:
             std::cout << "Settings::LoadFromFile: ERROR: No input type given.\n";
             input.close();
             return false;
             break;
      } //swi
    } //else
  }//while
  input.close();
  return true;
}

bool Settings::SaveToFile(const std::string& FileName) const
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    return false;
  }
  output << "[uint]\n";
  std::map<std::string, unsigned int>::const_iterator iter_i;
  iter_i = m_uintSettings.begin();
  while (iter_i != m_uintSettings.end())
  {
    output<<iter_i->first<<"="<<iter_i->second<<"\n";
    iter_i++;
  }
  output << "[float]\n";
  std::map<std::string, float>::const_iterator iter_f;
  iter_f = m_FloatSettings.begin();
  while (iter_f != m_FloatSettings.end())
  {
    output<<iter_f->first<<"="<<iter_f->second<<"\n";
    iter_f++;
  }
  output << "[string]\n";
  std::map<std::string, std::string>::const_iterator iter_s;
  iter_s = m_StringSettings.begin();
  while (iter_s != m_StringSettings.end())
  {
    output<<iter_s->first<<"="<<iter_s->second<<"\n";
    iter_s++;
  }
  output.close();
  return output.good();
}

} //namespace
