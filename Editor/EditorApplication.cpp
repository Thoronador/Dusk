#include "EditorApplication.h"
#include "../Engine/DuskFunctions.h"

namespace Dusk
{

EditorApplication::EditorApplication()
{
  mFrameListener = 0;
  mRoot = 0;
  mSystem = 0;
  mRenderer = 0;
}

EditorApplication::~EditorApplication()
{
  if (mSystem)
      delete mSystem;

  if (mRenderer)
      delete mRenderer;

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

  mRoot = new Ogre::Root(pluginsPath, "ogre.cfg", "Ogre.log");

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
  mFrameListener= new EditorFrameListener(mWindow, mCamera, true, true);
  mFrameListener->showDebugOverlay(true);
  mRoot->addFrameListener(mFrameListener);
}

void EditorApplication::createScene(void)
{
  mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
  mSystem = new CEGUI::System(mRenderer);

  CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
  mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
  mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

  CreateCEGUIRootWindow();
  CreateCEGUIMenu();

  //just for test purposes we add a MultiColumnList and fill it with some entries
  CEGUI::WindowManager& win = CEGUI::WindowManager::getSingleton();

  CEGUI::MultiColumnList *mcl = NULL;
  mcl = static_cast<CEGUI::MultiColumnList*> (win.createWindow("TaharezLook/MultiColumnList", "Editor/ObjectBaseList"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.85, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.1, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.25, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.25, 0));
  mcl->addColumn("Mesh", 2, CEGUI::UDim(0.25, 0));
  mcl->addColumn("Something", 3, CEGUI::UDim(0.25, 0));

  CEGUI::ListboxItem *lbi;
  uint row;

  lbi = new CEGUI::ListboxTextItem("The_ID");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem("Not a Tree");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem("flora/Oak.mesh");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem("500");
  mcl->setItem(lbi, 3, row);

  lbi = new CEGUI::ListboxTextItem("static_seat");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem("Iron Chair");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem("YetAnother.mesh");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem("50");
  mcl->setItem(lbi, 3, row);

  CEGUI::Window * sheet = win.getWindow("Editor/Root");
  mcl->setInheritsAlpha(false);
  sheet->addChildWindow(mcl);

  mSystem->setGUISheet(sheet);
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

//CEGUI-based stuff
void EditorApplication::CreateCEGUIRootWindow(void)
{
  CEGUI::WindowManager *winmgr = CEGUI::WindowManager::getSingletonPtr();
  CEGUI::Window *sheet = winmgr->createWindow("DefaultGUISheet", "Editor/Root");
  sheet->setAlpha(0.0);
  sheet->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  sheet->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
}

void EditorApplication::CreateCEGUIMenu(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

  CEGUI::Window *button = wmgr.createWindow("TaharezLook/Button", "Editor/LoadButton");
  button->setText("Load");
  button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
  button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  button->setInheritsAlpha(false);
  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().getWindow("Editor/Root");
  sheet->addChildWindow(button);
  button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::LoadButtonClicked, this));

  button = wmgr.createWindow("TaharezLook/Button", "Editor/SaveButton");
  button->setText("Save");
  button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
  button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.05, 0)));
  button->setInheritsAlpha(false);
  sheet->addChildWindow(button);

  button = wmgr.createWindow("TaharezLook/Button", "Editor/QuitButton");
  button->setText("Quit");
  button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
  button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.1, 0)));
  button->setInheritsAlpha(false);
  sheet->addChildWindow(button);
}

void EditorApplication::showCEGUILoadWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/LoadFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/LoadFrame"));
  }
  else
  {
    //create it
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/LoadFrame"));
    frame->setInheritsAlpha(false);
    frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.75, 0)));
    frame->setTitleBarEnabled(true);
    frame->setText("Load file...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //create buttons
    // Button "OK" - will load selected file (not implemented yet)
    CEGUI::Window *button = winmgr.createWindow("TaharezLook/Button", "Editor/LoadFrame/OKButton");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.06667, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.9, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::LoadFrameOKClicked, this));

    // Button "Cancel" - closes 'window', i.e. frame
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LoadFrame/CancelButton");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.06667, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.9, 0)));
    frame->addChildWindow(button);

    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::LoadFrameCancelClicked, this));

    //static text field
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LoadFrame/Label");
    button->setText("Files in directory:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.06667, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
    frame->addChildWindow(button);

    //to do: add files in directory to list and display them

    //debug: shows all files
    std::vector<FileEntry> file_list = getDirectoryFileList("./");
    unsigned int i;
    std::cout << "Files in directory:\n";
    for (i=0; i<file_list.size(); i++)
    {
      std::cout << file_list.at(i).FileName<<"; directory: ";
      if (file_list.at(i).IsDirectory)
      {
        std::cout <<"yes\n";
      }
      else
      {
        std::cout << "no\n";
      }
    }//for
    std::cout << "total: "<< file_list.size() <<"\n\n";

    //listbox to show all files
    CEGUI::Listbox* FileBox = static_cast<CEGUI::Listbox*> (winmgr.createWindow("TaharezLook/Listbox", "Editor/LoadFrame/Listbox"));
    FileBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.65, 0)));
    FileBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.2, 0)));
    FileBox->setMultiselectEnabled(false);
    FileBox->setSortingEnabled(false);
    frame->addChildWindow(FileBox);
    // --- add file names from vector
    CEGUI::ListboxTextItem* lbi = NULL;
    for (i=0; i<file_list.size(); i++)
    {
      lbi = new CEGUI::ListboxTextItem(file_list.at(i).FileName);
      FileBox->addItem(lbi);
    }//for
  }
  frame->setVisible(true);
  frame->setAlwaysOnTop(true);
}

void EditorApplication::showWarning(const std::string Text_of_warning)
{
  if (Text_of_warning=="")
  {
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/WarningFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/WarningFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/WarningFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Warning!");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //add static label for message
    CEGUI::Window* button;
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WarningFrame/Label");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.65, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.05, 0)));
    frame->addChildWindow(button);
    //create OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WarningFrame/OK");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::WarningFrameOKClicked, this));
  }
  winmgr.getWindow("Editor/WarningFrame/Label")->setText(Text_of_warning);
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.25, 0)));
  frame->setAlwaysOnTop(true);
}

//callbacks for buttons

bool EditorApplication::LoadButtonClicked(const CEGUI::EventArgs &e)
{
  showCEGUILoadWindow();
  return true;
}

bool EditorApplication::LoadFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LoadFrame");
  return true;
}

bool EditorApplication::LoadFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::Listbox* FileBox = static_cast<CEGUI::Listbox*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/LoadFrame/Listbox"));
  CEGUI::ListboxItem* lbi = FileBox->getFirstSelectedItem();
  if (lbi==NULL)
  {
    //no item selected
    showWarning("You have not selected a file which shall be loaded.");
    return true;
  }
  else
  {
    //we have a selected item, load it...
    std::string PathToFile = std::string(lbi->getText().c_str());
    //  --- close window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LoadFrame");
    //  --- clear all previously loaded data
    //  --- try to load file via Dusk::DataLoader
    //  **** still needs to be implemented ****
  }//else branch
  return true;
}

bool EditorApplication::WarningFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WarningFrame"))
  {
    winmgr.destroyWindow("Editor/WarningFrame");
  }
  return true;
}

}//namespace
