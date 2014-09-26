/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2013, 2014  Thoronador

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

#include "MediaWave.h"
#include <fstream>
#include <cstdlib>
#include "../Messages.h"

namespace Dusk
{

MediaWave::MediaWave(const std::string& identifier, const std::string& PathToMedia)
: Media(identifier, PathToMedia)
{
  TRiffChunk riff_c;
  TFmtChunk fmt_c;
  TDataChunk data_c;
  std::ifstream dat;
  char * temp;

  dat.open(PathToMedia.c_str(), std::ios::in | std::ios::binary);
  if(!dat)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: Unable to open stream for reading.\n"
              << "       File: \"" <<PathToMedia<<"\".\n\n";
    throw MediaCreationException();
  }
  dat.read(riff_c.Riff, 4); // "RIFF"
  if ((riff_c.Riff[0]!='R') || (riff_c.Riff[1]!='I') || (riff_c.Riff[2]!='F')
       || (riff_c.Riff[3]!='F'))
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" RIFF header.\n";
    dat.close();
    throw MediaCreationException();
  }
  dat.read((char*) &(riff_c.len), 4); //file size - 8 (in Bytes)
  dat.read(riff_c.Wave, 4); // "WAVE"
  if ((riff_c.Wave[0]!='W') || (riff_c.Wave[1]!='A') || (riff_c.Wave[2]!='V')
       || (riff_c.Wave[3]!='E'))
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: File \""<<PathToMedia<<"\" has incorrect"
              <<" WAVE header.\n";
    dat.close();
    throw MediaCreationException();
  }
  //Format chunk
  dat.read(fmt_c.fmt_, 4); // "fmt "
  if ((fmt_c.fmt_[0]!='f') || (fmt_c.fmt_[1]!='m') || (fmt_c.fmt_[2]!='t')
       || (fmt_c.fmt_[3]!=' '))
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: File \""<<PathToMedia
              <<"\" has incorrect format chunk header signature.\n";
    dat.close();
    throw MediaCreationException();
  }
  dat.read((char*) &(fmt_c.chunk_size), 4); //should have value of exactly 16
  //In case the format chunk is larger than that, everything after the 16th byte
  // will be ignored.
  if (fmt_c.chunk_size<16)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: Format chunk of file \""
              <<PathToMedia<<"\" has incorrect size of "<<fmt_c.chunk_size
              <<" bytes. (Should be 16 instead.)\n";
    dat.close();
    throw MediaCreationException();
  }
  else if (fmt_c.chunk_size>16)
  {
    DuskLog() << "MediaWave::MediaWave: Warning: Format chunk of file \""
              <<PathToMedia<<"\" is larger than 16 bytes. Actual size is "
              <<fmt_c.chunk_size<<" bytes. Everything after 16th byte will be "
              <<"ignored.\n";
  }
  dat.read((char*) &(fmt_c.FormatTag), 2); //should have value of 1 for PCM
                                        //(this is what we have for typical .wav)
  if (fmt_c.FormatTag!=1)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: File \""<<PathToMedia<<"\" is "
              << "not of PCM format. Format index: " <<fmt_c.FormatTag<<".\n";
    dat.close();
    throw MediaCreationException();
  }
  dat.read((char*) &(fmt_c.Channels), 2);  // 1 for mono, 2 for stereo
  dat.read((char*) &(fmt_c.SamplesPerSecond), 4);
  dat.read((char*) &(fmt_c.BytesPerSecond), 4);
  dat.read((char*) &(fmt_c.BlockAlign), 2);
  dat.read((char*) &(fmt_c.BitsPerSample), 2);

  //for larger format chunks: read rest of chunk into temp buffer and discard it
  if (fmt_c.chunk_size > 16)
  {
    //check for size, again, to avoid to allocate immense amount of memory
    if (fmt_c.chunk_size <= 1024)
    {
      temp = (char*) malloc(fmt_c.chunk_size -16);
      dat.read(temp, fmt_c.chunk_size -16);
      free(temp);
    }
    else //chunk is larger than 1 KB; quite unnormal
    {
      DuskLog() << "MediaWave::MediaWave: ERROR: Format chunk is much too big ("
                << fmt_c.chunk_size << " bytes). Exiting.\n";
      dat.close();
      throw MediaCreationException();
    }
  }//if

  //read the data chunk
  dat.read(data_c.data, 4); // "data"
  if ((data_c.data[0]!='d') || (data_c.data[1]!='a') || (data_c.data[2]!='t')
       || (data_c.data[3]!='a'))
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: File \""<<PathToMedia
              <<"\" has incorrect data chunk header signature.\n";
    dat.close();
    throw MediaCreationException();
  }
  dat.read((char*) &(data_c.length_of_data), 4); //Länge des folgenden Datenblocks
                                                 //bzw. der restlichen Datei

  //check if data length is valid
  if (data_c.length_of_data<fmt_c.BlockAlign)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: Data chunk of file \""<<PathToMedia
              << "\" is too short to contain valid data. Exiting.\n";
    dat.close();
    throw MediaCreationException();
  }

  //length check for high bound
  if (data_c.length_of_data>MaxMediaSize_MB*1024*1024)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: Size of PCM data from file \""
              <<PathToMedia<<"\" would be larger than "<< MaxMediaSize_MB
              << " MB. Aborting to avoid abusive memory allocation.\n";
    dat.close();
    throw MediaCreationException();
  }//if

  //for calculations of number and size of buffers
  unsigned long buffer_size=0, buffer_num=0, i=0;
  unsigned long last_buffer_size=0;
  ALenum error_state;//return value from alGetError();
  ALenum format_type;//format of data

  //Not sure about what is a good buffer size for WAVE/PCM file
  //Following line may need to be adjusted :?
  buffer_size =  32* fmt_c.BlockAlign *1024;
  //assure that buffer is not larger than amount of available data
  if (buffer_size>data_c.length_of_data)
  {
    buffer_size = data_c.length_of_data;
  }
  //buffer size needs to be an exact multiple of block align
  if ((buffer_size % fmt_c.BlockAlign)!=0)
  {
    //decrease buffer size to next lowest multiple of block align
    buffer_size = (buffer_size/fmt_c.BlockAlign) * fmt_c.BlockAlign;
    if (buffer_size==0)
    {
      buffer_size = fmt_c.BlockAlign;
    }
  }
  last_buffer_size = buffer_size;

  //determine number of buffers
  buffer_num = data_c.length_of_data/ buffer_size;
  if ((data_c.length_of_data % buffer_size)!=0)
  {
    buffer_num = buffer_num +1; //increase number of buffers
    last_buffer_size = data_c.length_of_data % buffer_size; //size of last buffer
                                                    //is diff. from regular size
  }
  //initialise values
  m_Name = identifier;
  m_FileName = PathToMedia;
  num_buffers = buffer_num;
  buffers = NULL;
  //attached_to.clear();

  //allocate memory for buffer_num ALuint variables
  buffers = (ALuint*) malloc(sizeof(ALuint)*buffer_num);
  alGetError();//clear error state
  alGenBuffers(buffer_num, buffers);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "MediaWave::MediaWave: ERROR while generating buffers for \""
              <<PathToMedia<<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided buffer array is not large enough to "
                     << "hold the requested number of buffers.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the buffers.\n";
           break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n";
           break;
    }//swi
    dat.close();
    alDeleteBuffers(buffer_num, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }

  //determine format
  if (fmt_c.BitsPerSample==16)
  {
    switch(fmt_c.Channels)
    {
      case 4:
        format_type = alGetEnumValue("AL_FORMAT_QUAD16");
        break;
      case 2:
        format_type = AL_FORMAT_STEREO16;
        break;
      case 1:
        format_type = AL_FORMAT_MONO16;
        break;
      default:
        DuskLog() << "MediaWave::MediaWave: ERROR: File \"" <<PathToMedia
                  <<"\" seems to have "<<fmt_c.Channels<<" channels. However, "
                  <<"only four, two (stereo) or one (mono) channels are "
                  <<"supported.\n";
        dat.close();
        alDeleteBuffers(num_buffers, buffers);
        free(buffers);
        buffers = NULL;
        throw MediaCreationException();
        break; //pro forma :P
    }//swi
  }
  else if (fmt_c.BitsPerSample==8)
  {
    switch(fmt_c.Channels)
    {
      case 4:
        format_type = alGetEnumValue("AL_FORMAT_QUAD8");
        break;
      case 2:
        format_type = AL_FORMAT_STEREO8;
        break;
      case 1:
        format_type = AL_FORMAT_MONO8;
        break;
      default:
        DuskLog() << "MediaWave::MediaWave: ERROR: File \"" <<PathToMedia
                  <<"\" seems to have "<<fmt_c.Channels<<" channels. However, "
                  <<"only four, two (stereo) or one (mono) channels are "
                  <<"supported. Creation aborted.\n";
        dat.close();
        alDeleteBuffers(num_buffers, buffers);
        free(buffers);
        buffers = NULL;
        throw MediaCreationException();
    }//swi
  }
  else //Bits per Sample neither 16 nor 8, thus unsupported by OpenAL
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: The sample rate of \""
              <<PathToMedia<<"\" ("<<fmt_c.BitsPerSample<< " bits per sample) "
              <<"is not supported. OpenAL supports only 8 and 16 bit samples.\n";
    dat.close();
    alDeleteBuffers(num_buffers, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }
  //check for valid format enumeration value
  if (format_type == 0) //call to alGetEnumValue could not get a proper result
  {
    DuskLog() << "MediaWave::MediaWave: ERROR: Could not find a valid OpenAL "
              << "format enumeration value. Most likely the format of \""
              <<PathToMedia<<"\" (channels: "<<fmt_c.Channels<<"; bits per sample: "
              <<fmt_c.BitsPerSample<<") is not supported.\n";
    dat.close();
    alDeleteBuffers(num_buffers, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }

  //now read the data into OpenAL buffers
  temp = (char*) malloc(buffer_size);
  for (i=0; i<buffer_num-1; i=i+1)
  {
    dat.read(temp, buffer_size);
    alGetError(); //clear error state
    alBufferData(buffers[i], format_type, temp, buffer_size, fmt_c.SamplesPerSecond);
    error_state = alGetError();
    if (error_state!= AL_NO_ERROR)
    {
      DuskLog() << "MediaWave::MediaWave: ERROR while buffering data.\n";
      switch (error_state)
      {
        case AL_INVALID_ENUM:
             DuskLog() <<"    The specified format does not exist.\n";
             break;
        case AL_INVALID_VALUE:
             DuskLog() <<"    The size parameter is not valid for the given format"
                       <<" or the buffer is already in use.\n";
             break;
        case AL_OUT_OF_MEMORY:
             DuskLog() <<"    Not enough memory to create the buffer.\n";
             break;
        default:
             DuskLog() <<"    Unknown error. Error code: "<<(int)error_state
                       <<".\n";
             break;
      }//swi
      dat.close();
      free(temp);
      //delete all previously generated buffers
      alDeleteBuffers(num_buffers, buffers);
      free(buffers);
      buffers = NULL;
      throw MediaCreationException();
    }//if
  }//for

  //read last buffer
  dat.read(temp, last_buffer_size);
  alGetError(); //clear error state
  alBufferData(buffers[buffer_num-1], format_type, temp, last_buffer_size, fmt_c.SamplesPerSecond);
  error_state = alGetError();
  if (error_state!= AL_NO_ERROR)
  {
    DuskLog() << "MediaWave::MediaWave: ERROR while buffering data.\n";
    switch (error_state)
    {
      case AL_INVALID_ENUM:
           DuskLog() <<"    The specified format does not exist.\n";
           break;
      case AL_INVALID_VALUE:
           DuskLog() <<"    The size parameter is not valid for the given format"
                     <<" or the buffer is already in use.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() <<"    Not enough memory to create the buffer.\n";
           break;
      default:
           DuskLog() <<"    Unknown error. Error code: "<<(int)error_state<<".\n";
           break;
    }//swi
    dat.close();
    free(temp);
    //delete all previously generated buffers
    alDeleteBuffers(num_buffers, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }//if
  else
  { //in case of no error we still need to close the stream and free the pointer
    dat.close();
    free(temp);
  }//else

  //we're finally done with reading the data
}


} //namespace
