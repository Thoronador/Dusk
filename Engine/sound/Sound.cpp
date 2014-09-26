/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2013  thoronador, walljumper, ssj5000

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

#include "Sound.h"
#include <algorithm>
#include <cmath> //needed for rotation calculations
#include <cstdio> //required for opening files for Ogg (since it's a C lib)
#include <fstream>
#include <stdexcept>
#include "../Messages.h"
#include "MediaWave.h"
#include "MediaOggVorbis.h"

namespace Dusk
{

//functions for class Sound:
//constructor
Sound::Sound()
: m_MediaList(std::map<std::string, Media*>()),
  m_SourceList(std::map<std::string, Source*>()),
  pDevice(NULL),
  pContext(NULL),
  AL_Ready(false),
  InitInProgress(false)
{ }

//destructor
Sound::~Sound()
{
  if (AL_Ready)
  {
    exit();
  }
}

Sound& Sound::get()
{
    static Sound Instance;
    return Instance;
}

//Initializes OpenAL, device and context for our application;
//returns: true, if initialization of OpenAL was successful; false otherwise
bool Sound::init(std::string PathToLib_AL, std::string PathToLib_Vorbisfile, const bool needVorbis)
{
  if (AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::init: Warning: Cannot start initialization of OpenAL ";
    DuskLog() <<" since it is already initialized or init. is in progress.\n";
    return false;
  }
  InitInProgress = true;

  //Initialization of device
  pDevice = alcOpenDevice(NULL); //opens default device
  //later: should possibly be modified to open a selected device instead of default
  if (pDevice == NULL)
  {
    DuskLog() << "Sound::init: ERROR: Could not open default device.\n";
    InitInProgress = false;
    return false;
  }
  //create context
  pContext = alcCreateContext(pDevice, NULL);
  if (pContext == NULL)
  {
    DuskLog() << "Sound::init: ERROR: ";
    ALCenum error = alcGetError(pDevice);
    switch (error) {
      case ALC_INVALID_DEVICE:
             DuskLog() << "Specified device is not valid for output.\n";
             break;
      case ALC_INVALID_VALUE:
             DuskLog() << "Additional context cannot be created for this device.\n";
             break;
      case ALC_NO_ERROR: //should not happen here
             DuskLog() << "There is (or better should be) no error here.\n";
             break;
      default:
             DuskLog() << "Unknown error while creating context. Error code: " << error << "\n";
             break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //try to set current context
  if (alcMakeContextCurrent(pContext) == ALC_FALSE)
  {
    DuskLog() << "Sound::init: ERROR: Could not set current context.\n";
    const ALCenum error = alcGetError(pDevice);
    switch (error)
    {
      case ALC_NO_ERROR:
           DuskLog() << " There should be no error. (Funny thing, isn\'t it?)\n";
           break;
      case ALC_INVALID_CONTEXT:
           DuskLog() << " The given context pointer is invalid.\n";
           break;
      default:
           DuskLog() << " Unknown error. Error code: " << error << "\n";
           break;
    }//swi
    InitInProgress = false;
    return false;
  }
  //the AL part is done here, so we can already set AL_Ready to true
  AL_Ready = true;

  DuskLog() << "Sound::init: Info: OpenAL functions loaded, device opened, "
            << "and context created successfully.\n";
  //just for curiosity/ debug reasons: get extension string
  DuskLog() << "Debug: Available AL extensions are:\n"
            << alGetString(AL_EXTENSIONS) << "\nEnd of extension list.\n"
            << "Available ALC extensions are:\n"
            << alcGetString(pDevice, ALC_EXTENSIONS) << "\nEnd of ALC extension list.\n"
            <<"alIsExtensionPresent(AL_EXT_VORBIS): "<<(int)alIsExtensionPresent("AL_EXT_VORBIS")
            << "\nEnum of AL_FORMAT_VORBIS_EXT: "<<alGetEnumValue("AL_FORMAT_VORBIS_EXT")
            << "\n";
  //the basic initialization is done here, we can return true (for now,
  //  more will be done later)
  InitInProgress = false;
  return true; //this is the result we want
}

//returns true, if deinitialization of OpenAL was successful; false otherwise
bool Sound::exit()
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::Exit: Warning: OpenAL is not initialized, thus we can "
              << "not deinitialize it.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::Exit: Warning: (De-)Initialization of OpenAL is "
              << "already in progress, thus we quit here.\n";
    return false;
  }
  InitInProgress = true;

  unsigned int i;
  std::vector<std::string> object_list;
  //try to free all AL sources
  object_list = getSourceList(false);
  for(i=0; i<object_list.size(); i++)
  {
    destroySource(object_list.at(i));
  }
  //try to free all file resources
  object_list = getMediaList(false);
  for(i=0; i<object_list.size(); i++)
  {
    //frees resources of given media
    destroyMedia(object_list.at(i));
  }


  //standard clean-up
  alcMakeContextCurrent(NULL); //NULL is valid for alcMakeContextCurrent, so we
                               //cannot get an error here
  alcDestroyContext(pContext);
  alcCloseDevice(pDevice);

  AL_Ready = false;
  InitInProgress = false;
  return true;
}

bool Sound::isInitialized() const
{
  return AL_Ready;
}

bool Sound::isMediaPresent(const std::string& mediaIdentifier) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  return m_MediaList.find(mediaIdentifier)!=m_MediaList.end();
}

