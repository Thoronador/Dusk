#include "Camera.h"
#include "API.h"
#include "Landscape.h"

namespace Dusk
{

    const float Camera::cAboveGroundLevel = 50.0;

    Camera::Camera(Ogre::SceneManager* scn)
    {
        //ctor
        m_Primary = scn->getRootSceneNode()->createChildSceneNode();
        m_Primary->setDirection(Ogre::Vector3(0,0,0));
        m_Secondary = m_Primary->createChildSceneNode();
        m_Secondary->setDirection(Ogre::Vector3(0,0,0));
        m_Camera = scn->createCamera("PlayerCam");
        m_Camera->setDirection(Ogre::Vector3(0,0,0));
        m_Secondary->attachObject(m_Camera);
        m_Camera->setNearClipDistance(5);
        getAPI().setDuskCamera(this);
        m_translationVector = Ogre::Vector3::ZERO;
        m_RotationPerSecond = 0.0f;
    }

    Camera::~Camera()
    {
        //dtor
    }

    Ogre::Camera* Camera::getOgreCamera()
    {
        return m_Camera;
    }

    void Camera::setPosition(const Ogre::Vector3& position)
    {
        m_Primary->setPosition(position);
    }

    void Camera::lookAt(const Ogre::Vector3& direction)
    {
        m_Primary->lookAt(direction,Ogre::Node::TS_LOCAL);
    }

    void Camera::move(const Ogre::FrameEvent& evt)
    {
        if (m_translationVector != Ogre::Vector3::ZERO)
        {
          m_Primary->translate(m_translationVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

          //new height
          Ogre::Vector3 camPos = m_Primary->getPosition();
          float new_height = Landscape::GetSingleton().GetHeightAtPosition(camPos.x, camPos.z);
          m_Primary->setPosition(camPos.x, new_height+cAboveGroundLevel, camPos.z);
        }
        if (m_RotationPerSecond != 0.0)
        {
          m_Primary->yaw(Ogre::Degree(m_RotationPerSecond * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
        }
    }

    void Camera::translate(const Ogre::Vector3& translationVector)
    {
        m_translationVector += translationVector;
    }

    void Camera::rotate(const float rotation)
    {
        m_RotationPerSecond += rotation;
    }
}

