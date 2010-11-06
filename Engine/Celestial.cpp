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
  if (m_End>m_Start)
  {
    return (currentTime>=m_Start) and (currentTime<=m_End);
  }//if
  return (currentTime>=m_Start) or (currentTime<=m_End);
}

float Celestial::getSpanRatio(const float currentTime) const
{
  if (m_End==m_Start) return 0.0f; //prevent divide by zero error
  return (currentTime-m_Start)/getSpan();
}

} //namespace