bool Sound::isSourcePresent(const std::string& sourceIdentifier) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  return m_SourceList.find(sourceIdentifier)!=m_SourceList.end();
}

Media & Sound::getMedia(const std::string& mediaIdentifier) const
{
  std::map<std::string, Media*>::const_iterator iter = m_MediaList.find(mediaIdentifier);
  if (iter!=m_MediaList.end())
    return *(iter->second);
  throw std::runtime_error("Error: The requested media does not exist!");
}

Source & Sound::getSource(const std::string& sourceIdentifier) const
{
  std::map<std::string, Source*>::const_iterator iter = m_SourceList.find(sourceIdentifier);
  if (iter!=m_SourceList.end())
    return *(iter->second);
  throw std::runtime_error("Error: The requested source does not exist!");
}

//returns list of present source names as string vector.
// parameter:
//     -with_attached_media: if set to true, it also returns the names of
//           attached media for the sources, i.e. first element is source name,
//           second element is media name of that source. Third element is source
//           name of second source, fourth element is media name of 2nd source,
//           and so on. Default value is false.
std::vector<std::string> Sound::getSourceList(const bool with_attached_media) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class instance
  std::vector<std::string> result;
  std::map<std::string, Source*>::const_iterator temp = m_SourceList.begin();
  while (temp!=m_SourceList.end())
  {
    result.push_back(temp->second->getIdentifier());
    if (with_attached_media)
    {
      if (temp->second->hasAttachedMedia())
      {
        result.push_back(temp->second->getAttachedMedia().getIdentifier());
      }//if
      else
      {
        result.push_back("");
      }//else
    }//if with media
    ++temp;
  }//while
  return result;
}

//returns list of present media names as string vector.
// parameter:
//     -with_file_names: if set to true, it also returns the names of the files
//           the media was created from, i.e. first element is media name,
//           second element is file name of that media. Third element is media
//           name of second media, fourth element is file name of 2nd media, and
//           so on. Default value is false.
std::vector<std::string> Sound::getMediaList(const bool with_file_names) const
{
  //no check for AL_Ready or InitInProgress, since it should work in every state
  //of the class
  std::vector<std::string> result;
  std::map<std::string, Media*>::const_iterator temp = m_MediaList.begin();
  while(temp!=m_MediaList.end())
  {
    result.push_back(temp->second->getIdentifier());
    if (with_file_names)
    {
      result.push_back(temp->second->getFileName());
    }//if
    ++temp;
  }//while
  return result;
}

bool Sound::createSource(const std::string& identifier)
{
  if (!AL_Ready || InitInProgress)
  {
    DuskLog() << "Sound::createSource: ERROR: OpenAL is not initialized, or"
              << "(de-)initialization is in progress; thus we can not create a "
              << "noise (yet).\n";
    return false;
  }

  if (isSourcePresent(identifier))
  {
    DuskLog() << "Sound::createSource: ERROR: A source named\""<<identifier
              <<"\" already exists. Creation stopped.\n";
    return false;
  }

  Source * temp = NULL;
  try
  {
    temp = new Source(identifier);
  }
  catch(...)
  {
    return false;
  }
  m_SourceList[identifier] = temp;
  return true;
}

