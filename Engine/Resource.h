/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2012-06-30
 Purpose: Resource class
          represents a single resource (for crafting) within the game

 History:
     - 2012-06-30 (rev 307) - initial version (by thoronador)
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_RESOURCE_H
#define DUSK_RESOURCE_H

#include "InjectionObject.h"

namespace Dusk
{

class Resource: public InjectionObject
{
  public:
    /* default constructor */
    Resource();

    /* constructor with parameter list */
    Resource(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~Resource();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes getDuskType() const;

    /* returns true, if the object shall be considered during collision
       detection

       remarks:
           Will always return true for resources.
    */
    virtual bool canCollide() const;

    /* returns true, if the resource is spawned and the player can harvest it */
    bool isSpawned() const;

    /* function to inject time for movement and perform movement */
    virtual void injectTime(const float SecondsPassed);

    /* Saves the resource to the given stream. Returns true on success, false
       otherwise.

       parameters:
           outStream - the output stream that will be used to save the resource
    */
    virtual bool saveToStream(std::ofstream& outStream) const;

    /* Tries to load a resource from the given stream. Returns true on
       success, false otherwise.

       parameters:
           inStream - the input stream that will be used to load the resource
    */
    virtual bool loadFromStream(std::ifstream& inStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual const std::string& getObjectMesh() const;
  private:
    //spawn status
    bool m_Spawned;
    // time until respawn
    float m_RespawnTime;
}; //class

} //namespace

#endif // DUSK_RESOURCE_H
