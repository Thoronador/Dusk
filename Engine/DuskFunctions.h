/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    23.12.2009
 Purpose: provides some (more or less) useful functions which are used in
          several headers and/or source files
 History:
     - 23.07.2009 - initial version; contains getDirectoryFileList() only
     - 24.07.2009 - IntToString() and FloatToString() added,
                    getDirectoryFileList() removed
     - 11.08.2009 - StringToInt(), StringToFloat() added
     - 24.08.2009 - IntToString() for unsigned integers added
     - 23.12.2009 - getDirectoryFileList() added (again),
                    split into .h and .cpp

 ToDo list:
     - ???
 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSKFUNCTIONS_H
#define DUSKFUNCTIONS_H

#include <string>
#include <vector>

namespace Dusk
{

/* Returns string representation of integer 'value' */
std::string IntToString(int value);

/* Returns string representation of unsigned integer 'value' */
std::string IntToString(unsigned int value);

/* Tries to convert the provided string into an integer. If the string contains
   invalid characters (e.g. letters) and cannot be converted to an integer, the
   function will return the specified default value. */
int StringToInt(const std::string& str_input, const int default_value);

/* Returns string representation of float 'value' */
std::string FloatToString(float value);

/* Tries to convert the provided string into a floating point number. If the
   string contains invalid characters (e.g. letters) and cannot be converted to
   a floating point value, the function will return the specified default value.
*/
float StringToFloat(const std::string& str_input, const float default_value);

/* structure for file list entries */
struct FileEntry {
         std::string FileName;
         bool IsDirectory;
};//struct

/* returns a list of all files in the given directory as a vector */
std::vector<FileEntry> get_DirectoryFileList(const std::string& Directory);

/* Checks for existence of file FileName and returns true, if it exists. */
bool FileExists(const std::string& FileName);

}//namespace

#endif // DUSKFUNCTIONS_H
