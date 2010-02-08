#include "Script.h"
#include "DuskFunctions.h"

namespace Dusk
{

Script::Script(std::string p_string)
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
