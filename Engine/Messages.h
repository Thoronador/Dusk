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

#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <fstream>
#include <deque>

namespace Dusk
{

class Messages
{
  public:
    ~Messages();
    static Messages& GetSingleton();
    void Log(const std::string& msg);
    void setOutput(const bool b);
    bool isOutput() const;
    size_t getTailSize() const;
    std::string getTailEntry(const unsigned int pos) const;
  private:
    static const size_t cMaxTailLength;
    std::ofstream mStream;
    std::deque<std::string> mTail;
    bool mOutput;
    Messages(const std::string& LogFile="Dusk.log", const bool out=true);
    Messages(const Messages& op) {}
};//class

inline void DuskLog(const std::string& text)
{
  Messages::GetSingleton().Log(text);
}

} //namespace

#endif // MESSAGES_H
