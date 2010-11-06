/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
