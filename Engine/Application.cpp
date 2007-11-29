#include "Application.h"

#include "InputSystemEditor.h"

namespace Dusk
{
    /**
    *Contructor of the Application class
    *
    */
    Application::Application()
            : m_Root(0)
    {
        //ctor
    }

    Application::~Application()
    {
        delete m_Root;
    }
    /**
    *@return if initialisation of the ogre core <br> - true initialisation successfull  <br> - false init failed
    */
    bool Application::initialise(std::string pluginFileName)
    {
        std::cout << pluginFileName;
        m_Root = new Ogre::Root(pluginFileName);

        // add resource locations
        addResourceLocations();

        // if we cannot initialise Ogre, just abandon the whole deal
        if ( !initOgreCore() ) return false;

        createSceneManager();
        createCamera();
        createViewports();

        // Set default mipmap level (NB some APIs ignore this)
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // Create any resource listeners (for loading screens)
        createResourceListener();

        // Initialise resources
        initResources();

        // Create the scene
        createScene();

        // Create the FrameListener
        createFrameListener();

        return true;
    }
    void Application::go(std::string pluginFileName)
    {
        if (!initialise(pluginFileName))
            return;
        m_Root->startRendering();

        // clean up
        destroyScene();
    }

    void Application::addResourceLocations()
    {
        // Load resource paths from config file
        Ogre::ConfigFile cf;
        cf.load("resources.cfg");

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        std::string secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
    }

//-------------------------------------------------------------------------------------
    bool Application::initOgreCore()
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        if (m_Root->restoreConfig() || m_Root->showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            m_Window = m_Root->initialise(true);
            return true;
        }
        else
        {
            return false;
        }
    }

//-------------------------------------------------------------------------------------
    void Application::createSceneManager()
    {
        // Create the SceneManager, in this case a generic one
        m_SceneManager = m_Root->createSceneManager(Ogre::ST_GENERIC);
    }

//-------------------------------------------------------------------------------------
    void Application::createCamera()
    {
        // Create the camera
        m_Camera = m_SceneManager->createCamera("PlayerCam");

        // Position it at 500 in Z direction
        m_Camera->setPosition(Ogre::Vector3(0,0,500));
        // Look back along -Z
        m_Camera->lookAt(Ogre::Vector3(0,0,-300));
        m_Camera->setNearClipDistance(5);
    }

//-------------------------------------------------------------------------------------
    void Application::createViewports()
    {
        // Create one viewport, entire window
        Ogre::Viewport* vp = m_Window->addViewport(m_Camera);
        vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        m_Camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    }

//-------------------------------------------------------------------------------------
    void Application::createResourceListener()
    {

    }

//-------------------------------------------------------------------------------------
    void Application::initResources()
    {
        // Initialise, parse scripts etc
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

//-------------------------------------------------------------------------------------
    void Application::destroyScene()
    {
    }

//-------------------------------------------------------------------------------------
    void Application::createFrameListener()
    {
        m_FrameListener = new Dusk::FrameListener(m_Window,m_Camera,m_Root,false);
        m_Root->addFrameListener(m_FrameListener);
    }
}
