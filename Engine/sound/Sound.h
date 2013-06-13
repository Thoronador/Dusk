/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2011, 2013 thoronador, walljumper, ssj5000

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
 Author:  thoronador, walljumper, ssj5000
 Date:    2010-01-07
 Purpose: Sound Singleton class
          provides the entire sound subsystem for the game

 History:
     - 2008-01-06 (rev 19)  - initial version (by thoronador)
     - 2008-01-15 (rev 21)  - Init() and Exit() added (not working at those days)
     - 2008-02-10 (rev 35)  - basic functionality of Init() and Exit() implemented
     - 2008-02-16 (rev 36)  - Init() and Exit() extended; AllFuncPointersToNULL()
     - 2008-02-21 (rev 37)  - PlayWAV() implemented; IsPlaying() added
     - 2008-02-22 (rev 38)  - Sound is now singleton (by walljumper)
     - 2008-02-23 (rev 40)  - getInstance() renamed to get(), bug fixed
     - 2008-02-24 (rev 43)  - Pause(), UnPause(), Stop() added (by thoronador)
                            - freeing buffers (incompletely?) at exit
     - 2008-02-25 (rev 44)  - Replay() added; smaller fixes
     - 2008-02-26 (rev 46)  - GetBufferedFiles() added
     - 2008-02-27 (rev 47)  - new (second) parameter for Init(); Play() improved
     - 2008-02-28 (rev 48)  - fixed wrong function name in Init(), occured under
                              Linux only (by ssj5000)
     - 2008-02-29 (rev 49)  - OggVorbis function pointers added; PlayWAV() now
                              checks for unsupported formats (by thoronador)
     - 2008-03-03 (rev 50)  - first try to play OggVorbis files
     - 2008-03-25 (rev 54)  - error checks in FreeFileResources()
     - 2008-03-26 (rev 56)  - PlayOgg() extended; memory leak in PlayWAV() fixed
     - 2008-03-28 (rev 59)  - functions for sound volume (Get-/SetVolume()) and
                              loop playback (LoopSound(), IsLooping()) added
     - 2008-03-28 (rev 60)  - return value of SetVolume() fixed
     - 2008-04-18 (rev 62)  - functions for positions of listener and sources
     - 2008-05-19 (rev 63)  - functions to get and set speed of sound added;
                              functions to get and set speed of sound sources
     - 2009-01-02 (rev 64)  - } steps to make Sound class more flexible,
     - 2009-01-02 (rev 65)  - } transition from file-based model to noise- and
     - 2009-01-02 (rev 66)  - } media-based model of Sound class
     - 2009-01-20 (rev 72)  - support for OggVorbis file implemented, it's
                              finally working
     - 2009-01-23 (rev 74)  - limit for media size introduced
     - 2009-01-30 (rev 78)  - methods to set and get noise offset
     - 2009-03-13 (rev 90)  - return value of Detach() fixed
     - 2009-03-14 (rev 91)  - format for OggVorbis files with one channel fixed
     - 2009-05-27 (rev 96)  - misspelled function name fixed
     - 2009-09-24 (rev 131) - function declarations improved
     - 2010-01-07 (rev 150) - documentation update
                            - IsInitialized() and HasVorbis() added
     - 2010-11-10 (rev 250) - naming guidelines enforced
     - 2010-12-01 (rev 265) - Sound is now in namespace Dusk
                            - usage of DuskLog/Messages class
     - 2010-12-03 (rev 266) - unneccessary #include removed
     - 2011-08-05 (rev 295) - functions to retrieve device names added
     - 2011-08-05 (rev 296) - function to retrieve default device name added
     - 2011-08-13 (rev 297) - minor fix in getAvailableDevices() function
     - 2013-05-30           - adjustment of some types for non-32bit-architectures

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SOUND_H
#define SOUND_H

#include <map>
#include <string>
#include <vector>
#if defined(_WIN32)
  //#include <windows.h>
  #include "openal/al.h" //OpenAL header
  #include "openal/alc.h"  //OpenAL header
  #include "oggvorbis/vorbisfile.h" //Vorbis header
#elif defined(__linux__) || defined(linux)
  //#include <dlfcn.h>
  #include <AL/al.h> //OpenAL header
  #include <AL/alc.h> //OpenAL header
  #include <vorbis/vorbisfile.h> //Vorbis header
#else
  #error "Unknown operating system!"
#endif

#include "Source.h"
#include "Media.h"

