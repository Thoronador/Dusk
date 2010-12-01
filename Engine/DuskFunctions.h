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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-05
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
     - 2010-04-21 (rev 190) - BoolToString() added
     - 2010-05-05 (rev 196) - documentation updated
     - 2010-06-03 (rev 214) - CountBitsSet() added
     - 2010-08-26 (rev 235) - CSVToVector() added
     - 2010-12-01 (rev 264) - faster version of FileExists()
     - 2010-12-01 (rev 265) - double-precision version of FloatToString()

 ToDo list:
     - implement a faster version of FileExists()
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

/* returns the number of bits set in value*/
unsigned int CountBitsSet(unsigned int value);

/* Returns string representation of integer 'value' */
std::string IntToString(const int value);

/* Returns string representation of unsigned integer 'value' */
std::string IntToString(const unsigned int value);

/* Tries to convert the provided string into an integer. If the string contains
   invalid characters (e.g. letters) and cannot be converted to an integer, the
   function will return the specified default value. */
int StringToInt(const std::string& str_input, const int default_value);

/* Returns string representation of float 'value' */
std::string FloatToString(const float value);

/* Returns string representation of double 'value' */
std::string FloatToString(const double value);

/* Tries to convert the provided string into a floating point number. If the
   string contains invalid characters (e.g. letters) and cannot be converted to
   a floating point value, the function will return the specified default value.
*/
float StringToFloat(const std::string& str_input, const float default_value);

/* converts a boolean value into a string ('yes' or 'no') */
std::string BoolToString(const bool value);


/**
 * Trims the given string from leading and trailing spaces.
 *
 * @param p_string      The string to trim.
 * @return the trimmed string.
 */
std::string trim(std::string p_string);

/* splits a line of comma-separated values into separate strings and puts each
   string into the returned vector, i.e. "abc;foo;bar" becomes the vector
   consisting of the elements "abc", "foo" and "bar".

   parameters:
       csv_data - the string containing the character-separated values
*/
std::vector<std::string> CSVToVector(const std::string& csv_data);

/* structure for file list entries */
struct FileEntry {
         std::string FileName;
         bool IsDirectory;
};//struct

/* returns a list of all files in the given directory as a vector */
std::vector<FileEntry> get_DirectoryFileList(const std::string& Directory);

/* Checks for existence of file FileName and returns true, if it exists.*/
bool FileExists(const std::string& FileName);

}//namespace

#endif // DUSKFUNCTIONS_H
