/*---------------------------------------------------------------------------
 Author:  ssj5000, thoronador
 Date:    23.12.2009
 Purpose: Settings Singleton class
          provides an interface to add/change and query game-wide settings.
 History:
     - 19.11.2007 - initial version (by ssj5000)
     - 24.09.2009 - added to Dusk namespace (by thoronador)
     - 18.12.2009 - two constant values (cBaseEncumbrance and
                    cEncumbranceStrengthCoefficient) added
     - 22.12.2009 - three more constants (cHealthBase, cHealthVitalityFactor,
                    cHealthLevelFactor) added
     - 23.12.2009 - split into .h and .cpp
                  - introduction of Settings Singleton class, which manages
                    all settings and allows to change them or read them from
                    a file

 ToDo list:
     - ???
 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>

namespace Dusk
{
  //add global settings here
  /*const unsigned int cBaseEncumbrance = 40;
  const unsigned int cEncumbranceStrengthCoefficient = 8;
  const unsigned int cHealthBase = 10;
  const unsigned int cHealthVitalityFactor = 3;
  const unsigned int cHealthLevelFactor = 2;*/

  class Settings
  {
    public:
      virtual ~Settings();

      /* Singleton access function */
      static Settings& GetSingleton();

      /* Adds a setting named setting_name with an (unsigned) integer value of uint_value

         remarks:
           If a setting with the same name already exists, the old value will be
           overwritten. The same applies for addSetting_float() and addSetting_string().
      */
      void addSetting_uint(const std::string& setting_name, const unsigned int uint_value);

      /* Adds a setting named setting_name with a floating point value of float_value */
      void addSetting_float(const std::string& setting_name, const float float_value);

      /* Adds a setting named setting_name with a string value of str_value */
      void addSetting_string(const std::string& setting_name, const std::string& str_value);

      /* enumeration type to identify the type of a setting */
      enum SettingType {stNone=0, stString=1, stFloat=2, stInt=3};

      /* Checks for the existence of a setting named setting_name and returns
         its type. If no such setting is present, the function will return
         stNone instead.
      */
      SettingType hasSetting(const std::string& setting_name) const;

      /* Retrieves the value of an integer setting named setting_name. If no
         such setting is present, the function will return zero.
       */
      unsigned int getSetting_uint(const std::string& setting_name) const;

      /* Retrieves the value of a floating point setting named setting_name.
         If no such setting is present, the function will return zero.
       */
      float getSetting_float(const std::string& setting_name) const;

      /* Retrieves the value of a String setting named setting_name. If no such
         setting is present, the function will return an empty string.
       */
      std::string getSetting_string(const std::string& setting_name) const;

      /* Tries to load setting from the file FileName. Returns true on success,
         false otherwise. */
      bool LoadFromFile(const std::string& FileName);

      /* Tries to save all settings to the file FileName. Returns true on
         success, false otherwise. */
      bool SaveToFile(const std::string& FileName) const;

      /* name of the file where Settings looks for predefined settings */
      static const std::string CharacterConfigurationFile;
    private:
      Settings();
      Settings(const Settings& op) {}

      /* Sets some initial settings */
      void InitialSettings();
      std::map<std::string, std::string> m_StringSettings;
      std::map<std::string, float> m_FloatSettings;
      std::map<std::string, unsigned int> m_uintSettings;
  }; //class

}//namespace

#endif
