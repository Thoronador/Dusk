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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-31
 Purpose: Projectile class
          This class represents a single projectile (e.g. a bullet fired from
          a weapon) within the game.

 History:
     - 2010-05-31 (rev 211) - initial version (by thoronador)
     - 2010-08-01 (rev 220) - GetObjectMesh() added
                            - Enable() removed (inherited method is used instead)
     - 2010-08-07 (rev 222) - ray-based collision detection implemented
                            - emitter property added
     - 2010-08-15 (rev 224) - ray-based collision detection extended
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-13 (rev 254) - update due to renaming of some functions in
                              Landscape class
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-11-26 (rev 260) - canCollide() added (always returns true)
     - 2010-11-30 (rev 262) - collision detection improved

 ToDo list:
     - Improve collision detection for projectile. Currently only collisions
       with landscape, static objects (DuskObjects) and NPCs will work.
     - ???

 Bugs:
     - Untested. No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "UniformMotionObject.h"

namespace Dusk
{

class Projectile: public UniformMotionObject
{
  public:
    /* default constructor */
    Projectile();

    /* constructor with parameter list */
    Projectile(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~Projectile();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes getDuskType() const;

    /* returns true, if the object shall be considered during collision
       detection

       remarks:
           Will always return true for projectiles.
    */
    virtual bool canCollide() const;

    /* Sets the time to live (TTL), i.e. the amount of seconds before the
       projectile will be destroyed automatically

       parameters:
          newTTL - the new time to live

       remarks:
         * A value of zero or less than zero means infinite time or until the
           projectile hits a target.
         * The main purpose for TTL is to avoid that projectiles which do not
           hit any target move on forever.
    */
    void setTTL(const float newTTL);

    /* returns the time to live, i.e. the amount of seconds before the
       projectile will be destroyed
    */
    float getTTL() const;

    /* constant value which can be used to signal infinite TTL */
    static const float InfiniteTTL;

    /* sets the object (usually an NPC) who shot this projectile

       parameters:
           emitter - object that emitted the projectile

       remarks:
           The emitter will be the only NPC that will NOT be hurt by the
           projectile.
    */
    void setEmitter(DuskObject* emitter);

    /* returns the projectile's emitter, if any */
    DuskObject* getEmitter() const;

    /* function to inject time for movement and perform movement */
    virtual void injectTime(const float SecondsPassed);

    /* causes the object to move towards a certain destination.
       Contrary to the implementation in UniformMotionObject, the projectile
       will NOT stop moving after it has reached that destination.

       parameters:
           dest - the new destination

       remarks:
           Reimplemented from UniformMotionObject.
    */
    virtual void travelToDestination(const Ogre::Vector3& dest);

    /* Saves the projectile to the given stream. Returns true on success, false
       otherwise.

       parameters:
           OutStream - the output stream that will be used to save the projectile
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Tries to load a projectile from the given stream. Returns true on
       success, false otherwise.

       parameters:
           InStream - the input stream that will be used to load the projectile
    */
    virtual bool loadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string getObjectMesh() const;
  private:
    // time to live before objet should be deleted
    float m_TTL;
    //NPC who shot this projectile
    DuskObject* m_Emitter;
}; //class

} //namespace

#endif // PROJECTILE_H
