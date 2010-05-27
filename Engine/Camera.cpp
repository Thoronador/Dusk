#include "Camera.h"
#include "API.h"
#include "Landscape.h"
#include "Player.h"

namespace Dusk
{
    //height of camera above ground/ landscape (in world units)
    const float Camera::cAboveGroundLevel = 50.0;

    //limits for zoom distance (values are chosen randomly)
    const float Camera::cMinimumZoom = 0.0;
    const float Camera::cMaximumZoom = 500.0;
    const float Camera::cRecommendedZoomStep = 2.5;

    Camera& Camera::getSingleton()
    {
      static Camera Instance;
      return Instance;
    }

    void Camera::setupCamera(Ogre::SceneManager* scn)
    {
        if (m_Primary == NULL)
        {
          m_Primary = scn->getRootSceneNode()->createChildSceneNode();
          m_Primary->setDirection(Ogre::Vector3(0,0,0));
        }
        if (m_Secondary == NULL)
        {
          m_Secondary = m_Primary->createChildSceneNode();
          m_Secondary->setDirection(Ogre::Vector3(0,0,0));
        }
        if (m_Camera == NULL)
        {
          m_Camera = scn->createCamera("PlayerCam");
          m_Camera->setDirection(Ogre::Vector3(0,0,0));
          m_Secondary->attachObject(m_Camera);
          m_Camera->setNearClipDistance(5);
        }
        m_translationVector = Ogre::Vector3::ZERO;
        m_RotationPerSecond = 0.0f;
    }

    bool Camera::setupDone() const
    {
      return (m_Primary!=NULL) and (m_Secondary!=NULL) and (m_Camera!=NULL);
    }

    Camera::Camera(Ogre::SceneManager* scn)
    {
        //ctor
        m_Primary = NULL;
        m_Secondary = NULL;
        m_Camera = NULL;
        m_JumpVelocity = 0.0f;
        m_Jump = false;
        if (scn!=NULL)
        {
          setupCamera(scn);
        }
    }

    Camera::~Camera()
    {
        //destructor
        m_translationVector = Ogre::Vector3::ZERO;
        m_RotationPerSecond = 0.0f;
        //now detach and delete everything related to the camera
        m_Secondary->detachObject(m_Camera);
        m_Secondary->getCreator()->destroyCamera(m_Camera);
        m_Camera = NULL;
        m_Primary->removeChild(m_Secondary);
        m_Secondary->getCreator()->destroySceneNode(m_Secondary->getName());
        m_Secondary = NULL;
        m_Primary->getParentSceneNode()->removeChild(m_Primary);
        m_Primary->getCreator()->destroySceneNode(m_Primary->getName());
        m_Primary = NULL;
    }

    Ogre::Camera* Camera::getOgreCamera()
    {
        return m_Camera;
    }

    void Camera::setPosition(const Ogre::Vector3& position)
    {
        m_Primary->setPosition(position);
        Player::GetSingleton().SetPosition(position);
    }

    void Camera::lookAt(const Ogre::Vector3& direction)
    {
        m_Primary->lookAt(direction,Ogre::Node::TS_LOCAL);
    }

    void Camera::move(const Ogre::FrameEvent& evt)
    {
        if (m_translationVector != Ogre::Vector3::ZERO or m_Jump)
        {
          m_Primary->translate(m_translationVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

          //new height
          Ogre::Vector3 camPos = m_Primary->getPosition();
          const float new_height = Landscape::GetSingleton().GetHeightAtPosition(camPos.x, camPos.z)
                                    +cAboveGroundLevel;
          if (m_Jump)
          {
            const float jump_height = camPos.y+ m_JumpVelocity*evt.timeSinceLastFrame;
            if (jump_height>=new_height)
            {
              const float gravity = -9.81*2.25; //maybe we need to adjust this later
              m_Primary->setPosition(camPos.x, jump_height, camPos.z);
              m_JumpVelocity = m_JumpVelocity + gravity*evt.timeSinceLastFrame;
            }
            else
            {
              m_Jump = false;
              m_Primary->setPosition(camPos.x, new_height, camPos.z);
            }
          }
          else
          {
            m_Primary->setPosition(camPos.x, new_height, camPos.z);
          }
          //adjust player's position
          Player::GetSingleton().SetPosition(m_Primary->getPosition());
        }
        //handle rotation
        if (m_RotationPerSecond != 0.0)
        {
          m_Primary->yaw(Ogre::Degree(m_RotationPerSecond * evt.timeSinceLastFrame), Ogre::Node::TS_LOCAL);
        }
    }

    void Camera::translate(const Ogre::Vector3& translationVector)
    {
        m_translationVector += translationVector;
        //check for player class
        if (m_translationVector!= Ogre::Vector3::ZERO)
        { //player/camera started moving/ moves on, so play run animation...
          Player::GetSingleton().StartAnimation("RunBase", true);
          Player::GetSingleton().StartAnimation("RunTop", true);
          //... and stop idle animations
          Player::GetSingleton().StopAnimation("IdleBase");
          Player::GetSingleton().StopAnimation("IdleTop");
        }
        else
        { //player has stopped moving, stop run animation...
          Player::GetSingleton().StopAnimation("RunBase");
          Player::GetSingleton().StopAnimation("RunTop");
          //... and play idle animations
          Player::GetSingleton().StartAnimation("IdleBase", true);
          Player::GetSingleton().StartAnimation("IdleTop", true);
        }
    }

    void Camera::rotate(const float rotation)
    {
        m_RotationPerSecond += rotation;
    }

    void Camera::jump(void)
    {
      if (!m_Jump)
      {
        m_Jump = true;
        m_JumpVelocity = 30.0f; //only a guess; maybe we should adjust that
                                //  value later
      }
    }

    void Camera::setZoom(const float distance)
    {
      if (m_Secondary!=NULL)
      {
        if (distance >= cMinimumZoom)
        {
          if (distance <= cMaximumZoom)
          {
            m_Secondary->setPosition(0.0f, 0.0f, distance);
          }
          else
          {
            m_Secondary->setPosition(0.0f, 0.0f, cMaximumZoom);
          }
        }
        else
        {
          m_Secondary->setPosition(0.0f, 0.0f, cMinimumZoom);
        }
      }
    }

    float Camera::getZoom() const
    {
      if (m_Secondary!=NULL)
      {
        return m_Secondary->getPosition().z;
      }
      return 0.0f;
    }
}
