/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2013  Thoronador

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

#include "AnyConversion.h"
#include "../Messages.h"
#include "AnimatedObject.h"
#include "Container.h"
#include "Item.h"
#include "Light.h"
#include "NPC.h"
#include "Player.h"
#include "Projectile.h"
#include "Resource.h"
#include "UniformMotionObject.h"
#include "Vehicle.h"
#include "WaypointObject.h"
#include "Weapon.h"

namespace Dusk
{

DuskObject* AnyToObjectPtr(const Ogre::Any& any)
{
  if (any.isEmpty()) return NULL;
  if (any.getType() == typeid(DuskObject*))
    return Ogre::any_cast<DuskObject*>(any);
  if (any.getType() == typeid(AnimatedObject*))
    return Ogre::any_cast<AnimatedObject*>(any);
  if (any.getType() == typeid(Container*))
    return Ogre::any_cast<Container*>(any);
  if (any.getType() == typeid(Item*))
    return Ogre::any_cast<Item*>(any);
  if (any.getType() == typeid(Light*))
    return Ogre::any_cast<Light*>(any);
  if (any.getType() == typeid(NPC*))
    return Ogre::any_cast<NPC*>(any);
  if (any.getType() == typeid(Player*))
    return Ogre::any_cast<Player*>(any);
  if (any.getType() == typeid(Projectile*))
    return Ogre::any_cast<Projectile*>(any);
  if (any.getType() == typeid(Resource*))
    return Ogre::any_cast<Resource*>(any);
  if (any.getType() == typeid(UniformMotionObject*))
    return Ogre::any_cast<UniformMotionObject*>(any);
  if (any.getType() == typeid(Vehicle*))
    return Ogre::any_cast<Vehicle*>(any);
  if (any.getType() == typeid(WaypointObject*))
    return Ogre::any_cast<WaypointObject*>(any);
  if (any.getType() == typeid(Weapon*))
    return Ogre::any_cast<Weapon*>(any);
  //unknown type
  DuskLog() << "Warning: unknown type in Dusk::AnyToObjectPtr, type name is \""
            << any.getType().name() << "\"!\n";
  return NULL;
}

} //namespace
