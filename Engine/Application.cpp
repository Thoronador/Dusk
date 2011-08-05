/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 ssj5000, walljumper, DaSteph, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Application.h"
#include "InputSystem.h"
#include "Scene.h"
#include "Dispatcher.h"
#include "API.h"
#include "Camera.h"
#include "Sound.h"
#include "Settings.h"
#include "DuskFunctions.h"
#include "Messages.h"
#include <OgreTexture.h>
#include <OgreRenderTexture.h>

namespace Dusk
{
    /**
    *Constructor of the Application class
    *
    */
    Application::Application()
            : m_Root(0)
    {
        //ctor
        getAPI().setApplication(this);
        //get selected screenshot file format
        if (Settings::getSingleton().getSetting_string("ScreenshotFormat", "PNG")=="JPEG")
        {
          m_ScreenshotType = ssfJPEG;
        }
        else
        {
          m_ScreenshotType = ssfPNG;
        }
    }

    Application::~Application()
    {
        if(m_Root) delete m_Root;
    }

    /**
    *@return if initialisation of the ogre core <br> - true initialisation successfull  <br> - false init failed
    */
    bool Application::initialise(const std::string& pluginFileName)
    {
        DuskLog() << "Plugin file: "<<pluginFileName<<"\n";
        m_Root = new Ogre::Root(pluginFileName);

        // add resource locations
        addResourceLocations();

        // if we cannot initialise Ogre, just abandon the whole deal
        if ( !initOgreCore() ) return false;

        createSceneManager();
        createCamera();
        createViewports();
        //Register Ogre Objects in API class
        getAPI().setOgreObjects(m_Root,m_Window,m_SceneManager);
        // Set default mipmap level (NB some APIs ignore this)
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // Create any resource listeners (for loading screens)
        createResourceListener();

        // Initialise resources
        initResources();

        // Create the scene
        Scene::get().createScene();

        // Create the FrameListener
        createFrameListener();

        //Initialize Input
        InputSystem::initializeInput(m_Window, m_Root);

        //Initialize Soundsystem
        if (Sound::get().init())
        {
          DuskLog() << "Soundsystem successfully initialised.\n";
          std::string device_name;
          if (Sound::get().getCurrentDeviceName(device_name))
          {
            DuskLog() << "Current output device is \""<<device_name<<"\".\n";
            std::vector<std::string> devices;
            if (Sound::get().getAvailableDevices(devices))
            {
              DuskLog() << "Available devices:\n";
              unsigned int i;
              for (i=0; i<devices.size(); ++i)
              {
                DuskLog() << "  " << devices[i]<<"\n";
              }
              DuskLog() <<devices.size()<< " audio device(s) in total.\n";
            }
          }
        }
        else
        {
          DuskLog() << "ERROR: Soundsystem could not be initialised properly.\n";
        }

        return true;
    }

    void Application::go(const std::string& pluginFileName)
    {
        if (!initialise(pluginFileName))
            return;
        m_Root->startRendering();

        /* clean up is already done by quit command */
        //Scene::get().destroyScene();
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
        Camera::getSingleton().setupCamera(m_SceneManager);
        m_Camera = Camera::getSingleton().getOgreCamera();
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
        m_FrameListener = new Dusk::FrameListener();
        m_Root->addFrameListener(m_FrameListener);
    }
//-------------------------------------------------------------------------------------
    Dusk::FrameListener* Application::getFrameListener()
    {
        return m_FrameListener;
    }
//-------------------------------------------------------------------------------------
    bool Application::createScreenshot() const
    {
      //We can't make screenshots without a window.
      if (m_Window==NULL or m_Camera==NULL) return false;

      const unsigned int screenIndex = Settings::getSingleton().getSetting_uint("ScreenshotIndex", 0);

      Ogre::TexturePtr ptrTexture = Ogre::TextureManager::getSingleton().createManual(
          "ScreenTex"+IntToString(screenIndex), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D, m_Window->getWidth(), m_Window->getHeight(), 0,
          Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
      Ogre::RenderTexture *renderTexture = ptrTexture->getBuffer()->getRenderTarget();

      renderTexture->addViewport(m_Camera);
      renderTexture->getViewport(0)->setClearEveryFrame(true);
      //set background colour to colour - important for fog
      renderTexture->getViewport(0)->setBackgroundColour(m_Window->getViewport(0)->getBackgroundColour());
      //renderTexture->getViewport(0)->setOverlaysEnabled(false);
      renderTexture->update();
      const std::string screenPrefix = Settings::getSingleton().getSetting_string("ScreenshotPrefix", "Screenshot");
      //select proper file format
      switch (m_ScreenshotType)
      {
        case ssfJPEG:
             renderTexture->writeContentsToFile(screenPrefix+IntToString(screenIndex)+".jpg");
             break;
        case ssfPNG:
             renderTexture->writeContentsToFile(screenPrefix+IntToString(screenIndex)+".png");
             break;
      }//switch
      Settings::getSingleton().addSetting_uint("ScreenshotIndex", screenIndex+1);
      Ogre::TextureManager::getSingleton().remove("ScreenTex"+IntToString(screenIndex));
      return true;
    }

} //namespace
