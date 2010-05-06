/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-01-03
 Purpose: CommandCreateMedia and CommandDestroyMedia classes
          Commands for creating and destroying sound media via Console

 History:
     - 2009-01-03 (rev 67)  - initial version (by thoronador)
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDMEDIASOUND_H
#define COMMANDMEDIASOUND_H

#include "Command.h"
#include <string>

namespace Dusk
{
  class CommandCreateMedia : public Command
  {
    public:
      /* constructor

         parameters:
             MediaID   - ID of new media which shall be created
             MediaPath - file path of media file (Wave or Ogg-Vorbis) which
                         will be associated with that media ID
      */
      CommandCreateMedia(const std::string& MediaID, const std::string& MediaPath);
      /* destructor */
      virtual ~CommandCreateMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    protected:
      std::string m_Media, m_MediaPath;
  };

  class CommandDestroyMedia : public Command
  {
    public:
      /* constructor

         parameters:
             MediaID - ID of media which shall be destroyed
      */
      CommandDestroyMedia(const std::string& MediaID);
      /* destructor */
      virtual ~CommandDestroyMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    protected:
      std::string m_Media;
  };
}

#endif // COMMANDMEDIASOUND_H
