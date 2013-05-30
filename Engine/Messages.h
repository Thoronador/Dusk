/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2013  Thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-13
 Purpose: Messages Singleton class
          represents a class to write log messages to a file

 History:
     - 2010-03-13 (rev 184) - initial version (by thoronador)
     - 2010-12-01 (rev 265) - documentation updated
                            - unneccessary member functions removed
                            - operator << overloaded for use with Messages
     - 2011-02-06 (rev 279) - minor error fixed
     - 2013-05-30           - better int types

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/


#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <fstream>
#include <deque>
#include <stdint.h>

namespace Dusk
{

class Messages
{
  public:
    /* destructor */
    ~Messages();

    /* singleton access method */
    static Messages& getSingleton();

    /* writes a string to the logfile

       parameters:
           msg - the string that has to be written to the file
    */
    void Log(const std::string& msg);

    /* sets whether logged messages are also written to standard output or not

       parameters:
           b - If set to true, messages will be written to standard output, too.
               Otherwise they won't.
    */
    void setOutput(const bool b);

    /* returns true, if logged messages are also written to standard output */
    bool isOutput() const;

    Messages& operator<<(const uint16_t n);
    Messages& operator<<(const int16_t n);
    Messages& operator<<(const uint32_t n);
    Messages& operator<<(const int32_t n);
    Messages& operator<<(const uint64_t n);
    Messages& operator<<(const int64_t n);
    Messages& operator<<(const long int n);
    Messages& operator<<(const unsigned long int n);
    Messages& operator<<(const float f);
    Messages& operator<<(const double d);
    Messages& operator<<(const std::string& str);
    Messages& operator<<(const char* c_str);

  private:
    std::ofstream mStream;
    bool mOutput;

    /* constructor

       parameters:
           LogFile - file to which the logs will be written
           out     - if set to true, messages will be displayed at standard
                     output, too.
    */
    Messages(const std::string& LogFile="Dusk.log", const bool out=true);

    /* empty copy constructor due to singleton pattern */
    Messages(const Messages& op) {}
};//class

inline void DuskLog(const std::string& text)
{
  Messages::getSingleton().Log(text);
}

inline Messages& DuskLog()
{
  return Messages::getSingleton();
}

} //namespace

#endif // MESSAGES_H
