/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "DuskFunctions.h"
#include <iostream>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
#elif defined(__linux__) || defined(linux)
  //Linux directory entries
  #include <dirent.h>
#else
  #error "Unknown operating system!"
#endif
#include <unistd.h>

namespace Dusk
{

unsigned int CountBitsSet(unsigned int value)
{
  unsigned int b;
  for (b=0; value; ++b)
  {
    value &= value - 1; //clear the least significant bit set
  }
  return b;
}

std::string IntToString(const int value)
{
  std::stringstream s_str;
  s_str << value;
  return s_str.str();
}

std::string IntToString(const unsigned int value)
{
  std::stringstream s_str;
  s_str << value;
  return s_str.str();
}

int StringToInt(const std::string& str_input, const int default_value)
{
  std::stringstream s_str(str_input);
  int value = default_value;
  if (!(s_str>>value))
  { //error during conversion -> return given default value
    return default_value;
  }
  return value;
}

std::string FloatToString(const float value)
{
  std::stringstream s_str;
  s_str << value;
  return s_str.str();
}

float StringToFloat(const std::string& str_input, const float default_value)
{
  std::stringstream s_str(str_input);
  float value = default_value;
  if (!(s_str>>value))
  { //error during conversion -> return given default value
    return default_value;
  }
  return value;
}

std::string BoolToString(const bool value)
{
  if (value) return "yes";
  return "no";
}

std::string trim(std::string p_string)
{
  int pos = -1;
  int look = 0;
  int size = p_string.size();
  while (look<size)
  {
    if (p_string.at(look) == ' ')
    {
      pos = look;
    }
    else
    {
      break;
    }
    look++;
  }//while
  if (pos > -1)
      p_string.erase(0, pos + 1);

  pos = - 1;
  look = p_string.size() - 1;
  while (look>=0)
  {
    if (p_string.at(look) == ' ')
    {
      pos = look;
    }
    else
    {
      break;
    }
    look--;
  }//while
  if (pos > -1)
      p_string.erase(pos, p_string.size() - pos);

  return p_string;
}

std::vector<std::string> CSVToVector(const std::string& csv_data)
{
  std::vector<std::string> result;
  result.clear(); //just to be safe
  if (csv_data.empty()) return result;
  size_t next_character_index = 0;
  while (next_character_index < csv_data.size())
  {
    const size_t next_comma = csv_data.find(',', next_character_index);
    if (next_comma==std::string::npos)
    {
      //no comma left in string, the rest is all one string
      result.push_back( csv_data.substr(next_character_index));
      next_character_index = csv_data.size();
      break;
    }
    else
    {
      //comma found
      result.push_back(csv_data.substr(next_character_index, next_comma-next_character_index));
      next_character_index = next_comma+1;
    }
  }//while
  return result;
}

std::vector<FileEntry> get_DirectoryFileList(const std::string& Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(Directory+"*").c_str(),&sr);
  if (handle == -1)
  {
    std::cout << "Dusk::getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }
  //search it
  while( _findnext(handle, &sr)==0)
  {
    one.FileName = std::string(sr.name);
    one.IsDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
  }//while
  _findclose(handle);
  #elif defined(__linux__) || defined(linux)
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "Dusk::getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.FileName = std::string(entry->d_name);
    one.IsDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
      entry = readdir(direc);
    }
  }//while
  closedir(direc);
  #else
    #error "Unknown operating system!"
  #endif
  return result;
}//function

bool FileExists(const std::string& FileName)
{
   return (access(FileName.c_str(), F_OK)==0);
}

} //namespace
