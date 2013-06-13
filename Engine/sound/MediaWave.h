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

#ifndef SOUND_MEDIAWAVE_H_INCLUDED
#define SOUND_MEDIAWAVE_H_INCLUDED

#include <stdint.h>
#include "Media.h"

//Types for wave format:
struct TRiffChunk
{
  char Riff[4];
  uint32_t len; //32 bit unsigned int; file size-8
  char Wave[4];
};
struct TFmtChunk
{
  char fmt_[4];
  uint32_t chunk_size;//32 bit unsigned int;
  uint16_t FormatTag;//16 bit unsigned int;
  uint16_t Channels;
  uint32_t SamplesPerSecond;
  uint32_t BytesPerSecond;
  uint16_t BlockAlign;
  uint16_t BitsPerSample;
};
struct TDataChunk
{
  char data[4];
  uint32_t length_of_data;
};

namespace Dusk
{

class MediaWave: public Media
{
  public:
    /* constructor

       parameters:
           identifier  - unique identifier
           PathToMedia - path to the Wave file
    */
    MediaWave(const std::string& identifier, const std::string& PathToMedia);
}; //class MediaWave

} //namespace

#endif // SOUND_MEDIAWAVE_H_INCLUDED
