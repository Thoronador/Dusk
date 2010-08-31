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
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced

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

         parameters:
             scm - the scene manager that will be used to create the light
      */
      virtual bool enable(Ogre::SceneManager* scm);

      /* Disables the light, i.e. tells the SceneManager not to display it.
         Returns true on success, false on error. */
      virtual bool disable();

      /* Returns true, if the light is currently enabled, false otherwise. */
      virtual bool isEnabled() const;

      /* retrieves the object type as an enumeration value */
      virtual ObjectTypes getDuskType() const;

      /* sets the direction the light is shining to

         parameters:
             dir - the light's new direction
      */
      void setDirection(const Ogre::Vector3& dir);

      /* retrieves the light's direction */
      Ogre::Vector3 getDirection() const;

      /* Saves the object to the given stream. Returns true on success, false
         otherwise.

         parameters:
             OutStream - the output stream that is used to save the object
      */
      virtual bool saveToStream(std::ofstream& OutStream) const;

      /* Tries to load an object from the given stream. Returns true on
         success, false otherwise.

         parameters:
             InStream - the input stream that is used to load the object

         remarks:
             If the function returns false, the data within the light object may
             be corrupted. It's advised not to use the object in this case.
      */
      virtual bool loadFromStream(std::ifstream& InStream);
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
