/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2011 thoronador, walljumper, ssj5000

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

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>
#if defined(_WIN32)
  #include <windows.h>
#elif defined(__linux__) || defined(linux)
  #include <dlfcn.h>
#else
  #error "Unknown operating system!"
#endif
#include "openal/al.h" //OpenAL header
#include "openal/alc.h"  //OpenAL header
#include "oggvorbis/vorbisfile.h" //Vorbis header

//Types for wave format:
struct TRiffChunk
{
  char Riff[4];
  unsigned long len; //32 bit unsigned int; file size-8
  char Wave[4];
};
struct TFmtChunk
{
  char fmt_[4];
  unsigned long chunk_size;//32 bit unsigned int;
  unsigned short FormatTag;//16 bit unsigned int;
  unsigned short Channels;
  unsigned long SamplesPerSecond;
  unsigned long BytesPerSecond;
  unsigned short BlockAlign;
  unsigned short BitsPerSample;
};
struct TDataChunk
{
  char data[4];
  unsigned long length_of_data;
};

//buffer management type
struct TMediaRec
{
  std::string MediaName; //unique name, case sensitive
  std::string FileName; //pro forma, not really needed after file is loaded
  ALuint num_buffers;
  ALuint * buffers;
  std::vector<std::string> attached_to;
  TMediaRec * next;
};

//source management type
struct TNoiseRec
{
  std::string NoiseName; //unique name, case sensitive
  ALuint sourceID;
  TMediaRec * attachedMedia;
  TNoiseRec * next;
};

//type declaration for OggVorbis function pointers
typedef int (*P_ov_clear)(OggVorbis_File *vf);
typedef vorbis_comment* (*P_ov_comment)(OggVorbis_File *vf,int link);
typedef vorbis_info* (*P_ov_info)(OggVorbis_File *vf,int link);
typedef int (*P_ov_open_callbacks)(void *datasource, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks);
typedef ogg_int64_t (*P_ov_pcm_total)(OggVorbis_File *vf,int i);
typedef long (*P_ov_read)(OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream);
typedef long (*P_ov_streams)(OggVorbis_File *vf);
typedef double (*P_ov_time_total)(OggVorbis_File *vf,int i);

