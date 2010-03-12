#include "API.h"
#ifndef DUSK_EDITOR
  #include "Application.h"
#else
  #include "../Editor/EditorApplication.h"
#endif

namespace Dusk
{
    API& API::getSingelton()
    {
        static API Instance;
        return Instance;
    }

    API::API()
    {
        //ctor
        m_Root = NULL;
        m_Camera = NULL;
        m_Window = NULL;
        m_SceneManager = NULL;
        m_Application = NULL;
        m_FrameListener = NULL;
        #ifndef DUSK_EDITOR
        m_DuskCamera = NULL;
        #endif
    }

    void API::setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr)
    {
        m_Root = root;
        m_Camera = camera;
        m_Window = window;
        m_SceneManager = mgr;
    }

    #ifndef DUSK_EDITOR
    void API::setApplication(Dusk::Application* app)
    {
        m_Application = app;
    }

    void API::setFrameListener(Dusk::FrameListener* op)
    {
        m_FrameListener = op;
    }

    void API::setDuskCamera(Dusk::Camera* cam)
    {
        m_DuskCamera = cam;
    }
    #else
    void API::setApplication(EditorApplication* app)
    {
        m_Application = app;
    }

    void API::setFrameListener(EditorFrameListener* op)
    {
        m_FrameListener = op;
    }

    /*void API::setDuskCamera(Dusk::Camera* cam)
    {
        m_EditorCamera = cam;
    } */
    #endif

    //basic get functions
    Ogre::Root* API::getOgreRoot()
    {
        return m_Root;
    }

    Ogre::Camera* API::getOgreCamera()
    {
        return m_Camera;
    }

    Ogre::RenderWindow* API::getOgreRenderWindow()
    {
        return m_Window;
    }

    Ogre::SceneManager* API::getOgreSceneManager()
    {
        return m_SceneManager;
    }

    #ifndef DUSK_EDITOR
    Dusk::Application* API::getApplication()
    {
        return m_Application;
    }

    Dusk::FrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }

    Dusk::Camera* API::getDuskCamera()
    {
        return m_DuskCamera;
    }
    #else
    EditorApplication* API::getApplication()
    {
        return m_Application;
    }

    EditorFrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }

    /*EditorCamera* API::getDuskCamera()
    {
        return m_EditorCamera;
    }*/
    #endif

} //namespace
