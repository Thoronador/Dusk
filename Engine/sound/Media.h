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

#ifndef SOUND_MEDIA_H_INCLUDED
#define SOUND_MEDIA_H_INCLUDED

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

namespace Dusk
{

//buffer management type
class Media
{
  public:
    /* constructor

       parameters:
           identifier  - unique identifier
           PathToMedia - path to the Wave or Ogg-Vorbis file
    */
    Media(const std::string& identifier, const std::string& PathToMedia);

    /* destructor */
    virtual ~Media();

    /* maximum size for created media in MB */
    const static unsigned int MaxMediaSize_MB;

    /* returns the media's "name" */
    const std::string& getIdentifier() const;

    /* returns the media's file name */
    const std::string& getFileName() const;

    /* returns the number of associated AL buffers */
    ALuint getNumberOfBuffers() const;

    /* returns the internal buffer pointer */
    const ALuint * getBufferPointer() const;

    /* returns the internal buffer pointer

       remarks:
           Except for the const its the same as the above function. You should
           always prefer the functions above, if possible.
    */
    ALuint * getNonConstBufferPointer() const;

    /* notifies the media about being attached to a new source

       parameters:
           sourceID - identifier of the source
    */
    void notifyAttached(const std::string& sourceID);

    /* notifies the media about being detached from a source

       parameters:
           sourceID - identifier of the source
    */
    void notifyDetached(const std::string& sourceID);

    /* returns a vector of names of sources to which this media is attached */
    const std::vector<std::string>& getRelatedSources() const;
  protected:
    std::string m_Name; //unique name, case sensitive
    std::string m_FileName; //pro forma, not really needed after file is loaded
    ALuint num_buffers;
    ALuint * buffers;
    std::vector<std::string> attached_to;
}; //class

class MediaCreationException: public std::exception
{
  public:
    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
}; //class

class MediaDestructionException: public std::exception
{
  public:
    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
}; //class

} //namespace

#endif // SOUND_MEDIA_H_INCLUDED