bool Sound::destroySource(const std::string& identifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::DestroyNoise: ERROR: OpenAL is not initialized, thus "
              << "we can not destroy a noise (yet).\n";
    return false;
  }
  //no check for InitInProgress, since this should work in every state of the
  //class instance, where AL is ready; and it's called during Exit(), so a check
  //would prevent proper exit.
  if (!isSourcePresent(identifier))
  {
    DuskLog() << "Sound::destroySource: ERROR: A source named \""<<identifier
              <<"\" does not exist, hence we cannot destroy it.\n";
    return false;
  }

  try
  {
    Source * temp = m_SourceList[identifier];
    delete temp;
    m_SourceList.erase(identifier);
    return true;
  }
  catch (...)
  {
    return false;
  }
}


//media management routines

bool Sound::createMedia(const std::string& MediaIdentifier, const std::string& PathToMedia)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::createMedia: Warning: OpenAL is not initialized, thus "
              << "we cannot load a media file yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::createMedia: Warning: (De-)Initialization of OpenAL is in "
              << "progress, thus we cannot load a media file.\n";
    return false;
  }
  if (isMediaPresent(MediaIdentifier))
  {
    DuskLog() << "Sound::createMedia: ERROR: A media named\""<<MediaIdentifier
              <<"\" already exists. Creation stopped.\n";
    return false;
  }

  //check file for extension (and so for the implied file format)
  std::string ending = PathToMedia.substr(PathToMedia.length()-4);
  std::transform(ending.begin(), ending.end(), ending.begin(), tolower);
  Media * temp = NULL;
  if (ending==".wav")
  {
    try
    {
      temp = new MediaWave(MediaIdentifier, PathToMedia);
    }
    catch (...)
    {
       return false;
    }
  }
  else if (ending==".ogg")
  {
    try
    {
      temp = new MediaOggVorbis(MediaIdentifier, PathToMedia);
    }
    catch (...)
    {
       return false;
    }
  }
  else
  {
    DuskLog() << "Sound::CreateMedia: Error: File \""<<PathToMedia<<"\" does "
              << "not seem to be a Wave or a Ogg-Vorbis file. File cannot be "
              << "loaded.\n";
    return false;
  }
  m_MediaList[MediaIdentifier] = temp;
  return true;
}

bool Sound::destroyMedia(const std::string& MediaIdentifier)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::destroyMedia: ERROR: OpenAL is not initialized, thus "
              << "we can not destroy any media (yet).\n";
    return false;
  }
  //no check for InitInProgress, since this should work in every state of the
  //class instance, where AL is ready; and it's called during Exit(), so a check
  //would prevent proper exit.
  if (!isMediaPresent(MediaIdentifier))
  {
    DuskLog() << "Sound::destroyMedia: ERROR: A media named \""<<MediaIdentifier
              <<"\" does not exist, hence we cannot destroy it.\n";
    return false;
  }

  Media * temp = m_MediaList[MediaIdentifier];
  const std::vector<std::string> & tempList = temp->getRelatedSources();
  //detach media. Result does not matter, media will be deleted anyway
  unsigned int i;
  for (i=0; i<tempList.size(); i=i+1)
  {
    m_SourceList[tempList[i]]->detach();
  }//for

  try
  {
    delete temp;
    m_MediaList.erase(MediaIdentifier);
    return true;
  }
  catch(...)
  {
    return false;
  }
}


//state retrieval

//returns speed of sound (for doppler and such stuff)
//returns 0.0 on error
float Sound::getSpeedOfSound() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetSpeedOfSound: Warning: OpenAL is not initialized "
              << "yet, hence we cannot query anything here.\n";
    return 0.0f;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetSpeedOfSound: ERROR: (De-)Initialization of OpenAL "
              << "is in progress. No state query possbile.\n";
    return 0.0f;
  }
  ALenum error_state;
  ALfloat result=0.0f;
  alGetError();//clear error state
  result = alGetFloat(AL_SPEED_OF_SOUND);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetSpeedOfSound: ERROR: Could not query state var.\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely (as in impossible) to happen
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION: //shouldn't happen, since we always have a valid
                                 // context after initialization of Sound class
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return 0.0f;
  }//if
  return result;
}

