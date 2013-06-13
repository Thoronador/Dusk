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

#include "Source.h"
#include "../Messages.h"

namespace Dusk
{

Source::Source(const std::string& identifier)
: m_Name(identifier),
  sourceID(0),
  attachedMedia(NULL)
{
  alGetError(); //clear error state
  alGenSources(1, &sourceID);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::Source: Error while creating AL source.\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided array pointer is not valid.\n";
           break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the source.\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    throw SourceCreationException();
  }
}

Source::~Source()
{
  //check for attached media
  if (attachedMedia!=NULL)
  {
    //detach media. Result does not matter, source will be deleted anyway
    detach();
  }//if
  //delete source
  alGetError(); //clear error state
  alDeleteSources(1, &sourceID);
  const ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::~Source: ERROR: could not delete source.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt structure?\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    throw SourceDestructionException();
  }//if
}

const std::string& Source::getIdentifier() const
{
  return m_Name;
}

bool Source::attach(Media& theMedia)
{
  //check for attached media
  if (attachedMedia!=NULL)
  {
    //there is already a attached media; detach it
    if (!detach())
    {
      DuskLog() << "Source::attach: ERROR: Source \""<<m_Name
                <<"\" already has a attached noise and detach() failed.\n";
      return false;
    }
  }

  alGetError();//clear error state
  //queue all of media's buffers to the source
  alSourceQueueBuffers(sourceID, theMedia.getNumberOfBuffers(), theMedia.getBufferPointer());
  ALenum error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    DuskLog() << "Source::attach: ERROR while queueing buffers to source.\n";
    switch (error_state)
    {
      case AL_INVALID_NAME:
           DuskLog() <<"    The source name or one of the buffer names is not "
                     <<"valid.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() <<"    There is no context, the source has already a static"
                     <<" buffer attached or the new buffer has not the same "
                     <<"format as the other buffers in the queue.\n"; break;
      default:
           DuskLog() <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    return false;
  }//if
  attachedMedia = &theMedia;
  theMedia.notifyAttached(m_Name);
  return true;
}

bool Source::detach()
{
  if (attachedMedia==NULL)
  {
    //no attached media present, we don't need detach here and are done :)
    return true;
  }
  //stop playback of source, in case it is playing (or paused)
  alSourceStop(sourceID);

  alGetError();//clear error state
  //actual detach
  alSourceUnqueueBuffers(sourceID, attachedMedia->getNumberOfBuffers(),
                         attachedMedia->getNonConstBufferPointer());
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::detach: ERROR: couldn't detach buffers from source \""
              << m_Name<<"\" (attached media: \""<<attachedMedia->getIdentifier()
              <<"\").\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name ("<<sourceID
                     << "). Corrupt data structure?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    At least one buffer is still being processed and "
                     << "could not be detached.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  //notify media about being detached
  attachedMedia->notifyDetached(m_Name);
  //source
  attachedMedia = NULL;
  return true;
}

bool Source::hasAttachedMedia() const
{
  return (NULL!=attachedMedia);
}

const Media& Source::getAttachedMedia() const
{
  if (NULL==attachedMedia)
  {
    DuskLog() << "Source::getAttachedMedia: ERROR: No attached media!\n";
    throw SourceHasNoMediaException();
  }
  return *attachedMedia;
}

