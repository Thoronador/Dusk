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

#include "MediaOggVorbis.h"
#include "../Messages.h"
#include <cstdlib>

#if defined(_WIN32)
  #include "oggvorbis/vorbisfile.h" //Vorbis header
#elif defined(__linux__) || defined(linux)
  #include <vorbis/vorbisfile.h> //Vorbis header
#else
  #error "Unknown operating system!"
#endif

namespace Dusk
{

MediaOggVorbis::MediaOggVorbis(const std::string& identifier, const std::string& PathToMedia)
: Media(identifier, PathToMedia)
{
  OggVorbis_File ov;
  vorbis_info * vinfo;
  FILE * dat;
  int section, ret;
  double time_total;
  ogg_int64_t pcm_samples;

  dat = fopen(PathToMedia.c_str(), "rb");
  if (dat==NULL)
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: Could not open file \""
              << PathToMedia << "\" via fopen properly.\n";
    throw MediaCreationException();
  }//if

  ret = ov_open_callbacks(dat, &ov, NULL, 0, OV_CALLBACKS_DEFAULT);
  if (ret<0)
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: Could not open file \""
              << PathToMedia << "\" via callbacks properly.\n";
    switch(ret)
    {
      case OV_EREAD:
           DuskLog() << "    A read from the media returned an error.\n"; break;
      case OV_ENOTVORBIS:
           DuskLog() << "    Bitstream does not contain any Vorbis data.\n";
           break;
      case OV_EVERSION:
           DuskLog() << "    Version mismatch.\n"; break;
      case OV_EBADHEADER:
           DuskLog() << "    Invalid Vorbis header.\n"; break;
      case OV_EFAULT:
           DuskLog() << "    Internal logic error/ bug.\n"; break;
      default:
           DuskLog() << "    Unknown error. Code: " << ret <<".\n"; break;
    }//switch
    ov_clear(&ov);
    throw MediaCreationException();
  }//if
  DuskLog() << "MediaOggVorbis::MediaOggVorbis: Debug info: File \""<< PathToMedia
             << "\" opened properly.\n";

  vinfo = ov_info(&ov, -1);
  DuskLog() <<"MediaOggVorbis::MediaOggVorbis: Information for \""<<PathToMedia<<"\":\n";
  if (vinfo == NULL)
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: Warning: Could not get file "
              << "information for \"" << PathToMedia << "\".\n";
    ov_clear(&ov);
    throw MediaCreationException();
  }

  DuskLog() << "    Vorbis encoder version: " << vinfo->version <<"\n"
            << "    Channels: " << vinfo->channels << "\n"
            << "    Sampling rate: " << vinfo->rate <<"\n    Bitrate:\n"
            << "    Nominal: " << vinfo->bitrate_nominal <<"\n"
            << "    Upper: " << vinfo->bitrate_upper <<"\n"
            << "    Lower: " << vinfo->bitrate_lower <<"\n";
  DuskLog() << "\n    Number of streams: "<< ov_streams(&ov) <<"\n";

  if ((vinfo->channels!=1) && (vinfo->channels!=2))
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: File \""<<PathToMedia<<"\" has "
              <<vinfo->channels<< " audio channels, however only one (mono) or "
              << "two (stereo) are supported.\n";
    ov_clear(&ov);
    throw MediaCreationException();
  }
  time_total = ov_time_total(&ov, -1);
  if (time_total == OV_EINVAL)
  {
    DuskLog() << "Couldn't get total time for stream. Stream does not exist or "
              << "is unseekable.\n";
  }
  else
  {
    DuskLog() << "    Total time: "<<time_total<<" seconds.\n";
  }
  pcm_samples = ov_pcm_total(&ov, -1);
  if (pcm_samples == OV_EINVAL)
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: Couldn't get total sample count"
              <<" for stream. Stream does not exist or is unseekable.\n";
    ov_clear(&ov);
    throw MediaCreationException();
  }
  DuskLog() << "    PCM samples: "<<pcm_samples<<" samples.\n";

  long int bytes_read, total_read;
  long int data_size;

  data_size = pcm_samples*vinfo->channels*2;

  if ((data_size>MaxMediaSize_MB*1024*1024)/*MaxMediaSize_MB MB (currently 30 MB)*/ || (data_size<=0))
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: Size of uncompressed stream "
              << "from file \""<<PathToMedia<<"\" would be larger than "
              << MaxMediaSize_MB << " MB. Aborting to avoid abusive memory "
              << "allocation.\n";
    ov_clear(&ov);
    throw MediaCreationException();
  }

  char * buffer = new char[data_size];
  DuskLog() << "MediaOggVorbis::MediaOggVorbis: Debug: "<<data_size<<" bytes allocated "
              <<"for uncompressed data from file \""<<PathToMedia<<"\".\n";
  section = 0;
  total_read = 0;

  do
  {
    bytes_read = ov_read(&ov, &buffer[total_read], data_size-total_read /*buffer length*/,
                         0 /*little endian*/, 2 /*16 bit data*/,
                         1 /*signed data*/, &section);
    if (bytes_read<0)
    {
      DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR while reading from file \""
                << PathToMedia << "\".\n";
      switch(bytes_read)
      {
        case OV_HOLE:
             DuskLog() << "    Interruption in data stream.\n"; break;
        case OV_EBADLINK:
             DuskLog() << "    Invalid stream section supplied.\n"; break;
        case OV_EINVAL:
             DuskLog() << "    File headers couldn't be read or are corrupt; or"
                       << " open call for supplied file failed.\n"; break;
        default:
             DuskLog() << "    Unknown error code: "<<bytes_read<<".\n"; break;
      }//switch
    }//if
    else
    {
      total_read = total_read + bytes_read;
    }//else
  } while (bytes_read>0);

  if (bytes_read<0)
  {
    delete buffer;
    ov_clear(&ov);
    throw MediaCreationException();
  }
  DuskLog() << "MediaOggVorbis::MediaOggVorbis: Debug: "<<total_read<<" bytes read, "
            <<data_size<<" bytes were assumed.\n";
  if (total_read!=data_size)
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR: Size miscalculations in file \""
                << PathToMedia << "\". Read "<<total_read<<" bytes, but assumed"
                <<" "<<data_size<<" bytes. Aborting.\n";
    delete buffer;
    ov_clear(&ov);
    throw MediaCreationException();
  }//if

  //all is read, now pass it to OpenAL
  ALenum format;
  if (vinfo->channels==2)
  {
    format = AL_FORMAT_STEREO16;
  }
  else
  {
    format = AL_FORMAT_MONO16;
  }

  //initialise media's values
  m_Name = identifier;
  m_FileName = PathToMedia;
  num_buffers = 1;
  buffers = NULL;
  attached_to.clear();

  //allocate memory for ALuint variable
  buffers = (ALuint*) malloc(sizeof(ALuint));
  alGetError();//clear error state
  alGenBuffers(1, buffers);
  ALenum error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR while generating buffers for \""
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
                    <<(int)error_state<<".\n"; break;
    }//swi
    ov_clear(&ov);
    delete buffer;
    alDeleteBuffers(1, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }

  error_state = alGetError();//clear error state
  alBufferData(buffers[0], format, buffer, data_size, vinfo->rate);
  error_state = alGetError();
  if (error_state !=AL_NO_ERROR) //error occured
  {
    DuskLog() << "MediaOggVorbis::MediaOggVorbis: ERROR while filling buffers for \""
              <<PathToMedia<<"\".\n";
    switch (error_state)
    {
      case AL_INVALID_VALUE:
           DuskLog() << "    The provided buffer array size is not valid for "
                     << "the given format, or the data pointer is NULL.\n";
           break;
      case AL_OUT_OF_MEMORY:
           DuskLog() << "    Not enough memory to generate the buffers.\n";
           break;
      case AL_INVALID_ENUM:
           DuskLog() << "    The specified format does not exist.\n"; break;
      default:
           DuskLog()<<"    Unknown error occured. Error code: "
                    <<(int)error_state<<".\n"; break;
    }//swi
    ov_clear(&ov);
    delete buffer;
    alDeleteBuffers(1, buffers);
    free(buffers);
    buffers = NULL;
    throw MediaCreationException();
  }

  //finally, we are through
  delete buffer;
  ov_clear(&ov);
}

} //namespace
