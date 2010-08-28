#include "Camera.h"
#include "API.h"
#include "Landscape.h"
//#include "Player.h"

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
        //m_translationVector = Ogre::Vector3::ZERO;
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
        if (scn!=NULL)
        {
          setupCamera(scn);
        }
    }

    Camera::~Camera()
    {
        //destructor
        m_RotationPerSecond = 0.0f;
        //now detach and delete everything related to the camera
        if (Ogre::Root::getSingletonPtr()!=NULL)
        {
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
        else
        {
          m_Camera = NULL;
          m_Secondary = NULL;
          m_Primary = NULL;
        }
    }

    Ogre::Camera* Camera::getOgreCamera() const
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
        if (m_RotationPerSecond != 0.0)
        {
          m_Primary->yaw(Ogre::Degree(m_RotationPerSecond * evt.timeSinceLastFrame), Ogre::Node::TS_WORLD);
        }
    }

    void Camera::rotate(const float rotation)
    {
        m_RotationPerSecond += rotation;
    }

    void Camera::rotateOnceX(const float delta)
    {
      if (m_Primary!=NULL)
      {
        m_Primary->yaw(Ogre::Degree(delta), Ogre::Node::TS_WORLD);
      }
    }

    void Camera::rotateOnceY(const float delta)
    {
      if (m_Primary!=NULL)
      {
        m_Primary->pitch(Ogre::Degree(delta), Ogre::Node::TS_LOCAL);
        const Ogre::Degree deg = m_Primary->getOrientation().getPitch();
        //adjust values, if larger than 90° or smaller than -90°
        if (deg>Ogre::Degree(90.0f))
        {
          m_Primary->pitch(Ogre::Degree(90.0f)-deg);
        }//if >90°
        else if (deg<Ogre::Degree(-90.0f))
        {
          m_Primary->pitch(Ogre::Degree(-90.0f)-deg);
        }//if <-90°
      }//if primary camera node present
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