bool Source::play()
{
  alGetError(); //clear error state
  alSourcePlay(sourceID);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::play: ERROR: could not play source.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

bool Source::pause()
{
  alGetError();//clear error state
  alSourcePause(sourceID);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::pause: could not pause source object.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

bool Source::unPause()
{
  ALenum error_state;
  ALint source_state;

  alGetError();//clear error state
  alGetSourcei(sourceID, AL_SOURCE_STATE, &source_state);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::unPause: ERROR unable to check source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should never occur, since AL_SOURCE_STATE is hardcoded
           DuskLog() << "    Specified parameter is not valid.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if

  //check state
  if (source_state == AL_PLAYING)
  {
    DuskLog() << "Source::unPause: Hint: Source \""<<m_Name
              <<"\" is already playing.\n";
  }
  else if (source_state == AL_PAUSED)
  {
    alSourcePlay(sourceID);
    error_state = alGetError();
    if (error_state != AL_NO_ERROR)
    {
      DuskLog() << "Source::unPause: ERROR: unable to play source.\n";
      switch(error_state)
      {
        case AL_INVALID_OPERATION:
             DuskLog() << "    There is no current context.\n"; break;
        case AL_INVALID_NAME:
             DuskLog() << "    Invalid source name. Data corruption?\n"; break;
        default:
             DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                       << ".\n"; break;
      }//swi
      return false;
    }//if
  }//else if AL_PAUSED
  else
  {
    DuskLog() << "Source::unPause: Hint: Source \""<<m_Name
              << "\" was not paused yet, thus we do nothing here.\n";
  }//else
  return true;
}

bool Source::stop()
{
  alGetError();//clear error state
  alSourceStop(sourceID);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::stop: could not stop source object.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

bool Source::loop(const bool doLoop)
{
  alGetError();//clear error state
  alSourcei(sourceID, AL_LOOPING, doLoop ? AL_TRUE : AL_FALSE);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::loop: ERROR: unable to set loop mode for source \""
              << m_Name << "\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_LOOPING is allowed enum type
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE: //should never occur, since AL_TRUE/AL_FALSE is valid
           DuskLog() << "    Given value is out of range.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

bool Source::setOffset(const float seconds)
{
  alGetError();//clear error state
  alSourcef(sourceID, AL_SEC_OFFSET, seconds);
  ALenum error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Source::setOffset: ERROR: Unable to set offset for \""
              << m_Name << "\" to "<<seconds<<" seconds.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //shouldn't happen, at least not with OpenAL 1.1
           DuskLog() << "    Invalid enumeration parameter. Make sure you have"
                     << " OpenAL 1.1 or higher installed.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The given offset value is out of range.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Internal data corruption?\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return false;
  }//if
  return true;
}

float Source::getOffset() const
{
  alGetError();//clear error state
  ALfloat seconds;
  alGetSourcef(sourceID, AL_SEC_OFFSET, &seconds);
  ALenum error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Source::getOffset: ERROR: Unable to get offset for \""
              << m_Name << "\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //shouldn't happen, at least not with OpenAL 1.1
           DuskLog() << "    Invalid enumeration parameter. Make sure you have"
                     << " OpenAL 1.1 or higher.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Internal data corruption?\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return -1.0;
  }//if
  return seconds;
}

bool Source::isPlaying() const
{
  ALint source_state;
  alGetError(); //clear error state
  alGetSourcei(sourceID, AL_SOURCE_STATE, &source_state);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::isPlaying: ERROR: unable to retrieve source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_SOURCE_STATE is valid enum
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return (source_state==AL_PLAYING);
}

bool Source::isLooping() const
{
  alGetError(); //clear error state
  ALint loop_state;
  alGetSourcei(sourceID, AL_LOOPING, &loop_state);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::isLooping: ERROR: unable to retrieve source state.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_LOOPING is valid enum
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return (loop_state==AL_TRUE);
}

bool Source::setVolume(const float volume)
{
  alGetError(); //clear error state
  alSourcef(sourceID, AL_GAIN, volume);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::setVolume: ERROR: Could not set volume for "
              << "source \""<<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Value out of range.\n"; break;
      case AL_INVALID_ENUM: //should never occur, because AL_GAIN is valid
           DuskLog() << "    Invalid parameter.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source("<<sourceID<<"). Corrupt "
                     << "source list?\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  if (volume >1.0f)
  {
    DuskLog() << "Source::setVolume: Warning: Some OpenAL implementations cut "
              << "volume values larger than 1.0 down to 1.0.\n";
  }//if
  return true;
}

float Source::getVolume(const bool consider_MinMax) const
{
  alGetError(); //clear error state
  ALfloat volume_info;
  alGetSourcef(sourceID, AL_GAIN, &volume_info);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::getVolume: ERROR: Could not retrieve source state "
              << "for \""<<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value pointer.\n"; break;
      case AL_INVALID_ENUM: //should never occur here, AL_GAIN is valid
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    The specified source ("<<sourceID<<") is"
                     << " not valid. Corrupt list?\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return 0.0f; //assume something
  }//if
  //no error so far
  if (!consider_MinMax)
  {
    return volume_info;
  }
  ALfloat bound_on_gain =0.0f;
  alGetSourcef(sourceID, AL_MIN_GAIN, &bound_on_gain);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::getVolume: ERROR: Could not retrieve minimum "
              << "bound on volume for source \"" <<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value pointer.\n"; break;
      case AL_INVALID_ENUM: //should never occur here
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
        they would have already occured on the last call to alGetSourcef and
        we would not even get this far. ;)*/
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return volume_info; //return the gain value, though it might be beyond
                      // the minimum value. But we don't have a choice here.
  }//if
  //correct volume value, if below minimum
  if (bound_on_gain>volume_info)
  {
    volume_info = bound_on_gain;
  }//if min_gain > actual gain
  alGetSourcef(sourceID, AL_MAX_GAIN, &bound_on_gain);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::getVolume: ERROR: Could not retrieve maximum "
              << "bound on volume for source \"" <<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value pointer.\n"; break;
      case AL_INVALID_ENUM: //should never occur here
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      /*AL_INVALID_NAME or AL_INVALID_OPERATION should not occur here, cause
        they would have already occured on the last call to alGetSourcef and
        we would not even get this far. ;)*/
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return volume_info; //return the gain value, though it might be beyond
                        // the maximum value. But we don't have a choice here.
  }//if
  //correct volume value, if above maximum
  if (bound_on_gain<volume_info)
  {
    volume_info = bound_on_gain;
  }//if max < actual gain
  return volume_info;
}

