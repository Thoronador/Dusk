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