const unsigned int MaxMediaSize_MB = 30;

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

    /* Returns true, if the OggVorbis functions are loaded

       remarks:
           If this function returns false, you will not be able to load any
           OggVorbis files. However, you still might be able to load Wave files,
           depending on the return value of isInitialized().
    */
    bool hasVorbis() const;

    // **presence checks**
    /* Returns true, if a Media named MediaIdentifier is present */
    bool isMediaPresent(const std::string& MediaIdentifier) const;

    /* Returns true, if a Noise named NoiseIdentifier is present */
    bool isNoisePresent(const std::string& NoiseIdentifier) const;

    /* Returns the list of all currently present noises.

       parameters:
           with_attached_media - if set to true, the list also contains the
                                 names of the media attached to each noise. (If
                                 no media is attached, this is an empty string.)
    */
    std::vector<std::string> getNoiseList(const bool with_attached_media = false) const;

    /* Returns the list of all currently loaded media.

      parameters:
          with_file_names - if set to true, the list also contains the names of
                            the files the media were loaded from.
    */
    std::vector<std::string> getMediaList(const bool with_file_names = false) const;

    // **noise management routines**
    /* Created a new Noise named NoiseIdentifier and returns true on success */
    bool createNoise(const std::string& NoiseIdentifier);

    /* Destroys the Noise named NoiseIdentifier and returns true on success */
    bool destroyNoise(const std::string& NoiseIdentifier);
    //   Attach: associates existing Noise with a Media
    bool attach(const std::string& NoiseIdentifier, const std::string& MediaIdentifier);
    //   Detach: revokes association between Noise and its attached Media
    bool detach(const std::string& NoiseIdentifier);
    //   noise playback management
    /* Starts playback of media attached to the Noise named NoiseIdentifier, and
       returns true on success.

      remarks:
          Before you can call this function, you have to load a audio file to
          a media and attach that media to the noise. Assuming that all function
          calls return true, the following lines are needed to play a file named
          "loud.ogg":

          Sound::get().Init(); // <-- only once at the very beginning
          Sound::get().CreateNoise("This Noise");
          Sound::get().CreateMedia("Some Media", "loud.ogg");
          Sound::get().Attach("This Noise", "Some Media");
          Sound::get().PlayNoise("This Noise");
    */
    bool playNoise(const std::string& NoiseIdentifier);

    /* Pauses a playing noise and returns true on success.
       Pausing a noise that is either paused or stopped is a legal no-op,
       trying to pause a non-existing noise is an no-op and will return false
       and print a warning/hint.
    */
    bool pauseNoise(const std::string& NoiseIdentifier);

    /* Resumes a previously paused noise and returns true on success..
       Unpausing a playing or stopped noise is legal no-op, which will return
       true.
    */
    bool unPauseNoise(const std::string& NoiseIdentifier);

    /* Stops a playing noise and returns true on success, false on failure/error.
       Stopping an already stopped noise is legal no-op and will return true.
    */
    bool stopNoise(const std::string& NoiseIdentifier);

    /* Sets a noise into looping mode if DoLoop==true, otherwise it gets the
       noise out of looping mode. Returns true on success, false otherwise.
    */
    bool loopNoise(const std::string& NoiseIdentifier, const bool DoLoop = true);

    /* Sets the offset of noise NoiseIdentifier to the given amount of seconds
       and returns true on success. Trying to set an offset beyond the length of
       the attached media will result in failure.
    */
    bool setNoiseOffset(const std::string& NoiseIdentifier, const float seconds);

    /* Retrieves noise offset in seconds. On error, -1.0 is returned. */
    float getNoiseOffset(const std::string& NoiseIdentifier) const;

    //   state retrieval functions
    /* Returns true if the noise NoiseIdentifier is currently playing. */
    bool isPlayingNoise(const std::string& NoiseIdentifier) const;

    /* Returns true if the noise NoiseIdentifier is in looping mode. */
    bool isLoopingNoise(const std::string& FileName) const;
    //   noise volume functions
    bool setNoiseVolume(const std::string& NoiseIdentifier, const float volume = 1.0f);
    float getNoiseVolume(const std::string& NoiseIdentifier, const bool consider_MinMax = false) const;
    //   position of noises
    bool setNoisePosition(const std::string& NoiseIdentifier, const float x, const float y, const float z);
    std::vector<float> getNoisePosition(const std::string& NoiseIdentifier) const;
    //   velocity of noises
    bool SetNoiseVelocity(const std::string& NoiseIdentifier, const float x, const float y, const float z);
    std::vector<float> GetNoiseVelocity(const std::string& NoiseIdentifier) const;


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
  protected:

  private:
    /* constructor - private due to singleton pattern */
    Sound();

    /* empty copy constructor */
    Sound(const Sound& op){}

    /* tries to load a RIFF Wave file into the buffers and returns true on
       success

       parameters:
           MediaIdentifier - identifier of the new media
           PathToMedia     - path to the Wave file
    */
    bool createWAVMedia(const std::string& MediaIdentifier, const std::string& PathToMedia);

    /* tries to load an Ogg-Vorbis file into the buffers and returns true on
       success, false on failure

       parameters:
           MediaIdentifier - identifier of the new media
           PathToMedia     - path to the Ogg-Vorbis file
    */
    bool createOggMedia(const std::string& MediaIdentifier, const std::string& PathToMedia);

    /* Initialises all internal function pointers with NULL.

      remarks:
         Never ever call this one manually, or you might render the whole
         class inoperative!
    */
    void AllFuncPointersToNULL(void);

    TMediaRec * pMediaList;
    TNoiseRec * pNoiseList;

    ALCdevice *pDevice;
    ALCcontext *pContext;
    bool AL_Ready;
    bool Vorbis_Ready;
    bool InitInProgress;

    #if defined(_WIN32)
      HINSTANCE libHandleAL; //handle to OpenAL dynamic library
      HINSTANCE libHandleOV; //handle to OggVorbis dynamic library
    #elif defined(__linux__) || defined(linux)
      void * libHandleAL; //handle to OpenAL dynamic library
      void * libHandleOV; //handle to OggVorbis dynamic library
    #else
      #error "Unknown operating system!"
    #endif
    //****
    //**OpenAL function pointers
    //**** alc.h: AL context functions
    //**** Context Management
    LPALCCREATECONTEXT alcCreateContext;
    LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
    LPALCPROCESSCONTEXT alcProcessContext;
    LPALCSUSPENDCONTEXT alcSuspendContext;
    LPALCDESTROYCONTEXT alcDestroyContext;
    LPALCGETCURRENTCONTEXT alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE alcGetContextsDevice;

    //**** Device Management
    LPALCOPENDEVICE alcOpenDevice;
    LPALCCLOSEDEVICE alcCloseDevice;

    //**** Error handling
    LPALCGETERROR alcGetError;

    //**** Extension handling functions (ALC)
    LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
    /* Disabled for now
    LPALCGETPROCADDRESS alcGetProcAddress;
    LPALCGETENUMVALUE alcGetEnumValue;
    */

    //**** Query functions
    LPALCGETSTRING alcGetString;
    LPALCGETINTEGERV alcGetIntegerv;

    //****al.h: AL functions
    //**** Renderer State management
    LPALENABLE alEnable;
    LPALDISABLE alDisable;
    LPALISENABLED alIsEnabled;

    //**** State retrieval
    LPALGETSTRING alGetString;
    LPALGETBOOLEANV alGetBooleanv;
    LPALGETINTEGERV alGetIntegerv;
    LPALGETFLOATV alGetFloatv;
    LPALGETDOUBLEV alGetDoublev;
    LPALGETBOOLEAN alGetBoolean;
    LPALGETINTEGER alGetInteger;
    LPALGETFLOAT alGetFloat;
    LPALGETDOUBLE alGetDouble;

    //***** Error handling
    LPALGETERROR alGetError;

    //**** Extension handling func (AL)
    LPALISEXTENSIONPRESENT alIsExtensionPresent;
    LPALGETPROCADDRESS alGetProcAddress;
    LPALGETENUMVALUE alGetEnumValue;

    //**** Set Listener parameters
    LPALLISTENERF alListenerf;
    LPALLISTENER3F alListener3f;
    LPALLISTENERFV alListenerfv;
    LPALLISTENERI alListeneri;
    LPALLISTENER3I alListener3i;
    LPALLISTENERIV alListeneriv;

    //**** Get Listener parameters
    LPALGETLISTENERF alGetListenerf;
    LPALGETLISTENER3F alGetListener3f;
    LPALGETLISTENERFV alGetListenerfv;
    LPALGETLISTENERI alGetListeneri;
    LPALGETLISTENER3I alGetListener3i;
    LPALGETLISTENERIV alGetListeneriv;

    //**** Create Source objects
    LPALGENSOURCES alGenSources;
    //**** Delete Source objects
    LPALDELETESOURCES alDeleteSources;
    //***Verify a handle is a valid source
    LPALISSOURCE alIsSource;

    //**** Set Source parameters
    LPALSOURCEF alSourcef;
    LPALSOURCE3F alSource3f;
    LPALSOURCEFV alSourcefv;
    LPALSOURCEI alSourcei;
    LPALSOURCE3I alSource3i;
    LPALSOURCEIV alSourceiv;

    //**** Get Source parameters
    LPALGETSOURCEF alGetSourcef;
    LPALGETSOURCE3F alGetSource3f;
    LPALGETSOURCEFV alGetSourcefv;
    LPALGETSOURCEI alGetSourcei;
    LPALGETSOURCE3I alGetSource3i;
    LPALGETSOURCEIV alGetSourceiv;

    //**** Source vector based playback calls
    LPALSOURCEPLAYV alSourcePlayv; // Play, replay, or resume (if paused) a list of Sources
    LPALSOURCESTOPV alSourceStopv; // Stop a list of Sources
    LPALSOURCEREWINDV alSourceRewindv; // Rewind a list of Sources
    LPALSOURCEPAUSEV alSourcePausev; // Pause a list of Sources

    //**** Source based playback calls
    LPALSOURCEPLAY alSourcePlay; // Play, replay, or resume a Source
    LPALSOURCESTOP alSourceStop; // Stop a Source
    LPALSOURCEREWIND alSourceRewind; // Rewind a Source (set playback postiton to beginning)
    LPALSOURCEPAUSE alSourcePause; // Pause a Source

    //**** Source Queuing
    LPALSOURCEQUEUEBUFFERS alSourceQueueBuffers;
    LPALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;

    //**** Create Buffer objects
    LPALGENBUFFERS alGenBuffers;
    //**** Delete Buffer objects
    LPALDELETEBUFFERS alDeleteBuffers;
    //**** Verify a handle is a valid Buffer
    LPALISBUFFER alIsBuffer;
    //**** Specify the data to be copied into a buffer
    LPALBUFFERDATA alBufferData;

    //**** Set Buffer parameters
    LPALBUFFERF alBufferf;
    LPALBUFFER3F alBuffer3f;
    LPALBUFFERFV alBufferfv;
    LPALBUFFERI alBufferi;
    LPALBUFFER3I alBuffer3i;
    LPALBUFFERIV alBufferiv;

    //**** Get Buffer parameters
    LPALGETBUFFERF alGetBufferf;
    LPALGETBUFFER3F alGetBuffer3f;
    LPALGETBUFFERFV alGetBufferfv;
    LPALGETBUFFERI alGetBufferi;
    LPALGETBUFFER3I alGetBuffer3i;
    LPALGETBUFFERIV alGetBufferiv;

    //**** Global Parameters
    /* Partially disabled for now
    LPALDOPPLERFACTOR alDopplerFactor;
    LPALDOPPLERVELOCITY alDopplerVelocity;*/
    LPALSPEEDOFSOUND alSpeedOfSound;
    /*LPALDISTANCEMODEL alDistanceModel;
    */

    //**** OggVorbis function pointers
    P_ov_clear ov_clear;
    P_ov_comment ov_comment;
    P_ov_info ov_info;
    P_ov_open_callbacks ov_open_callbacks;
    P_ov_pcm_total ov_pcm_total;
    P_ov_read ov_read;
    P_ov_streams ov_streams;
    P_ov_time_total ov_time_total;
};//class Sound

} //namespace

#endif // SOUND_H