bool Source::setPosition(const float x, const float y, const float z)
{
  alGetError(); //clear error state
  alSource3f(sourceID, AL_POSITION, x, y, z);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::setPosition: ERROR: could not set position.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Data corruption?\n"; break;
      case AL_INVALID_ENUM: //should never happen, since AL_POSITION is valid
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Given values are out of range (x: "<<x<<"; y: "<<y
                     << "; z: "<<z<<").\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return false;
  }//if
  return true;
}

std::vector<float> Source::getPosition() const
{
  std::vector<float> result(3, 0.0f);
  alGetError(); //clear error state
  alGetSource3f(sourceID, AL_POSITION, &result[0], &result[1], &result[2]);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::getPosition: ERROR: could not retrieve source "
              << "state of \""<<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_ENUM: //should never happen, AL_POSITION is valid
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    At least one value pointer is invalid.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(3, 0.0f);
  }//if
  return result;
}

bool Source::setVelocity(const float x, const float y, const float z)
{
  alGetError(); //clear error state
  alSource3f(sourceID, AL_VELOCITY, x, y, z);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::setVelocity: ERROR: could not set position for "
              << "source \""<<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corruption of data?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_VELOCITY is valid enum
           DuskLog() << "    Invalid enumeration value given.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Given values are out of range (x: "<<x<<"; y: "<<y
                     << "; z: "<<z<<").\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

std::vector<float> Source::getVelocity() const
{
  std::vector<float> result(3, 0.0);

  alGetError(); //clear error state
  alGetSource3f(sourceID, AL_VELOCITY, &result[0], &result[1], &result[2]);
  ALenum error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Source::getVelocity: ERROR: could not retrieve source "
              << "velocity for \""<<m_Name<<"\".\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_NAME:
           DuskLog() << "    Invalid source name. Corrupt data?\n"; break;
      case AL_INVALID_ENUM: //should never occur, AL_VELOCITY is valid enum
           DuskLog() << "    Invalid enumeration value.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values supplied.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//switch
    return std::vector<float>(3, 0.0);
  }//if
  return result;
}

const char* SourceCreationException::what() const throw()
{
  return "Source::Source: Error while creating AL source.\n";
}

const char* SourceDestructionException::what() const throw()
{
  return "Source::~Source: Error while deleting AL source.\n";
}

const char* SourceHasNoMediaException::what() const throw()
{
  return "Source::getAttachedMedia: There is no media attached to that source.\n";
}

} //namespace