namespace Dusk
{

//Klasse Sound
class Sound
{
  public:
    virtual ~Sound();
    //(de-)initialization routines
    /* Initialises OpenAL, device and context for our application, and tries to
       initialise OggVorbis as well. Returns true on success, false otherwise.

      parameters:
          PathToLib_AL - path to libopenal.so/OpenAL32.dll
                         if empty string or string literal "NULL" is given, a
                         predefined, platform-dependent value will be used.
          PathToLib_Vorbisfile - path to libvorbisfile.so/vorbisfile.dll
                         if empty string or string literal "NULL" is given, a
                         predefined, platform-dependent value will be used.
      remarks:
          You need to call this function once before you can use any other
          functionality of this class.
    */
    bool init(std::string PathToLib_AL="NULL", std::string PathToLib_Vorbisfile="NULL", const bool needVorbis=false);//initializes OpenAL

    /* De-initialises the Sund class and returns true on success. */
    bool exit();//deinitializes OpenAL

    /* Returns true, if Sound is initialised. */
    bool isInitialized() const;

    // **presence checks**
    /* Returns true, if a Media named mediaIdentifier is present */
    bool isMediaPresent(const std::string& mediaIdentifier) const;

    /* Returns true, if a Source named sourceIdentifier is present */
    bool isSourcePresent(const std::string& sourceIdentifier) const;

    /* returns reference to the requested media, if present.
       If the media is not present, the function will throw an exception.
    */
    Media & getMedia(const std::string& mediaIdentifier) const;

    /* returns reference to the requested source, if present.
       If the source is not present, the function will throw an exception.
    */
    Source & getSource(const std::string& sourceIdentifier) const;

    /* Returns the list of all currently present sources.

       parameters:
           with_attached_media - if set to true, the list also contains the
                                 names of the media attached to each source. (If
                                 no media is attached, this is an empty string.)
    */
    std::vector<std::string> getSourceList(const bool with_attached_media = false) const;

    /* Returns the list of all currently loaded media.

      parameters:
          with_file_names - if set to true, the list also contains the names of
                            the files the media were loaded from.
    */
    std::vector<std::string> getMediaList(const bool with_file_names = false) const;

    // **noise management routines**
    /* Creates a new Source named identifier and returns it on success */
    bool createSource(const std::string& identifier);

    /* Destroys the Source named identifier and returns true on success */
    bool destroySource(const std::string& identifier);


    // **media management routines**
    /* Tries to create a media named MediaIdentifier from the file at
       PathToMedia and returns true on success.

      remarks:
          Only Wave and OggVorbis files are currently accepted as media files.
    */
    bool createMedia(const std::string& MediaIdentifier, const std::string& PathToMedia);

    /* Tries to destroy the media named MediaIdentifier and returns true on
       success, false on failure.
    */
    bool destroyMedia(const std::string& MediaIdentifier);

    // **general state query/set functions**
    /* Returns the speed of sound in world units per second, or zero if there
       was an error.
    */
    float getSpeedOfSound() const;

    /* Sets the speed of sound to new_value world units per second and returns
       true on success.

      remarks:
          Only positive values (i.e. greater than zero) are allowed.
    */
    bool setSpeedOfSound(const float new_value);

    // **listener management functions**
    //   positioning
    bool setListenerPostion(const float x, const float y, const float z);
    std::vector<float> getListenerPosition() const;
    bool translateListenerPostion(const float delta_x, const float delta_y, const float delta_z);
    //   velocity
    bool setListenerVelocity(const float x, const float y, const float z);
    std::vector<float> getListenerVelocity() const;
    //   orientation of listener
    std::vector<float> getListenerOrientation() const;
    bool rotateListener(const float x_axis, const float y_axis=0.0f, const float z_axis=0.0f);

    //device query functions
    /* tries to return the current device's name in result and returns true, if
       successful. If the function fails, it will return false and the content
       of result will not be changed.
    */
    bool getCurrentDeviceName(std::string& result) const;

    /* tries to return the default device's name in result and returns true, if
       successful. If the function fails, it will return false and the content
       of result will not be changed.
    */
    bool getDefaultDeviceName(std::string& result) const;

    /* tries to return a list of the names of all available devices in result
       and returns true, if successful. If the function fails, it will return
       false and the content of result will not be changed.
    */
    bool getAvailableDevices(std::vector<std::string>& result) const;

    //singleton access method
    static Sound& get();
  private:
    /* constructor - private due to singleton pattern */
    Sound();

    /* empty copy constructor */
    Sound(const Sound& op){}

    std::map<std::string, Media*> m_MediaList;
    std::map<std::string, Source*> m_SourceList;

    ALCdevice *pDevice;
    ALCcontext *pContext;
    bool AL_Ready;
    bool InitInProgress;
};//class Sound

} //namespace

#endif // SOUND_H
