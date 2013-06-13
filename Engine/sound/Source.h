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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2013-06-12
 Purpose: Source class
          manages a single sound source

 History:
     - 2013-06-12 - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SOUND_SOURCE_H_INCLUDED
#define SOUND_SOURCE_H_INCLUDED

#include <exception>
#include <string>
#include <vector>

#if defined(_WIN32)
  #include "openal/al.h" //OpenAL header
#elif defined(__linux__) || defined(linux)
  #include <AL/al.h> //OpenAL header
#else
  #error "Unknown operating system!"
#endif

#include "Media.h"

namespace Dusk
{

class Source
{
  private:
    std::string m_Name; //unique name, case sensitive
    ALuint sourceID;
    Media * attachedMedia;
  public:
    /* constructor

       parameters:
           identifier - unique identifier of the object
    */
    Source(const std::string& identifier);

    /* destructor */
    ~Source();

    /* returns the name */
    const std::string& getIdentifier() const;

    /* Attach: associates existing Source with a Media */
    bool attach(Media& theMedia);

    /* Detach: revokes association between Source and its attached Media */
    bool detach();

    /* returns true, if a media is attached to that source */
    bool hasAttachedMedia() const;

    /* returns reference to the attached media.
       If no media is attached, the function throws an exception.
       Use hasAttachedMedia() to check that before caling this function.
    */
    const Media& getAttachedMedia() const;

    /* Starts playback of media attached to the Source, and returns true on
       success.

       remarks:
           Before you can call this function, you have to load a audio file to
           a media and attach that media to the Source. Assuming that all
           function calls return true, the following lines are needed to play a
           file named "loud.ogg":

           Sound::get().Init(); // <-- only once at the very beginning
           Sound::get().CreateNoise("This Noise");
           Sound::get().CreateMedia("Some Media", "loud.ogg");
           Sound::get().Attach("This Noise", "Some Media");
           Sound::get().PlayNoise("This Noise");
    */
    bool play();

    /* Pauses a playing source and returns true on success.
       Pausing a source that is either paused or stopped is a legal no-op.
    */
    bool pause();

    /* Resumes a previously paused source and returns true on success.
       Unpausing a playing or stopped source is legal no-op, which will return
       true.
    */
    bool unPause();

    /* Stops a playing source and returns true on success, false on failure/error.
       Stopping an already stopped source is legal no-op and will return true.
    */
    bool stop();

    /* Sets a source into looping mode if doLoop==true, otherwise it gets the
       source out of looping mode. Returns true on success, false otherwise.
    */
    bool loop(const bool doLoop = true);

    /* Sets the offset of the source to the given amount of seconds and returns
       true on success. Trying to set an offset beyond the length of the
       attached media will result in failure.
    */
    bool setOffset(const float seconds);

    /* Retrieves source offset in seconds. On error, -1.0 is returned. */
    float getOffset() const;

    //   state retrieval functions
    /* Returns true if the source is currently playing. */
    bool isPlaying() const;

    /* Returns true if the source is in looping mode. */
    bool isLooping() const;

    // noise volume functions
    bool setVolume(const float volume = 1.0f);
    float getVolume(const bool consider_MinMax = false) const;
    // position of source
    bool setPosition(const float x, const float y, const float z);
    std::vector<float> getPosition() const;
    // velocity of sources
    bool setVelocity(const float x, const float y, const float z);
    std::vector<float> getVelocity() const;
}; //class

class SourceCreationException: public std::exception
{
  public:
    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
}; //class

class SourceDestructionException: public std::exception
{
  public:
    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
}; //class

class SourceHasNoMediaException: public std::exception
{
  public:
    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
}; //class

} //namespace

#endif // SOUND_SOURCE_H_INCLUDED
