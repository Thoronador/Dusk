/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2013  Thoronador

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

#include "Media.h"
#include <cstdlib>
#include "../Messages.h"

namespace Dusk
{

const unsigned int Media::MaxMediaSize_MB = 30;

Media::Media(const std::string& identifier, const std::string& PathToMedia)
: m_Name(identifier),
  m_FileName(PathToMedia),
  num_buffers(0),
  buffers(NULL),
  attached_to(std::vector<std::string>())
{
  //empty
}

Media::~Media()
{
  //delete buffers
  alGetError(); //clear error state
  alDeleteBuffers(num_buffers, buffers);
  const ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Media::~Media: ERROR: could not delete buffers.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    At least one buffer is still in use and can't be"
                     <<" deleted.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid buffer name. Corrupt structure?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The requested number of buffers cannot be "
                     << "deleted.\n"; break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    throw MediaDestructionException();
  }//if
  free(buffers);
  buffers = NULL;
}

const std::string& Media::getIdentifier() const
{
  return m_Name;
}

const std::string& Media::getFileName() const
{
  return m_FileName;
}

ALuint Media::getNumberOfBuffers() const
{
  return num_buffers;
}

const ALuint * Media::getBufferPointer() const
{
  return buffers;
}

ALuint * Media::getNonConstBufferPointer() const
{
  return buffers;
}

void Media::notifyAttached(const std::string& sourceID)
{
  attached_to.push_back(sourceID);
}

void Media::notifyDetached(const std::string& sourceID)
{
  int i;
  //remove entry from media
  for(i=attached_to.size()-1; i>=0; i=i-1)
  {
    if (attached_to[i]==sourceID)
    {
      attached_to[i] = attached_to.back();
      attached_to.pop_back();
    }//if
  }//for
}

const std::vector<std::string>& Media::getRelatedSources() const
{
  return attached_to;
}

const char* MediaCreationException::what() const throw()
{
  return "Media::Media: Error while creating media.\n";
}

const char* MediaDestructionException::what() const throw()
{
  return "Media::~Media: Error while deleting media.\n";
}

} //namespace
