/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-05
 Purpose: Light class
          represents a single light within the game

 History:
     - 2009-09-18 (rev 129) - initial version (by thoronador)
     - 2009-09-22 (rev 130) - load/save methods
     - 2009-09-24 (rev 131) - method declaration improved
     - 2009-09-27 (rev 133) - radius/range is now considered during Enable()
     - 2009-12-05 (rev 139) - GetType() added
                            - method declarations improved
     - 2010-05-05 (rev 196) - documentation update

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LIGHT_H
#define LIGHT_H
#include "DuskObject.h"
#include <string>
#include <fstream>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <OgreLight.h>

namespace Dusk
{

  class Light: public DuskObject
  {
    public:
      /* standard constructor */
      Light();

      /* constructor with parameter list

         remarks:
           Whenever you have the choice, you should prefer this constructor over
           the standard constructor.
      */
      Light(const std::string& ID, const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
            const Ogre::Vector3& dir = Ogre::Vector3::ZERO);

      /* Enables the light, i.e. tells the SceneManager to display it.
         Returns true on success, false on error.
      */
      virtual bool Enable(Ogre::SceneManager* scm);

      /* Disables the light, i.e. tells the SceneManager not to display it.
         Returns true on success, false on error. */
      virtual bool Disable();

      /* Returns true, if the light is currently enabled, false otherwise. */
      virtual bool IsEnabled() const;

      /* retrieves the object type as an enumeration value */
      virtual ObjectTypes GetType() const;

      /* sets the direction the light is shining to */
      void SetDirection(const Ogre::Vector3& dir);

      /* retrieves the light's direction */
      Ogre::Vector3 GetDirection() const;

      /* Saves the object to the given stream. Returns true on success, false
         otherwise.
      */
      virtual bool SaveToStream(std::ofstream& OutStream) const;

      /* Tries to load an object from the given stream. Returns true on
         success, false otherwise.

         remarks:
             If the function returns false, the data within the light object may
             be corrupted. It's advised not to use the object in this case.
      */
      virtual bool LoadFromStream(std::ifstream& InStream);
    protected:
      /* contrary to DuskObject, this entity member is of type Light and not of
         type Ogre::Entity. Might possibly cause problems.
      */
      Ogre::Light* entity;
      /* direction of the light */
      Ogre::Vector3 m_Direction;
  };//class

}//namespace

#endif // LIGHT_H
