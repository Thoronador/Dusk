#include "DuskApplication.h"

DuskApplication::DuskApplication()
: m_Root(0)
{

}

DuskApplication::~DuskApplication()
{
    delete m_Root;
}

void DuskApplication::go()
{
   if (!initialise())
      return;

   m_Root->startRendering();

   // clean up
   destroyScene();
}


void Root::startRendering(void)
{
   assert(mActiveRenderer != 0);

   mActiveRenderer->_initRenderTargets();

   // Clear event times
   for(int i=0; i!=3; ++i)
      mEventTimes[i].clear();

   // Infinite loop, until broken out of by frame listeners
   // or break out by calling queueEndRendering()
   mQueuedEnd = false;

   while( !mQueuedEnd )
   {
      //Allow platform to pump/create/etc messages/events once per frame
      mPlatformManager->messagePump(mAutoWindow);

      if (!renderOneFrame())
         break;
   }
}

//-------------------------------------------------------------------------------------
bool DuskApplication::initialise()
{
   m_Root = new Root();

   // add resource locations
   addResourceLocations();

   // if we cannot initialise Ogre, just abandon the whole deal
   if ( !initOgreCore() ) return false;

   createSceneManager();
   createCamera();
   createViewports();

   // Set default mipmap level (NB some APIs ignore this)
   TextureManager::getSingleton().setDefaultNumMipmaps(5);

   // Create any resource listeners (for loading screens)
   createResourceListener();

   // Initialise resources
   initResources();

   // Create the scene
   createScene();

   return true;
};

//-------------------------------------------------------------------------------------
void DuskApplication::addResourceLocations()
{
   // Load resource paths from config file
   ConfigFile cf;
   cf.load("resources.cfg");

   // Go through all sections & settings in the file
   ConfigFile::SectionIterator seci = cf.getSectionIterator();

   String secName, typeName, archName;
   while (seci.hasMoreElements())
   {
      secName = seci.peekNextKey();
      ConfigFile::SettingsMultiMap *settings = seci.getNext();
      ConfigFile::SettingsMultiMap::iterator i;
      for (i = settings->begin(); i != settings->end(); ++i)
      {
         typeName = i->first;
         archName = i->second;
         ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
      }
   }
}

//-------------------------------------------------------------------------------------
bool DuskApplication::initOgreCore()
{
   // Show the configuration dialog and initialise the system
   // You can skip this and use root.restoreConfig() to load configuration
   // settings if you were sure there are valid ones saved in ogre.cfg
   if(m_Root->restoreConfig() || m_Root->showConfigDialog())
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
void DuskApplication::createSceneManager()
{
   // Create the SceneManager, in this case a generic one
   m_SceneMgr = m_Root->createSceneManager(ST_GENERIC);
}

//-------------------------------------------------------------------------------------
void DuskApplication::createCamera()
{
   // Create the camera
   m_Camera = m_SceneMgr->createCamera("PlayerCam");

   // Position it at 500 in Z direction
   m_Camera->setPosition(Vector3(0,0,500));
   // Look back along -Z
   m_Camera->lookAt(Vector3(0,0,-300));
   m_Camera->setNearClipDistance(5);
}

//-------------------------------------------------------------------------------------
void DuskApplication::createViewports()
{
   // Create one viewport, entire window
   Viewport* vp = m_Window->addViewport(m_Camera);
   vp->setBackgroundColour(ColourValue(0,0,0));

   // Alter the camera aspect ratio to match the viewport
   m_Camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

//-------------------------------------------------------------------------------------
void DuskApplication::createResourceListener()
{

}

//-------------------------------------------------------------------------------------
void DuskApplication::initResources()
{
   // Initialise, parse scripts etc
   ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//-------------------------------------------------------------------------------------
void DuskApplication::destroyScene()
{
}

