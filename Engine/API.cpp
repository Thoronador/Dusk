#include "API.h"
#include "Application.h"

namespace Dusk
{
    API::API(Dusk::Application* app)
    {
        //ctor
        m_Application = app;

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
    API::~API()
    {
        //dtor
    }
    void API::setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr)
    {
        m_Root = root;
        m_Camera = camera;
        m_Window = window;
        m_SceneManager = mgr;
    }
}