//sets speed of sound (for doppler and such stuff)
// -returns: false on error, true otherwise
bool Sound::setSpeedOfSound(const float new_value)
{
  //values equal to or lesser than zero are rated "not funny"
  if (new_value<=0.0f)
  {
    DuskLog()<<"Sound::SetSpeedOfSound: ERROR: Only positive values allowed!\n";
    return false;
  }
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetSpeedOfSound: Warning: OpenAL is not initialized "
              << "yet, hence we cannot set any values here.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetSpeedOfSound: ERROR: (De-)Initialization of OpenAL "
              << "is in progress. No state changes possbile.\n";
    return false;
  }

  ALenum error_state;
  alGetError();//clear error state
  alSpeedOfSound(new_value);
  error_state = alGetError();
  if (error_state!=AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetSpeedOfSound: ERROR: Could not set new value.\n";
    switch(error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value given ("<<new_value<<").\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}//function SetSpeedOfSound


//****listener functions****

//Sets position of the listener
bool Sound::setListenerPostion(const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetListenerPostion: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot set position here.\n";
    return false;
  }

  ALenum error_state;
  alGetError(); //clear error state
  alListener3f(AL_POSITION, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetListenerPosition: ERROR: Could not set listener's "
              << "position!\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM: //should never happen here, since param is constant
           DuskLog() << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value, possible NaN or Inf?\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//retrieves listener's position, if possible
//otherwise, return value is vector of zeroes
std::vector<float> Sound::getListenerPosition() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerPostion: Warning: OpenAL is not initialized"
              << ", thus we cannot get the listener's position yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerPostion: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot get a position here.\n";
    return std::vector<float>(3, 0.0f);
  }

  std::vector<float> result(3, 0.0f); //declare vector and initialize it with
                                      // three zeros (better than thrice push_b)
  ALenum error_state;

  alGetError(); //clear error state
  alGetListener3f(AL_POSITION, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerPosition: ERROR: Could not get listener's "
              << "position.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n";
           break;
      case AL_INVALID_ENUM: //should not happen here
           DuskLog() << "    Invalid enumeration value.\n";
           break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values.\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n";
           break;
    }//swi
    //Normally we would return "false" here, but since there is no bool, we can
    //use the content of the result vector as well.
  }//if
  return result;
}

//changes listener's position relative to current listener's postion, i.e.
// simple vector addition of current pos. and parameter vector
bool Sound::translateListenerPostion(const float delta_x, const float delta_y, const float delta_z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::ListenerTranslatePostion: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::ListenerTranslatePostion: ERROR: (De-)Initialization "
              << "of OpenAL is in progress, thus we cannot set position here.\n";
    return false;
  }

  ALenum error_state;
  ALfloat current_x, current_y, current_z; //will hold current listener pos.
  alGetError(); //clear error state
  //get current position of listener
  alGetListener3f(AL_POSITION, &current_x, &current_y, &current_z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerTranslatePosition: ERROR: Could not get the "
              << "current listener position.\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //should never happen here
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context. :(\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer values.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  //now set the new position
  alListener3f(AL_POSITION, current_x+delta_x, current_y+delta_y, current_z+delta_z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerTranslatePosition: ERROR: Could not set new "
              << "listener position. :(\n";
    switch/*reloaded*/(error_state)
    {
      case AL_INVALID_ENUM: //should not happen
           DuskLog() << "    Invalid enumeration parameter.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context, so what?\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid position value, maybe NaN or Inf?"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     <<".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}

//sets listener's velocity and returns true on success, false on error
bool Sound::setListenerVelocity(const float x, const float y, const float z)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::SetListenerVelocity: Warning: OpenAL is not "
              << "initialized, we cannot set the listener velocity yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::SetListenerVelocity: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot set a velocity here.\n";
    return false;
  }

  ALenum error_state;

  alGetError(); //clear error state
  alListener3f(AL_VELOCITY, x, y, z);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::SetListenerVelocity: ERROR: Could not set velocity!\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely to happen
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value given, possibly NaN? (Values: x: "<<x
                     << "; y: "<<y<<"; z: "<<z<<")\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true;
}//function SetListenerVelocity

/*retrieves listener's velocity,
  returns (0.0, 0.0, 0.0), if velocity could not be determined. However, a return
  value of (0.0, 0.0, 0.0) does not necessarily indicate an error, since this
  is also a legal return value for a non-moving listener.

  Note to self: Maybe we should use NaN instead of (0.0, 0.0, 0.0) in case of
                error. However, this might cause problems if retrieved values
                are not checked for possible occurence of NaN. */
std::vector<float> Sound::getListenerVelocity() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerVelocity: Warning: OpenAL is not "
              << "initialized, we cannot have a listener velocity yet.\n";
    return std::vector<float>(3, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerVelocity: ERROR: (De-)Initialization of "
              << "OpenAL is in progress, thus we cannot get a velocity here.\n";
    return std::vector<float>(3, 0.0f);
  }

  ALenum error_state;
  std::vector<float> result(3, 0.0f);

  alGetError();//clears error state
  alGetListener3f(AL_VELOCITY, &result[0], &result[1], &result[2]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerVelocity: ERROR: Could not retrieve "
              << "velocity value of listener!\n";
    switch(error_state)
    {
      case AL_INVALID_ENUM: //unlikely
           DuskLog() << "    Invalid enumeration token.\n"; break;
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value(s) given.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(3, 0.0f);
  }//if
  return result;
}//function GetListenerVelocity

//Gets the direction and the "up" vector of the Listener as a pair of 3-tuples
//returns two 3-dimensional zero vectors on failure
std::vector<float> Sound::getListenerOrientation() const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::GetListenerOrientation: Warning: OpenAL is not init"
              << "ialized, thus we cannot set the listener's position yet.\n";
    return std::vector<float>(6, 0.0f);
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::GetListenerOrientation: ERROR: (De-)Initialization "
              << "of OpenAL is in progress, thus we cannot set position here.\n";
    return std::vector<float>(6, 0.0f);
  }

  ALenum error_state;
  ALfloat orientation[6];
  std::vector<float> result;

  alGetError(); //clear error state
  alGetListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::GetListenerOrientation: ERROR: Could not get listener "
              << "orientation.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The enumeration parameter is not valid.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    The value pointer is invalid.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return std::vector<float>(6, 0.0f); //"return false"
  }//if
  //fill vector (ALenum directly converts to int)
  for (error_state=0; error_state<6; error_state++)
  {
    result.push_back(orientation[error_state]);
  }
  return result;
}

