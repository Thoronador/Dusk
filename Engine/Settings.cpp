#include "Settings.h"
#include "DuskFunctions.h"
#include <fstream>
#include <iostream>

namespace Dusk
{
  const std::string Settings::CharacterConfigurationFile = "settings.conf";
  const char Settings::cCommentCharacter = '#';

Settings::SettingRecord::SettingRecord()
{
  float_value = 0.0f;
  str_value = "";
  uint_value = 0;
  s_type = stNone;
}

Settings::SettingRecord::SettingRecord(const float f)
{
  float_value = f;
  str_value = "";
  uint_value = 0;
  s_type = stFloat;
}

Settings::SettingRecord::SettingRecord(const unsigned int i)
{
  float_value = 0.0f;
  str_value = "";
  uint_value = i;
  s_type = stInt;
}

Settings::SettingRecord::SettingRecord(const std::string& s)
{
  float_value = 0.0f;
  str_value = s;
  uint_value = 0;
  s_type = stString;
}

Settings::SettingRecord::SettingRecord(const SettingRecord& op)
{
  float_value = op.float_value;
  str_value = op.str_value;
  uint_value = op.uint_value;
  s_type = op.s_type;
}

Settings::Settings()
{
  m_AllSettings.clear();
  initialSettings();
  std::cout << "Dusk::Settings: Loading configuration file... ";
  if (FileExists(CharacterConfigurationFile))
  {
    if (loadFromFile(CharacterConfigurationFile))
    {
      std::cout << "done.\n";
    }
    else
    {
      std::cout << "failed.\n";
    }
  }
  else
  {
    std::cout << "skipping (file not found).\n";
  }
}

void Settings::initialSettings()
{
  addSetting_uint("BaseEncumbrance", 40);
  addSetting_uint("EncumbranceStrengthCoefficient", 8);
  addSetting_uint("HealthBase", 10);
  addSetting_uint("HealthVitalityFactor", 3);
  addSetting_uint("HealthLevelFactor", 2);
  addSetting_uint("CriticalDamageFactor", 2);
}

Settings::~Settings()
{
  //empty
}

Settings& Settings::getSingleton()
{
   static Settings Instance;
   return Instance;
}

void Settings::addSetting_string(const std::string& setting_name, const std::string& str_value)
{
  m_AllSettings[setting_name] = SettingRecord(str_value);
}

void Settings::addSetting_float(const std::string& setting_name, const float float_value)
{
  m_AllSettings[setting_name] = SettingRecord(float_value);
}

void Settings::addSetting_uint(const std::string& setting_name, const unsigned int uint_value)
{
  m_AllSettings[setting_name] = SettingRecord(uint_value);
}

Settings::SettingType Settings::hasSetting(const std::string& setting_name) const
{
  const std::map<std::string, SettingRecord>::const_iterator iter = m_AllSettings.find(setting_name);
  if (iter != m_AllSettings.end())
  {
    return iter->second.s_type;
  }
  return stNone;
}

unsigned int Settings::getSetting_uint(const std::string& setting_name) const
{
  const std::map<std::string, SettingRecord>::const_iterator iter
           = m_AllSettings.find(setting_name);
  if (iter != m_AllSettings.end())
  {
    if (iter->second.s_type==stInt)
    {
      return iter->second.uint_value;
    }//if type matches
  }
  //nothing found
  return 0;
}

float Settings::getSetting_float(const std::string& setting_name) const
{
  const std::map<std::string, SettingRecord>::const_iterator iter =
     m_AllSettings.find(setting_name);
  if (iter != m_AllSettings.end())
  {
    if (iter->second.s_type==stFloat)
    {
      return iter->second.float_value;
    }//if type matches
  }
  //nothing found
  return 0.0f;
}

std::string Settings::getSetting_string(const std::string& setting_name) const
{
  const std::map<std::string, SettingRecord>::const_iterator iter =
          m_AllSettings.find(setting_name);
  if (iter != m_AllSettings.end())
  {
    if (iter->second.s_type==stString)
    {
      return iter->second.str_value;
    }//it type matches
  }
  //nothing found
  return "";
}

bool Settings::loadFromFile(const std::string& FileName)
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
    else if (line.length()>0) //skip empty lines
    {
      //skip comments
      if (line.at(0)!=cCommentCharacter)
      {
        //got data for new setting
        sep_pos = line.find('=');
        if (sep_pos == std::string::npos || sep_pos == 0)
        {
          std::cout << "Settings::LoadFromFile: ERROR: Invalid line found: \""
                    << line <<"\".\nGeneral format: \"Name of Setting=value\"\n"
                    <<"Loading from file cancelled.\n";
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
      }
    } //else
  }//while
  input.close();
  return true;
}

bool Settings::saveToFile(const std::string& FileName) const
{
  std::ofstream output;
  output.open(FileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
  if (!output)
  {
    return false;
  }
  const char cc = cCommentCharacter;
  output << cc <<" Dusk configuration file\n"
         << cc <<"\n"
         << cc <<" This file holds some game settings which are read at the\n"
         << cc <<" start of the game. If you don't know what a setting means,\n"
         << cc <<" then leave it as it is, because changing these values can\n"
         << cc <<" have severe impact in-game.\n"
         << cc <<" The format for settings is \"NameOfSetting=value\".\n"
         << cc <<" Settings following [uint] have to be positive integers,\n"
         << cc <<" settings after [float] have to be floating point values,\n"
         << cc <<" and settings following [string] can have any value.\n"
         << cc <<" However, a new-line character starts a new setting and the\n"
         << cc <<" maximum length of a line is limited to 255 characters.\n"
         << cc <<" Lines starting with "<<cc<<" are comments.\n\n";

  SettingType lastType = stNone;
  std::map<std::string, SettingRecord>::const_iterator iter;
  iter = m_AllSettings.begin();
  while (iter != m_AllSettings.end())
  {
    if (iter->second.s_type!=stNone)
    {
      if (iter->second.s_type!=lastType)
      {
        switch (iter->second.s_type)
        {
          case stInt:
               output << "[uint]\n"; break;
          case stFloat:
               output << "[float]\n"; break;
          case stString:
               output << "[string]\n"; break;
          default:
               //unexpected type
               output.close();
               return false;
        }//swi
        lastType = iter->second.s_type;
      }//if type changed
      output<<iter->first<<"=";
      switch (iter->second.s_type)
      {
        case stInt:
             output <<iter->second.uint_value; break;
        case stFloat:
             output <<iter->second.float_value; break;
        case stString:
             output <<iter->second.str_value; break;
        default:
             //unexpected type
             output.close();
             return false;
      }//swi
      output<<"\n";
    }//if type != stNone
    ++iter;
  }//while
  output.close();
  return output.good();
}

} //namespace
