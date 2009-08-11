#ifndef DUSKFUNCTIONS_H
#define DUSKFUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace Dusk
{

std::string IntToString(int value)
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

std::string FloatToString(float value)
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

/*typedef struct FileEntry {
                 std::string FileName;
                 bool IsDirectory;
};//struct

std::vector<FileEntry> get_DirectoryFileList(const std::string Directory)
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
  #else
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
    //check for socket and pipes, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO)
    {
      result.push_back(one);
      entry = readdir(direc);
    }
  }//while
  closedir(direc);
  #endif
  return result;
}//function*/

}//namespace

#endif // DUSKFUNCTIONS_H
