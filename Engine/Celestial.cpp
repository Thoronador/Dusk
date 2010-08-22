#include "Celestial.h"

namespace Dusk
{

Celestial::Celestial()
{
  //give it a defined time span
  m_Start = 6.0f;
  m_End = 18.0f;
}

Celestial::~Celestial()
{
  //empty
}

void Celestial::setStartTime(const float s)
{
  if (s<24.0f or s>=0.0f)
  {
    m_Start = s;
  }
}

float Celestial::getStartTime() const
{
  return m_Start;
}

void Celestial::setEndTime(const float e)
{
  if (e<24.0f or e>=0.0f)
  {
    m_End = e;
  }
}

float Celestial::getEndTime() const
{
  return m_End;
}

float Celestial::getSpan() const
{
  if (m_End>m_Start) return m_End-m_Start;
  return m_End+24.0f-m_Start;
}

bool Celestial::isInSpan(const float currentTime) const
{
  return (m_End>m_Start and currentTime>=m_Start and currentTime<=m_End)
         or (currentTime>=m_Start or currentTime<=m_End);
}

float Celestial::getSpanRatio(const float currentTime) const
{
  if (m_End>m_Start) return currentTime/(m_End-m_Start);
  if (m_End==m_Start) return 0.0f; //prevent divide by zero error
  return currentTime/(24.0f+m_End-m_Start);
}

} //namespace
