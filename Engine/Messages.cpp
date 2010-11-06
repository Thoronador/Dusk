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

#include "Messages.h"
#include <iostream>

namespace Dusk
{

const size_t Messages::cMaxTailLength=30;

Messages::Messages(const std::string& LogFile, const bool out)
{
  mStream.open(LogFile.c_str());
  mOutput = out;
}

Messages::~Messages()
{
  mStream.close();
}

Messages& Messages::GetSingleton()
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
  mTail.push_back(msg);
  if (mTail.size()>cMaxTailLength)
  {
    mTail.pop_front();
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

size_t Messages::getTailSize() const
{
  return mTail.size();
}

std::string Messages::getTailEntry(const unsigned int pos) const
{
  if (pos>=mTail.size()) return "";
  return mTail.at(pos);
}

} //namespace
