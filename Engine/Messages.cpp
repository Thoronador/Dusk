/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2013  Thoronador

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

#include "Messages.h"
#include <iostream>
#include "DuskFunctions.h"

namespace Dusk
{

Messages::Messages(const std::string& LogFile, const bool out)
{
  mStream.open(LogFile.c_str());
  mOutput = out;
}

Messages::~Messages()
{
  mStream.flush();
  mStream.close();
}

Messages& Messages::getSingleton()
{
  static Messages Instance;
  return Instance;
}

void Messages::Log(const std::string& msg)
{
  mStream << msg;
  mStream.flush();
  if (mOutput)
  {
    std::cout << msg;
  }
}

void Messages::setOutput(const bool b)
{
  mOutput = b;
}

bool Messages::isOutput() const
{
  return mOutput;
}

Messages& Messages::operator<<(const uint16_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const int16_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const uint32_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const int32_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const uint64_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const int64_t n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const unsigned long n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const long n)
{
  mStream<<n;
  mStream.flush();
  if (mOutput)
  {
    std::cout << n;
  }
  return *this;
}

Messages& Messages::operator<<(const float f)
{
  Log(FloatToString(f));
  return *this;
}

Messages& Messages::operator<<(const double d)
{
  Log(FloatToString(d));
  return *this;
}

Messages& Messages::operator<<(const std::string& str)
{
  Log(str);
  return *this;
}

Messages& Messages::operator<<(const char* c_str)
{
  Log(std::string(c_str));
  return *this;
}

} //namespace
