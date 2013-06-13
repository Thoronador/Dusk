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

#include "CommandAssociateSound.h"
#include "../sound/Sound.h"

namespace Dusk
{
  // **** command for associating source with media ****
  //constructor
  CommandAssociateSoundMedia::CommandAssociateSoundMedia(const std::string& SourceID, const std::string& MediaID)
  : Command(),
    m_Source(SourceID),
    m_Media(MediaID)
  {
  }

  //destructor
  CommandAssociateSoundMedia::~CommandAssociateSoundMedia()
  {
    //empty
  }

  bool CommandAssociateSoundMedia::execute(Dusk::Scene* scene, int count)
  {
    Source & src = Sound::get().getSource(m_Source);
    Media & m = Sound::get().getMedia(m_Media);
    return src.attach(m);
  }

  // **** command for revoking associating between source and media ****
  //constructor
  CommandDeassociateSoundMedia::CommandDeassociateSoundMedia(const std::string& SourceID)
  : Command(),
    m_Source(SourceID)
  {
  }

  //destructor
  CommandDeassociateSoundMedia::~CommandDeassociateSoundMedia()
  {
    //empty
  }

  bool CommandDeassociateSoundMedia::execute(Dusk::Scene* scene, int count)
  {
    Source & src = Sound::get().getSource(m_Source);
    return src.detach();
  }
}
