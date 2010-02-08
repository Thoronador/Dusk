/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-12-23
 Purpose: provides some (more or less) useful functions which are used in
          several headers and/or source files
 History:
     - 2009-07-23 (rev 110) - initial version; contains getDirectoryFileList()
     - 2009-07-24 (rev 111) - IntToString() and FloatToString() added,
                              getDirectoryFileList() removed
     - 2009-08-11 (rev 120) - StringToInt(), StringToFloat() added
     - 2009-08-24 (rev 121) - IntToString() for unsigned integers added
     - 2009-12-23 (rev 145) - getDirectoryFileList() added (again),
                              split into .h and .cpp
     - 2010-02-08 (rev 168) - trim() added /moved from Script.h

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


/**
 * Trims the given string from leading and trailing spaces.
 *
 * @param p_string      The string to trim.
 * @return the trimmed string.
 */
std::string trim(std::string p_string);

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