//rotate Listener orientation around x-, y- or z-axis... ONLY ONE OF THEM
// parameters: values (in radiant) indicating how far listener should rotate
bool Sound::rotateListener(const float x_axis, const float y_axis, const float z_axis)
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::ListenerRotate: Warning: OpenAL is not initialized, "
              << "thus we cannot rotate the listener position yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot rotate the listener here.\n";
    return false;
  }

  ALenum error_state;
  ALfloat orientation[6];
  ALfloat new_orientation[6];

  alGetError(); //clear error state
  alGetListenerfv(AL_ORIENTATION, &orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: Could not get listener's "
              << "orientation, thus we cannot rotate.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n";
           break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The enum parameter is invalid.\n";
           break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid pointer value.\n";
           break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n";
           break;
    }//swi
    return false;
  }//if

  //initialisation of new values with old values
  //note: error_state is of ALenum, but ALenum directly converts to int
  for (error_state=0; error_state<6; error_state++)
  {
    new_orientation[error_state] = orientation[error_state];
  }//for


  //do the rotation here
  /* Rotation matrix (2D):
            / cos x    -sin x \
     R(x) = |                 |  where x is (likely to be) in [0, 2*Pi]
            \ sin x     cos x /
  */
  float sinus, cosinus;
  if (x_axis != 0.0)
  {
    sinus = sin(x_axis);
    cosinus = cos(x_axis);
    //rotate around x axis (x-coords remain)
    new_orientation[1] = orientation[1]*cosinus - orientation[2]*sinus;//at-vector,y
    new_orientation[2] = orientation[1]*sinus + orientation[2]*cosinus;//at-vector,z
    new_orientation[4] = orientation[4]*cosinus - orientation[5]*sinus;//up-vector,y
    new_orientation[5] = orientation[4]*sinus + orientation[5]*cosinus;//up-vector,z
  }//if x_axis
  else if (y_axis != 0.0)
  {
    sinus = sin(y_axis);
    cosinus = cos(y_axis);
    //rotate around y axis (y-coords remain untouched)
    new_orientation[2] = orientation[2]*cosinus - orientation[0]*sinus;//at-vector,z
    new_orientation[0] = orientation[2]*sinus + orientation[0]*cosinus;//at-vector,x
    new_orientation[5] = orientation[5]*cosinus - orientation[3]*sinus;//up-vector,z
    new_orientation[3] = orientation[5]*sinus + orientation[3]*cosinus;//up-vector,x
  }//if y_axis
  else if (z_axis != 0.0)
  {
    sinus = sin(z_axis);
    cosinus = cos(z_axis);
    //rotate around z axis (z-coords remain untouched)
    new_orientation[0] = orientation[0]*cosinus - orientation[1]*sinus;//at_vector,x
    new_orientation[1] = orientation[0]*sinus + orientation[1]*cosinus;//at-vector,y
    new_orientation[3] = orientation[3]*cosinus - orientation[4]*sinus;//up-vector,x
    new_orientation[4] = orientation[3]*sinus + orientation[4]*cosinus;//up-vector,y
  }//if z_axis
  //set new values for at- & up-vector
  alListenerfv(AL_ORIENTATION, &new_orientation[0]);
  error_state = alGetError();
  if (error_state != AL_NO_ERROR)
  {
    DuskLog() << "Sound::ListenerRotate: ERROR: Could not set new orientation "
              << "of listener.\n";
    switch(error_state)
    {
      case AL_INVALID_OPERATION:
           DuskLog() << "    There is no current context.\n"; break;
      case AL_INVALID_ENUM:
           DuskLog() << "    Invalid enum parameter.\n"; break;
      case AL_INVALID_VALUE:
           DuskLog() << "    Invalid value pointer given.\n"; break;
      default:
           DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                     << ".\n"; break;
    }//swi
    return false;
  }//if
  return true; //seems like wie made it :)
}

