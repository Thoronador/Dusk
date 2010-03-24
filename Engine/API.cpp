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
        m_Window = NULL;
        m_SceneManager = NULL;
        m_Application = NULL;
        m_FrameListener = NULL;
    }

    void API::setOgreObjects(Ogre::Root* root,Ogre::RenderWindow* window,Ogre::SceneManager* mgr)
    {
        m_Root = root;
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
    #else
    void API::setApplication(EditorApplication* app)
    {
        m_Application = app;
    }

    void API::setFrameListener(EditorFrameListener* op)
    {
        m_FrameListener = op;
    }
    #endif

    //basic get functions
    Ogre::Root* API::getOgreRoot()
    {
        return m_Root;
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
    #else
    EditorApplication* API::getApplication()
    {
        return m_Application;
    }

    EditorFrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }
    #endif

} //namespace
