#include "EditorApplication.h"

EditorApplication::EditorApplication()
{
  mFrameListener = 0;
  mRoot = 0;
}

EditorApplication::~EditorApplication()
{
  if (mFrameListener)
      delete mFrameListener;
  if (mRoot)
      delete mRoot;
}

void EditorApplication::go(void)
{
  if (!setup())
    return;

  mRoot->startRendering();

  // clean up
  destroyScene();
}

bool EditorApplication::setup(void)
{
  Ogre::String pluginsPath;
  // only use plugins.cfg if not static
  #ifndef OGRE_STATIC_LIB
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      pluginsPath = "plugins-windows.cfg";
    #else
      pluginsPath = "plugins-linux.cfg";
    #endif
  #endif

  mRoot = new Root(pluginsPath, "ogre.cfg", "Ogre.log");

  setupResources();

  bool carryOn = configure();
  if (!carryOn) return false;

  createSceneManager();
  createCamera();
  createViewports();

  // Set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // Create any resource listeners (for loading screens)
  createResourceListener();
  // Load resources
  loadResources();

  // Create the scene
  createScene();

  createFrameListener();

  return true;
}

bool EditorApplication::configure(void)
{
  if(mRoot->showConfigDialog())
  {
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    mWindow = mRoot->initialise(true);
    return true;
  }
  else
  {
    return false;
  }
}

void EditorApplication::createSceneManager(void)
{
  // create an Ogre::SceneManager
  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "EditorSceneManager");
}

void EditorApplication::createCamera(void)
{
  mCamera = mSceneMgr->createCamera("EditorCam");
  // Position it at 500 in Z direction
  mCamera->setPosition(Ogre::Vector3(0,0,500));
  // Look back along -Z
  mCamera->lookAt(Ogre::Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);
}

void EditorApplication::createFrameListener(void)
{
  mFrameListener= new EditorFrameListener(mWindow, mCamera);
  mFrameListener->showDebugOverlay(true);
  mRoot->addFrameListener(mFrameListener);
}

void EditorApplication::destroyScene(void)
{
  //empty
}

void EditorApplication::createViewports(void)
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  //alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void EditorApplication::setupResources(void)
{
  std::cout << "DEBUG: entered setupResources()\n";
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

  Ogre::String secName, typeName, archName;
  while (seci.hasMoreElements())
  {
    secName = seci.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i)
    {
      typeName = i->first;
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
              archName, typeName, secName);
    }
  }
  std::cout << "DEBUG: left setupResources()\n";
}

void EditorApplication::createResourceListener(void)
{
  //empty
}

void EditorApplication::loadResources(void)
{
  // Initialise, parse scripts etc
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