bool Sound::getCurrentDeviceName(std::string& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getCurrentDeviceName: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getCurrentDeviceName: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }
  if (pDevice==NULL)
  {
    DuskLog() << "Sound::getCurrentDeviceName: ERROR: there is no current device.\n";
    return false;
  }

  alcGetError(pDevice); //clear error state
  const ALCchar * ptrName = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);
  const ALCenum error_state = alcGetError(pDevice);
  if (error_state==ALC_NO_ERROR)
  {
    result = ptrName;
    return true;
  }
  //error occured
  DuskLog() << "Sound::getCurrentDeviceName: ERROR: Could not get device name.\n";
  switch(error_state)
  {
    case ALC_INVALID_ENUM:
         DuskLog() << "    Invalid enum parameter.\n"; break;
    default:
         DuskLog() << "    Unknown error. Error code: "<<(int)error_state
                   << ".\n"; break;
  }//swi
  return false;
}

bool Sound::getDefaultDeviceName(std::string& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getDefaultDeviceName: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getDefaultDeviceName: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }

  const ALCchar * ptrName;
  //check for extension ALC_ENUMERATE_ALL_EXT, that might give different devices
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
  {
    //use extension-provided name
    ptrName = alcGetString(pDevice, alcGetEnumValue(pDevice, "ALC_DEFAULT_ALL_DEVICES_SPECIFIER"));
  }
  else
  {
    //use standard
    ptrName = alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER);
  }
  if (ptrName==NULL)
  {
    //system has no output device
    DuskLog() << "Sound::getDefaultDeviceName: ERROR: There is no audio output"
              << " device on that system.\n";
    return false;
  }
  result = ptrName;
  return true;
}

bool Sound::getAvailableDevices(std::vector<std::string>& result) const
{
  if (!AL_Ready)
  {
    DuskLog() << "Sound::getAvailableDevices: Warning: OpenAL is not initialized, "
              << "thus we cannot retrieve a device name yet.\n";
    return false;
  }
  if (InitInProgress)
  {
    DuskLog() << "Sound::getAvailableDevices: ERROR: (De-)Initialization of OpenAL"
              << " is in progress, thus we cannot retrieve a device name.\n";
    return false;
  }

  const ALCchar * ptrName;
  //check for extension ALC_ENUMERATE_ALL_EXT, that might give more devices
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
  {
    //use extension
    ptrName = alcGetString(NULL, alcGetEnumValue(pDevice, "ALC_ALL_DEVICES_SPECIFIER"));
  }
  else if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
  {
    //use standard extension
    ptrName = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
  }
  else
  {
    //no suitable extension present
    DuskLog() << "Sound::getAvailableDevices: ERROR: No known enumeration "
              << " extension is available, thus we cannot retrieve a list of "
              << "device names.\n";
    return false;
  }
  result.clear();
  unsigned int offset = 0;
  std::string last;
  do
  {
    last = &ptrName[offset];
    if (not last.empty())
    {
      result.push_back(last);
    }
    offset = offset + last.length()+1;
  } while (not last.empty());
  return true;
}

} //namespace
