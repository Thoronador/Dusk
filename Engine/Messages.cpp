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
