/*---------------------------------------------------------------------------
 Author:  ssj5000, thoronador
 Date:    2010-01-07
 Purpose: Settings Singleton class
          provides an interface to add/change and query game-wide settings.

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2009-09-24 (rev 131) - added to Dusk namespace (by thoronador)
     - 2009-12-18 (rev 142) - two constant values (cBaseEncumbrance and
                              cEncumbranceStrengthCoefficient) added
     - 2009-12-22 (rev 143) - three more constants (cHealthBase, cHealthVitalityFactor,
                              cHealthLevelFactor) added
     - 2009-12-23 (rev 145) - split into .h and .cpp
                            - introduction of Settings Singleton class, which
                              manages all settings and allows to change them or
                              read them from a file
     - 2010-01-07 (rev 150) - settings file can now have line comments
     - 2010-06-06 (rev 215) - factor for critical damage added to initial values
     - 2010-08-28 (rev 237) - internal structure changed: one map for all types
                              of settings instead one map for each type

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

      /* character that indicates comment lines*/
      static const char cCommentCharacter;
    protected:
      //internal structure to store a setting
      struct SettingRecord
      {
        SettingType s_type;
        float float_value;
        unsigned int uint_value;
        std::string str_value;
        //constructor
        SettingRecord();
        //float setting constructor
        SettingRecord(const float f);
        //integer setting constructor
        SettingRecord(const unsigned int i);
        //string setting constructor
        SettingRecord(const std::string& s);
        //copy constructor
        SettingRecord(const SettingRecord& op);
      };//struct
    private:
      /* constructor */
      Settings();

      /* copy constructor - empty due to singleton pattern */
      Settings(const Settings& op) {}

      /* Sets some initial settings */
      void InitialSettings();

      std::map<std::string, SettingRecord> m_AllSettings;
  }; //class

}//namespace

#endif
