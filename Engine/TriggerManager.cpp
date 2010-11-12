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

#include "TriggerManager.h"

namespace Dusk
{

TriggerManager::TriggerManager()
{
  //empty
}

TriggerManager::~TriggerManager()
{
  m_TriggerList.clear();
}

TriggerManager& TriggerManager::getSingleton()
{
  static TriggerManager Instance;
  return Instance;
}

unsigned int TriggerManager::getNumberOfTriggers() const
{
  return m_TriggerList.size();
}

bool TriggerManager::addTrigger(Trigger* trig)
{
  //we don't add NULL pointers
  if (NULL==trig) return false;
  const std::pair<std::set<Trigger*>::iterator, bool> p = m_TriggerList.insert(trig);
  return p.second;
}

bool TriggerManager::removeTrigger(Trigger* trig)
{
  if (NULL==trig) return false;
  return m_TriggerList.erase(trig)!=0;
}

TriggerManager::ConstIterator TriggerManager::getBegin() const
{
  return m_TriggerList.begin();
}

TriggerManager::ConstIterator TriggerManager::getEnd() const
{
  return m_TriggerList.end();
}

} //namespace
