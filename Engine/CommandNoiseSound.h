/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: CommandCreateNoise and CommandDestroyNoise classes
          Commands for creating and destroying sounds via Console

 History:
     - 2009-01-03 (rev 67)  - initial version (by thoronador)
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDNOISESOUND_H
#define COMMANDNOISESOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandCreateNoise : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of the new noise that shall be created
      */
      CommandCreateNoise(const std::string& NoiseID);
      /* destructor */
      virtual ~CommandCreateNoise();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };

  class CommandDestroyNoise : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of the new noise that shall be deleted
      */
      CommandDestroyNoise(const std::string& NoiseID);
      /* destructor */
      virtual ~CommandDestroyNoise();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDNOISESOUND_H
