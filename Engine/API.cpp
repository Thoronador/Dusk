#include "API.h"
#include "Application.h"

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
        m_Application = NULL;

        m_Root = NULL;
        m_Camera = NULL;
        m_Window = NULL;
        m_SceneManager = NULL;

        m_Application = NULL;
        m_FrameListener = NULL;
        m_Console = NULL;
        m_Dispatcher = NULL;
        m_InputSystemBinding = NULL;
        m_InputSystemEditor = NULL;
        m_Scene = NULL;
    }
    void API::setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr)
    {
        m_Root = root;
        m_Camera = camera;
        m_Window = window;
        m_SceneManager = mgr;
    }
    void API::setApplication(Dusk::Application* app)
    {
        m_Application = app;
    }
    void API::setFrameListener(Dusk::FrameListener* op)
    {
        m_FrameListener = op;
    }
    void API::setConsole(Dusk::Console* op)
    {
        m_Console = op;
    }
    void API::setDispatcher(Dusk::Dispatcher* op)
    {
        m_Dispatcher = op;
    }
    void API::setInputSystemBinding(Dusk::InputSystemBinding* op)
    {
        m_InputSystemBinding = op;
    }
    void API::setInputSystemEditor(Dusk::InputSystemEditor* op)
    {
        m_InputSystemEditor = op;
    }
    void API::setScene(Dusk::Scene* op)
    {
        m_Scene = op;
    }

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

    Dusk::Application* API::getApplication()
    {
        return m_Application;
    }
    Dusk::FrameListener* API::getFrameListener()
    {
        return m_FrameListener;
    }
    Dusk::Console* API::getConsole()
    {
        return m_Console;
    }
    Dusk::Dispatcher* API::getDispatcher()
    {
        return m_Dispatcher;
    }
    Dusk::InputSystemBinding* API::getInputSystemBinding()
    {
        return m_InputSystemBinding;
    }
    Dusk::InputSystemEditor* API::getInputSystemEditor()
    {
        return m_InputSystemEditor;
    }
    Dusk::Scene* API::getScene()
    {
        return m_Scene;
    }
}
