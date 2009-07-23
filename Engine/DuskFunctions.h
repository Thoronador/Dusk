#ifndef DUSKFUNCTIONS_H
#define DUSKFUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>

#if defined(_WIN32)
  //future Windows includes go here
#else
  //Linux directory entries
  #include <dirent.h>
#endif

namespace Dusk
{

typedef struct FileEntry {
                 std::string FileName;
                 bool IsDirectory;
};//struct

//only works on Linux at the moment
std::vector<FileEntry> getDirectoryFileList(const std::string Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  std::cout <<"Dusk::getDirectoryFileList: Warning: this function is currently"
            <<" not implemented on other platforms than Linux. Returning empty"
            <<" list.\n";
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
}//function

}//namespace

#endif // DUSKFUNCTIONS_H
