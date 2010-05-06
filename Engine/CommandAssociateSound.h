/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: CommandAssociateSoundMedia and CommandDeassociateSoundMedia classes
          Commands for associating sounds with media and detaching media from
          sounds via Console

 History:
     - 2009-01-03 (rev 67)  - initial version (by thoronador)
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - done

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDASSOCIATESOUND_H
#define COMMANDASSOCIATESOUND_H

#include "Command.h"
#include <string>

namespace Dusk
{
  class CommandAssociateSoundMedia : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of Noise that shall be associated with a media
             MediaID - ID of the media that shall be associated with the given
                       noise
      */
      CommandAssociateSoundMedia(const std::string& NoiseID, const std::string& MediaID);
      /* destructor */
      virtual ~CommandAssociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise, m_Media;
  };

  class CommandDeassociateSoundMedia : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of Noise whose associated media shall be detached
      */
      CommandDeassociateSoundMedia(const std::string& NoiseID);
      /* destructor */
      virtual ~CommandDeassociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDASSOCIATESOUND_H
