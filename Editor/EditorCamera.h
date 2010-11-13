/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2009, 2010 thoronador

    The Dusk Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Editor.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <OgreVector3.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

namespace Dusk
{

  /* EditorCamera Singleton class

     manages the free-flying camera of the Editor
  */
  class EditorCamera
  {
    public:
      /* destructor */
      virtual ~EditorCamera();

      /* singleton access method */
      static EditorCamera& getSingleton();

      /* initialises camera and sets up the internal structure

         parameters:
             scm - pointer to the scene manager that will hold the camera
      */
      void setupCamera(Ogre::SceneManager* scm);

      /* returns a pointer to the internally used Ogre::Camera */
      Ogre::Camera* getOgreCamera();

      /* sets the camera's position in world space

         parameters:
             position - vector of the new position
      */
      void setPosition(const Ogre::Vector3& position);

      /* returns current position of the camera */
      Ogre::Vector3 getPosition(void);

      /* returns the camera's orientation */
      Ogre::Quaternion getOrientation(void);

      /* sets the zoom level of the camera. Positive values will zoom behind the
         camera, negative values zoom to a position in front of the camera.
      */
      void setZoomDistance(const float distance);

      /* returns the current zoom level */
      float getZoomDistance(void);

      /* resets the camera to its original position. This position is NOT the
         origin of the world space coordinate system, but somewhat close to it.
      */
      void resetToOrigin(void);

      /* sets the amount of rotation the camera performs around the y-axis
         within one second

         parameters:
             degrees - amount of rotation per second in degrees
      */
      void setRotationSpeed(const float degrees);

      /* returns the speed of camera rotation */
      float getRotationSpeed(void);

      /* causes a one-time rotation that is added to the current rotation of
         the camera

         parameters:
             degrees - amount of rotation
      */
      void relativeRotation(const float degrees);

      /* sets the movement vector (direction and length) of the camera

         parameter:
             velocity - movement per second
      */
      void setTranslationVector(const Ogre::Vector3& velocity);

      /* returns the current translation vector */
      const Ogre::Vector3& getTranslationVector(void);

      /* activates or deactivates the "turbo" for the camera, depending on value

         parameters:
             value - if set to true, the camera will move at four times the
                     normal speed. If set to false, the camera will move at
                     normal speed.
      */
      void setTurboMode(const bool value);

      /* moves and rotates the camera, depending on the amount of time that has passed */
      void processMovement(const float seconds);
    private:
      /* constructor */
      EditorCamera();

      /* copy constructor - empty due to singleton pattern */
      EditorCamera(const EditorCamera& op){}
      Ogre::Camera* cam;
      Ogre::SceneNode* m_primaryCameraNode;
      Ogre::SceneNode* m_secondaryCameraNode;
      Ogre::Vector3 translation_per_second;
      bool m_Turbo;
      float rotation_per_second;
  }; //class

}//namespace

#endif // EDITORCAMERA_H
