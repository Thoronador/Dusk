#include "Camera.h"
#include "API.h"
namespace Dusk
{
    Camera::Camera(Ogre::SceneManager* scn)
    {
        //ctor
        m_Primary = scn->getRootSceneNode()->createChildSceneNode();
        m_Secondary = m_Primary->createChildSceneNode();
        m_Camera = scn->createCamera("PlayerCam");
        m_Secondary->attachObject(m_Camera);
        m_Camera->setNearClipDistance(5);
        getAPI().setDuskCamera(this);
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
        m_Camera->lookAt(direction);
    }
    void Camera::move(const Ogre::FrameEvent& evt)
    {
    }
}

