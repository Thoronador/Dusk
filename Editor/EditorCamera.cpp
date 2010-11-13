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

#include "EditorCamera.h"

namespace Dusk
{

EditorCamera::EditorCamera()
{
  cam = NULL;
  m_primaryCameraNode = m_secondaryCameraNode = NULL;
  translation_per_second = Ogre::Vector3::ZERO;
  rotation_per_second = 0.0f;
  m_Turbo = false;
}

EditorCamera& EditorCamera::getSingleton()
{
  static EditorCamera Instance;
  return Instance;
}

EditorCamera::~EditorCamera()
{
  //empty
  if ( m_primaryCameraNode != NULL && m_secondaryCameraNode!=NULL
       && cam != NULL)
  {
    m_secondaryCameraNode->detachObject(cam);
    m_secondaryCameraNode->getCreator()->destroyCamera(cam);
    cam = NULL;

    m_primaryCameraNode->removeChild(m_secondaryCameraNode);
    m_secondaryCameraNode->getCreator()->destroySceneNode("Secondary SceneNode of EditorCamera");
    m_secondaryCameraNode = NULL;

    m_primaryCameraNode->getParentSceneNode()->removeChild(m_primaryCameraNode);
    m_primaryCameraNode->getCreator()->destroySceneNode("Primary SceneNode of EditorCamera");
    m_primaryCameraNode = NULL;
  }
}

Ogre::Camera* EditorCamera::getOgreCamera()
{
  return cam;
}

void EditorCamera::setupCamera(Ogre::SceneManager* scm)
{
  m_primaryCameraNode = scm->getRootSceneNode()->createChildSceneNode("Primary SceneNode of EditorCamera");
  m_secondaryCameraNode = m_primaryCameraNode->createChildSceneNode("Secondary SceneNode of EditorCamera");

  cam = scm->createCamera("EditorCam");
  cam->setNearClipDistance(5);

  m_secondaryCameraNode->attachObject(cam);
  // Position it at 500 in Z direction
  m_primaryCameraNode->setPosition(Ogre::Vector3(0,0,500));
  // Look back along -Z
  m_primaryCameraNode->lookAt(Ogre::Vector3(0,0,-300), Ogre::Node::TS_WORLD);
}

void EditorCamera::setPosition(const Ogre::Vector3& position)
{
  if (m_primaryCameraNode != NULL)
  {
    m_primaryCameraNode->setPosition(position);
  }
}

Ogre::Vector3 EditorCamera::getPosition(void)
{
  if (m_primaryCameraNode != NULL)
  {
    return m_primaryCameraNode->getPosition();
  }
  return Ogre::Vector3::ZERO;
}

Ogre::Quaternion EditorCamera::getOrientation(void)
{
  if (m_primaryCameraNode == NULL)
  {
    return Ogre::Quaternion::IDENTITY;
  }
  return m_primaryCameraNode->getOrientation();
}

void EditorCamera::setZoomDistance(const float distance)
{
  if (m_secondaryCameraNode!=NULL)
  {
    m_secondaryCameraNode->setPosition(0.0f, 0.0f, distance);
  }
}

float EditorCamera::getZoomDistance(void)
{
  if (m_secondaryCameraNode!=NULL)
  {
    return m_secondaryCameraNode->getPosition().z;
  }
  return 0.0f;
}

void EditorCamera::resetToOrigin(void)
{
  if (m_primaryCameraNode != NULL)
  {
    m_primaryCameraNode->setPosition(Ogre::Vector3(0,0,500));
    m_primaryCameraNode->lookAt(Ogre::Vector3(0,0,-300), Ogre::Node::TS_WORLD);
  }
  if (m_secondaryCameraNode != NULL)
  {
    m_secondaryCameraNode->setPosition(0.0f, 0.0f, 0.0f);
  }
}

void EditorCamera::processMovement(const float seconds)
{
  if ( m_primaryCameraNode == NULL )
  {
    return;
  }
  if ( translation_per_second != Ogre::Vector3::ZERO )
  {
    if (m_Turbo)
    {
      m_primaryCameraNode->translate(4*seconds*translation_per_second, Ogre::Node::TS_LOCAL);
    }
    else
    {
      m_primaryCameraNode->translate(seconds*translation_per_second, Ogre::Node::TS_LOCAL);
    }
  }
  if ( rotation_per_second != 0.0f )
  {
    m_primaryCameraNode->yaw(Ogre::Degree(rotation_per_second * seconds), Ogre::Node::TS_LOCAL);
  }
}

void EditorCamera::setRotationSpeed(const float degrees)
{
  rotation_per_second = degrees;
}

float EditorCamera::getRotationSpeed(void)
{
  return rotation_per_second;
}

void EditorCamera::relativeRotation(const float degrees)
{
  if ( m_primaryCameraNode == NULL )
  {
    return;
  }
  m_primaryCameraNode->yaw(Ogre::Degree(degrees), Ogre::Node::TS_LOCAL);
}

void EditorCamera::setTranslationVector(const Ogre::Vector3& velocity)
{
  translation_per_second = velocity;
}

const Ogre::Vector3& EditorCamera::getTranslationVector(void)
{
  return translation_per_second;
}

void EditorCamera::setTurboMode(const bool value)
{
  m_Turbo = value;
}

}//namespace
