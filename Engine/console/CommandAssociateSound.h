/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: CommandAssociateSoundMedia and CommandDeassociateSoundMedia classes
          Commands for associating sounds with media and detaching media from
          sounds via Console

 History:
     - 2009-01-03 (rev 67)  - initial version (by thoronador)
     - 2010-05-06 (rev 197) - documentation update
     - 2010-11-10 (rev 250) - update for corrected function names in Sound

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
             SourceID - ID of Source that shall be associated with a media
             MediaID  - ID of the media that shall be associated with the given
                        source
      */
      CommandAssociateSoundMedia(const std::string& SourceID, const std::string& MediaID);
      /* destructor */
      virtual ~CommandAssociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Source, m_Media;
  };

  class CommandDeassociateSoundMedia : public Command
  {
    public:
      /* constructor

         parameters:
             SourceID - ID of Source whose associated media shall be detached
      */
      CommandDeassociateSoundMedia(const std::string& SourceID);
      /* destructor */
      virtual ~CommandDeassociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Source;
  };
}

#endif // COMMANDASSOCIATESOUND_H
