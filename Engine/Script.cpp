/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2009, 2010 DaSteph, thoronador

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

#include "Script.h"
#include "DuskFunctions.h"

namespace Dusk
{

Script::Script(const std::string& p_string)
{
    m_string = p_string;
}

Script::~Script()
{
}

std::vector<std::string> Script::explodeCommands() const
{
    std::string p_string = m_string;
    std::vector<std::string> pieces;

    if (p_string.find(";") == std::string::npos)   //no semicolon -> one command
        pieces.push_back(trim(p_string));
    else
    {
        long offset = 0;
        long lastOffset = 0;
        long size = p_string.size();
        int brackLevel = 0;
        int quotLevel = 0;
        //starting from beginning, scanning script
        while (offset < size)
        {
            if (p_string.at(offset) == '{')
                brackLevel++;
            else if (p_string.at(offset) == '}')
                brackLevel--;
            else if (p_string.at(offset) == '"' && brackLevel == 0)
                quotLevel = (quotLevel + 1) % 2; // 1 -> 0 bzw. 0 -> 1
            else if (p_string.at(offset) == ';' && brackLevel == 0 && quotLevel == 0)
            {
                long length = offset - lastOffset;
                if (length > 0) //skip empty commands
                    pieces.push_back(trim(p_string.substr(lastOffset, length)));
                lastOffset = offset + 1;
            }
            offset++;
        }
        //afterwards we have to add the rest
        if (lastOffset < size) //maybe the last command was ended
        {
            long length = offset - lastOffset;
            if (length > 0) //skip empty commands
                pieces.push_back(trim(p_string.substr(lastOffset, length)));
        }
    }

    return pieces;
}

Script Script::getStartScript() const
{
    return Script(std::string("start {") + m_string + std::string("}"));
}

Script Script::getStopScript() const
{
    return Script(std::string("stop {") + m_string + std::string("}"));
}

std::string Script::getStringRepresentation() const
{
  return m_string;
}

bool Script::isEmpty() const
{
  return (m_string=="");
}

} // namespace
