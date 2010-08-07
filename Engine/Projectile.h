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
    Projectile(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~Projectile();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes GetType() const;

    /* Sets the time to live (TTL), i.e. the amount of seconds before the
       projectile will be destroyed automatically

       remarks:
         * A value of zero or less than zero means infinite time or until the
           projectile hits a target.
         * The main purpose for TTl is to avoid that projectiles which do not
           hit any target move on forever.
    */
    void SetTTL(const float newTTL);

    /* returns the time to live, i.e. the amount of seconds before the
       projectile will be destroyed
    */
    float GetTTL() const;

    /* constant value which can be used to signal infinite TTL */
    static const float InfiniteTTL;

    /* sets the object (usually an NPC) who shot this projectile

       parameters:
           emitter - object that emitted the projectile

       remarks:
           The emitter will be the only NPC that will NOT be hurt by the
           projectile.
    */
    void SetEmitter(DuskObject* emitter);

    /* returns the projectile's emitter, if any */
    DuskObject* GetEmitter() const;

    /* function to inject time for movement and perform movement */
    virtual void injectTime(const float SecondsPassed);

    /* causes the object to move towards a certain destination.
       Contrary to the implementation in UniformMotionObject, the projectile
       will NOT stop moving after it has reached that destination.

       remarks:
           Reimplemented from UniformMotionObject.
    */
    virtual void TravelToDestination(const Ogre::Vector3& dest);

    /* Saves the projectile to the given stream. Returns true on success, false
       otherwise.
    */
    virtual bool SaveToStream(std::ofstream& OutStream) const;

    /* Tries to load a projectile from the given stream. Returns true on
       success, false otherwise.
    */
    virtual bool LoadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string GetObjectMesh() const;
  private:
    // time to live before objet should be deleted
    float m_TTL;
    //NPC who shot this projectile
    DuskObject* m_Emitter;
}; //class

} //namespace

#endif // PROJECTILE_H
