#include "EditorApplication.h"
#include "EditorCamera.h"
#include "../Engine/DuskConstants.h"
#include "../Engine/DataLoader.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/Landscape.h"
#include "../Engine/ObjectData.h"

#if defined(_WIN32)
  //Windows includes go here
  #include <io.h>
#else
  //Linux directory entries
  #include <dirent.h>
#endif

namespace Dusk
{

const CEGUI::colour cSelectionColour = CEGUI::colour(65.0f/255.0f, 105.0f/255.0f, 225.0f/255.0f, 0.5f);

std::vector<FileEntry> get_DirectoryFileList(const std::string Directory)
{
  std::vector<FileEntry> result;
  FileEntry one;
  #if defined(_WIN32)
  //Windows part
  intptr_t handle;
  struct _finddata_t sr;
  sr.attrib = _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID |
              _A_SUBDIR | _A_ARCH;
  handle = _findfirst(std::string(Directory+"*").c_str(),&sr);
  if (handle == -1)
  {
    std::cout << "Dusk::getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }
  //search it
  while( _findnext(handle, &sr)==0)
  {
    one.FileName = std::string(sr.name);
    one.IsDirectory = ((sr.attrib & _A_SUBDIR)==_A_SUBDIR);
    result.push_back(one);
  }//while
  _findclose(handle);
  #else
  //Linux part
  DIR * direc = opendir(Directory.c_str());
  if (direc == NULL)
  {
    std::cout << "Dusk::getDirectoryFileList: ERROR: unable to open directory "
              <<"\""<<Directory<<"\". Returning empty list.\n";
    return result;
  }//if

  struct dirent* entry = readdir(direc);
  while (entry != NULL)
  {
    one.FileName = std::string(entry->d_name);
    one.IsDirectory = entry->d_type==DT_DIR;
    //check for socket, pipes, block device and char device, which we don't want
    if (entry->d_type != DT_SOCK && entry->d_type != DT_FIFO && entry->d_type != DT_BLK
        && entry->d_type != DT_CHR)
    {
      result.push_back(one);
      entry = readdir(direc);
    }
  }//while
  closedir(direc);
  #endif
  return result;
}//function


EditorApplication::EditorApplication()
{
  mFrameListener = 0;
  mRoot = 0;
  mSystem = 0;
  mRenderer = 0;
  LoadedDataFile = "";
  LoadFrameDirectory = "."+path_sep;
  LoadFrameFiles.clear();
  ID_of_object_to_delete = "";
  popup_pos_x = 0.0f;
  popup_pos_y = 0.0f;
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
  EditorCamera::GetSingleton().setupCamera(mSceneMgr);

  /*
  mCamera = mSceneMgr->createCamera("EditorCam");
  // Position it at 500 in Z direction
  mCamera->setPosition(Ogre::Vector3(0,0,500));
  // Look back along -Z
  mCamera->lookAt(Ogre::Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);
  */
}

void EditorApplication::createFrameListener(void)
{
  mFrameListener= new EditorFrameListener(mWindow, EditorCamera::GetSingleton().getOgreCamera(), true, true);
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
  //CreateCEGUIMenu();
  CreateCEGUIMenuBar();
  CreateCEGUICatalogue();
  CreatePopupMenus();
}

void EditorApplication::destroyScene(void)
{
  //empty
}

void EditorApplication::createViewports(void)
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport(EditorCamera::GetSingleton().getOgreCamera());
  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
  //alter the camera aspect ratio to match the viewport
  EditorCamera::GetSingleton().getOgreCamera()->setAspectRatio(
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
  mSystem->setGUISheet(sheet);
}

void EditorApplication::CreateCEGUIMenuBar(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

  CEGUI::Menubar *menu = static_cast<CEGUI::Menubar*> (wmgr.createWindow("TaharezLook/Menubar", "Editor/MenuBar"));
  menu->setSize(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.05, 0)));
  menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.00, 0)));
  menu->setInheritsAlpha(false);

  CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().getWindow("Editor/Root");
  sheet->addChildWindow(menu);


  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/File"));
  menu_item->setText("File");
  menu_item->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.8, 0)));
  menu_item->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.10, 0)));
  menu->addChildWindow(menu_item);
  //menu_item->openPopupMenu

  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/File/PopUp"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/File/PopUp/Load"));
  menu_item->setText("Load");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::LoadButtonClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/File/PopUp/Save"));
  menu_item->setText("Save");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::SaveButtonClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/File/PopUp/Quit"));
  menu_item->setText("Quit");
  popup->addItem(menu_item);

  //"mode" menu
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode"));
  menu_item->setText("Mode");
  menu_item->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.8, 0)));
  menu_item->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.10, 0)));
  menu->addChildWindow(menu_item);

  popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/Mode/PopUp"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Move"));
  menu_item->setText("Free Movement");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeMoveClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Land"));
  menu_item->setText("Landscape Editing");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeLandClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Cata"));
  menu_item->setText("Catalogue");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeListClicked, this));
  popup->addItem(menu_item);

  CEGUI::Window* mode_indicator = wmgr.createWindow("TaharezLook/StaticText", "Editor/ModeIndicator");
  mode_indicator->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
  mode_indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.00, 0)));
  mode_indicator->setText("M: Catalogue");
  mode_indicator->setInheritsAlpha(false);
  sheet->addChildWindow(mode_indicator);
}

void EditorApplication::CreateCEGUICatalogue(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  CEGUI::FrameWindow* catalogue = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/Catalogue"));
  catalogue->setSize(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.9, 0)));
  catalogue->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.05, 0)));
  catalogue->setText("Object Catalogue");
  catalogue->setTitleBarEnabled(true);
  catalogue->setCloseButtonEnabled(false);
  catalogue->setFrameEnabled(true);
  catalogue->setSizingEnabled(false);
  catalogue->setInheritsAlpha(false);
  winmgr.getWindow("Editor/Root")->addChildWindow(catalogue);

  CEGUI::TabControl * tab = static_cast<CEGUI::TabControl*> (winmgr.createWindow("TaharezLook/TabControl", "Editor/Catalogue/Tab"));
  tab->setSize(CEGUI::UVector2(CEGUI::UDim(0.96, 0), CEGUI::UDim(0.90, 0)));
  tab->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02, 0), CEGUI::UDim(0.05, 0)));
  //tab->setText("Object Catalogue");
  catalogue->addChildWindow(tab);

  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Object");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Objects");
  tab->addTab(pane);
  //add the grid
  CEGUI::MultiColumnList *mcl = NULL;
  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Object/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.48, 0));
  mcl->addColumn("Mesh", 1, CEGUI::UDim(0.48, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplication::ObjectTabClicked, this));

  //add some random data
  addObjectRecordToCatalogue("The_ID", "flora/Oak.mesh");
  addObjectRecordToCatalogue("static_seat", "YetAnother.mesh");

  //Item tab
  pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Item");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Items");
  tab->addTab(pane);

  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Item/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Value", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Weight", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 4, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);

  //sample data
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem("apple");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem("Fresh Apple");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem("5");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem("0.2");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem("food/golden_delicious.mesh");
  mcl->setItem(lbi, 4, row);
}

void EditorApplication::CreatePopupMenus(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

  //PopUp Menu for static objects' tab
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/ObjectPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/New"));
  menu_item->setText("New object...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ObjectNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/Edit"));
  menu_item->setText("Edit selected object...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ObjectEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/Delete"));
  menu_item->setText("Delete selected object");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ObjectDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Object/List")->addChildWindow(popup);
  popup->closePopupMenu();
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

    //listbox to show all files
    CEGUI::Listbox* FileBox = static_cast<CEGUI::Listbox*> (winmgr.createWindow("TaharezLook/Listbox", "Editor/LoadFrame/Listbox"));
    FileBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.65, 0)));
    FileBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.2, 0)));
    FileBox->setMultiselectEnabled(false);
    FileBox->setSortingEnabled(true);
    FileBox->subscribeEvent(CEGUI::Listbox::EventMouseDoubleClick,
            CEGUI::Event::Subscriber(&EditorApplication::LoadFrameOKClicked, this));
    frame->addChildWindow(FileBox);
    UpdateLoadWindowFiles(LoadFrameDirectory);
  }
  frame->setVisible(true);
  frame->setAlwaysOnTop(true);
}

void EditorApplication::UpdateLoadWindowFiles(const std::string Directory)
{
    LoadFrameDirectory = Directory;
    LoadFrameFiles = get_DirectoryFileList(Directory);
    unsigned int i;
    std::cout << "Files in directory \""<<Directory<<"\":\n";
    for (i=0; i<LoadFrameFiles.size(); i++)
    {
      std::cout << "  "<<LoadFrameFiles.at(i).FileName<<"; directory: ";
      if (LoadFrameFiles.at(i).IsDirectory)
      {
        std::cout <<"yes\n";
      }
      else
      {
        std::cout << "no\n";
      }
    }//for
    std::cout << "total: "<< LoadFrameFiles.size() <<"\n\n";

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

    //listbox to show all files
    CEGUI::Listbox* FileBox = static_cast<CEGUI::Listbox*> (winmgr.getWindow("Editor/LoadFrame/Listbox"));
    // --- clear previous entries
    FileBox->resetList();
    // --- add file names from vector
    CEGUI::ListboxTextItem* lbi = NULL;
    for (i=0; i<LoadFrameFiles.size(); i++)
    {
      lbi = new CEGUI::ListboxTextItem(LoadFrameFiles.at(i).FileName);
      FileBox->addItem(lbi);
    }//for
    FileBox->requestRedraw();
}

void EditorApplication::RefreshObjectList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = NULL;
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Object/List"))
  {
    showWarning("ERROR: Could not find ojbet list window in Window Manager!");
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Object/List"));
  mcl->resetList();

  std::map<std::string, std::string>::iterator first;
  std::map<std::string, std::string>::iterator end;
  first = ObjectBase::GetSingleton().GetFirst();
  end = ObjectBase::GetSingleton().GetEnd();
  while (first != end)
  {
    addObjectRecordToCatalogue(first->first, first->second);
    first++;
  }//while
  return;
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
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/WarningFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create OK button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/WarningFrame/OK");
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

void EditorApplication::showHint(const std::string hint_text)
{
  if (hint_text=="")
  {
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/HintFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/HintFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/HintFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Information");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //add static label for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/HintFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);
    //create OK button
    CEGUI::Window* button;
    button = winmgr.createWindow("TaharezLook/Button", "Editor/HintFrame/OK");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::HintFrameOKClicked, this));
  }
  winmgr.getWindow("Editor/HintFrame/Label")->setText(hint_text);
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.25, 0)));
  frame->setAlwaysOnTop(true);
}

void EditorApplication::showObjectNewWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ObjectNewFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ObjectNewFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("New Object...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    CEGUI::Window * button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ObjectNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for mesh path
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ObjectNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //buttons at bottom
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectNewFrameOKClicked, this));
    frame->addChildWindow(button);

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplication::showObjectEditWindow(void)
{
  if (ID_of_object_to_edit=="")
  {
    std::cout << "ObjectEditWindow: No ID given.\n";
    return;
  }

  if (!ObjectBase::GetSingleton().hasObject(ID_of_object_to_edit))
  {
    std::cout << "ObjectEditWindow: Object not present in database.\n";
    showWarning("There seems to be no object with the ID \""+ID_of_object_to_edit
                +"\". Aborting.");
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ObjectEditFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ObjectEditFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Edit Object...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    CEGUI::Window * button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectEditFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectEditFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ObjectEditFrame/ID_Edit");
    button->setText(ID_of_object_to_edit);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for mesh path
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ObjectEditFrame/Mesh_Edit");
    button->setText(ObjectBase::GetSingleton().GetMeshName(ID_of_object_to_edit,false));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //buttons at bottom
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectEditFrame/Save");
    button->setText("Save");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectEditFrameSaveClicked, this));
    frame->addChildWindow(button);

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplication::showObjectConfirmDeleteWindow(void)
{

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/ObjectDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ObjectDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ObjectDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Object...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static label for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/ObjectDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    textbox->setText("Do you really want to delete the object \""+ID_of_object_to_delete+"\" and all of its references?");
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplication::showObjectEditConfirmIDChangeWindow(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow*  frame = NULL;

  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ConfirmObjectIDChangeFrame"));
  }
  else
  {
    //create it
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ConfirmObjectIDChangeFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Rename Object?");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox",
                                                        "Editor/ConfirmObjectIDChangeFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    textbox->setText("The ID of this object has changed.\nDo you want to rename the object \""
                     +ID_of_object_to_edit+"\" to \">insert new ID here<\" or create a new one?");
    frame->addChildWindow(textbox);

    if (winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
    {
      textbox->setText("The ID of this object has changed.\nDo you want to rename the object \""
                   +ID_of_object_to_edit+"\" to \""
                   +winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText()
                   +"\" or create a new one?");
    }

    //buttons: New, Rename, Cancel
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/New");
    button->setText("New Object");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.06, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectConfirmIDChangeNewClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/Rename");
    button->setText("Rename Object");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectConfirmIDChangeRenameClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectConfirmIDChangeCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

//for catalogue:

void EditorApplication::addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData)
{
  CEGUI::MultiColumnList* mcl = NULL;
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Item/List"))
  {
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Item/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionColours(cSelectionColour);
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(ItemData.Name);
  lbi->setSelectionColours(cSelectionColour);
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(ItemData.weight));
  lbi->setSelectionColours(cSelectionColour);
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(ItemData.value));
  lbi->setSelectionColours(cSelectionColour);
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(ItemData.Mesh);
  lbi->setSelectionColours(cSelectionColour);
  mcl->setItem(lbi, 4, row);
}

void EditorApplication::addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh)
{
  CEGUI::MultiColumnList* mcl = NULL;
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Object/List"))
  {
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionColours(cSelectionColour);
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(Mesh);
  lbi->setSelectionColours(cSelectionColour);
  mcl->setItem(lbi, 1, row);
}

//callbacks for buttons

bool EditorApplication::LoadButtonClicked(const CEGUI::EventArgs &e)
{
  showCEGUILoadWindow();
  return true;
}

bool EditorApplication::SaveButtonClicked(const CEGUI::EventArgs &e)
{
  //did we load a file previously?
  if (LoadedDataFile =="")
  {
    showHint("You have not loaded any file yet, thus we cannot save to that file.");
    return true;
  }
  //save it
  if (DataLoader::GetSingleton().SaveToFile(LoadedDataFile, ALL_BITS))
  {
    showHint("File successfully saved!");
    return true;
  }//if
  showWarning("Failed to save data to file.\nThe resulting file could be damaged and thus not processable for the Editor and the game.");
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
    if (PathToFile == ".")
    { //we don't need to change to current directory
      return true;
    }
    //find file in list
    unsigned int i, idx;
    idx = LoadFrameFiles.size() +1;
    for (i=0; i<LoadFrameFiles.size(); i++)
    {
      if (LoadFrameFiles.at(i).FileName == PathToFile)
      {
        idx = i;
        break;
      }
    }//for
    //check for directory
    if (LoadFrameFiles.at(i).IsDirectory)
    {
      //update path and quit
      UpdateLoadWindowFiles(LoadFrameDirectory+PathToFile+path_sep);
      return true;
    }
    //no directory, but file chosen -> load it
    // --- clear previously loaded data
    DataLoader::GetSingleton().ClearData(ALL_BITS);
    LoadedDataFile = "";
    ID_of_object_to_delete = "";
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
             (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Item/List"));
    mcl->resetList();
    mcl = static_cast<CEGUI::MultiColumnList*>
             (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
    mcl->resetList();
    // --- load file
    if (!(DataLoader::GetSingleton().LoadFromFile(LoadFrameDirectory+PathToFile)))
    {
      showWarning( "Error while loading data from file \""+PathToFile+"\"! "
                  +"Clearing all loaded data to avoid unexpected behaviour.");
      //clear stuff that might have been loaded successfully before error
      DataLoader::GetSingleton().ClearData(ALL_BITS);
      Landscape::GetSingleton().RemoveFromEngine(mSceneMgr);
      return true;
    }
    showHint("File \""+PathToFile+"\" was successfully loaded into editor!");
    LoadedDataFile = LoadFrameDirectory+PathToFile;
    //  --- close window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LoadFrame");
    //  --- show data in grid
    //  **** still needs to be implemented ****

    // --- items
    std::cout << "DEBUG: item iterators...\n";
    std::map<std::string, ItemRecord>::iterator anfang;
    std::map<std::string, ItemRecord>::iterator ende;
    anfang = ItemBase::GetSingleton().GetFirst();
    ende = ItemBase::GetSingleton().GetEnd();
    while (anfang != ende)
    {
      addItemRecordToCatalogue(anfang->first, anfang->second);
      anfang++;
    }//while

    // --- objects
    std::cout << "DEBUG: object iterators...\n";
    std::map<std::string, std::string>::iterator one;
    std::map<std::string, std::string>::iterator two;
    one = ObjectBase::GetSingleton().GetFirst();
    two = ObjectBase::GetSingleton().GetEnd();
    while (one != two)
    {
      addObjectRecordToCatalogue(one->first, one->second);
      one++;
    }//while

    //  --- make loaded stuff visible via Ogre
    std::cout << "DEBUG: SendToEngine...\n";
    Landscape::GetSingleton().SendToEngine(mSceneMgr);
    std::cout << "DEBUG: EnableAllObjects...\n";
    ObjectData::GetSingleton().EnableAllObjects(mSceneMgr);
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

bool EditorApplication::HintFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/HintFrame"))
  {
    winmgr.destroyWindow("Editor/HintFrame");
  }
  return true;
}

bool EditorApplication::ObjectTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //maybe we should provide checks here
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/ObjectPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    std::cout << "Debug: MouseEvent data:\n"
              << "  position: x: "<<mea.position.d_x<<"; y: "<<mea.position.d_y<<"\n"
              << "  button: ";
    switch (mea.button)
    {
      case CEGUI::LeftButton: std::cout << "left\n"; break;
      case CEGUI::RightButton: std::cout << "right\n"; break;
      default: std::cout << "other\n"; break;
    }//swi
    if (mea.button == CEGUI::RightButton)
    {
      float pu_x, pu_y;
      CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Object/List")->getPixelRect();
      pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      //std::cout << "Debug: pu position: x: "<<pu_x<<"; y: "<<pu_y<<"\n";
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
      //save cursor position for later use
      popup_pos_x = mea.position.d_x;
      popup_pos_y = mea.position.d_y;
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

bool EditorApplication::ObjectNewClicked(const CEGUI::EventArgs &e)
{
  /* not implemented */
  showObjectNewWindow();
  return true;
}

bool EditorApplication::ObjectEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
  CEGUI::ListboxItem* lb_item = mcl->getFirstSelectedItem();
  if (lb_item==NULL)
  {
    std::cout << "Debug: No item selected.\n";
    return true;
  }

  //Debug
  std::cout << "DEBUG: selection colours:\n  top left:"
            << lb_item->getSelectionColours().d_top_left.getRed() << "; "
            << lb_item->getSelectionColours().d_top_left.getGreen() << "; "
            << lb_item->getSelectionColours().d_top_left.getBlue() << "; "
            << lb_item->getSelectionColours().d_top_left.getAlpha() << "\n"

            << lb_item->getSelectionColours().d_top_right.getRed() << "; "
            << lb_item->getSelectionColours().d_top_right.getGreen() << "; "
            << lb_item->getSelectionColours().d_top_right.getBlue() << "; "
            << lb_item->getSelectionColours().d_top_right.getAlpha() << "\n"

            << lb_item->getSelectionColours().d_bottom_left.getRed() << "; "
            << lb_item->getSelectionColours().d_bottom_left.getGreen() << "; "
            << lb_item->getSelectionColours().d_bottom_left.getBlue() << "; "
            << lb_item->getSelectionColours().d_bottom_left.getAlpha() << "\n"

            << lb_item->getSelectionColours().d_bottom_right.getRed() << "; "
            << lb_item->getSelectionColours().d_bottom_right.getGreen() << "; "
            << lb_item->getSelectionColours().d_bottom_right.getBlue() << "; "
            << lb_item->getSelectionColours().d_bottom_right.getAlpha() << "\n";
  //DEBUG end

  unsigned int row_index = mcl->getItemRowIndex(lb_item);
  lb_item = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));

  ID_of_object_to_edit = std::string(lb_item->getText().c_str());
  showObjectEditWindow();
  return true;
}

bool EditorApplication::ObjectDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Object/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select an object from the list to delete it.");
  }
  else
  {
    unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_object_to_delete = std::string(lbi->getText().c_str());
    showObjectConfirmDeleteWindow();
  }
  return true;
}

bool EditorApplication::ObjectNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  return true;
}

bool EditorApplication::ObjectNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    CEGUI::Editbox* id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ObjectNewFrame/ID_Edit"));
    CEGUI::Editbox* mesh_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ObjectNewFrame/Mesh_Edit"));
    //make sure we have some data
    if (id_edit->getText()=="")
    {
      showWarning("You have to enter an ID string to create a new object!");
      return true;
    }
    if (mesh_edit->getText()=="")
    {
      showWarning("You have to enter a mesh path to create a new object!");
      return true;
    }

    //check for presence of object with same ID
    if (ObjectBase::GetSingleton().hasObject(std::string(id_edit->getText().c_str())))
    {
      showWarning("An Object with the given ID already exists.");
      return true;
    }

    //finally add it to ObjectBase
    ObjectBase::GetSingleton().addObject(std::string(id_edit->getText().c_str()), std::string(mesh_edit->getText().c_str()));
    //update catalogue
    addObjectRecordToCatalogue(std::string(id_edit->getText().c_str()), std::string(mesh_edit->getText().c_str()));
    //destroy window
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  return true;
}

bool EditorApplication::ObjectEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
  }
  return true;
}

bool EditorApplication::ObjectEditFrameSaveClicked(const CEGUI::EventArgs &e)
{
  //not yet implemented

  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Editbox* id_edit;
  CEGUI::Editbox* mesh_edit;

  if (!winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit") ||
      !winmgr.isWindowPresent("Editor/ObjectEditFrame/Mesh_Edit"))
  {
    showWarning("Error: Editbox(es) for ID and/or mesh is/are not registered at window manager!");
    return true;
  }//if
  id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit"));
  mesh_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ObjectEditFrame/Mesh_Edit"));

  if (std::string(id_edit->getText().c_str())=="")
  {
    showHint("You have to enter an ID for this object!");
    return true;
  }
  if (std::string(mesh_edit->getText().c_str())=="")
  {
    showHint("You have to enter a mesh path for this object!");
    return true;
  }
  if (std::string(id_edit->getText().c_str())!=ID_of_object_to_edit)
  {
    //ID was changed
   showObjectEditConfirmIDChangeWindow();
   return true;
  }
  //check if mesh has remained the same
  if (std::string(mesh_edit->getText().c_str())==ObjectBase::GetSingleton().GetMeshName(ID_of_object_to_edit))
  {
    showHint("You have not changed the data of this object, thus there are no changes to be saved.");
    return true;
  }

  //save it
  ObjectBase::GetSingleton().addObject(std::string(id_edit->getText().c_str()),
                                     std::string(mesh_edit->getText().c_str()));
  //update list
  RefreshObjectList();
  //update shown objects
  ObjectData::GetSingleton().reenableReferencesOfObject(ID_of_object_to_edit, mSceneMgr);
  //delete window
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
  }
  ID_of_object_to_edit = "";
  return true;
}

bool EditorApplication::ObjectDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectDeleteFrame");
  }
  return true;
}

bool EditorApplication::ObjectDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  //not yet implemented
  if (ID_of_object_to_delete == "")
  {
    showWarning("Error: object ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ObjectDeleteFrame");
    return true;
  }
  if (!ObjectBase::GetSingleton().deleteObject(ID_of_object_to_delete))
  {
    showHint("ObjectBase class holds no object of the given ID ("
             +ID_of_object_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ObjectDeleteFrame");
    return true;
  }
  unsigned int refs_deleted = ObjectData::GetSingleton().deleteReferencesOfObject(ID_of_object_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Object \""+ID_of_object_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Object \""+ID_of_object_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of objects
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_object_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_object_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ObjectDeleteFrame");
  return true;
}

bool EditorApplication::ModeMoveClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_Movement);
  CEGUI::WindowManager::getSingleton().getWindow("Editor/ModeIndicator")->setText("Mode: Movement");
  return true;
}

bool EditorApplication::ModeLandClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_Landscape);
  CEGUI::WindowManager::getSingleton().getWindow("Editor/ModeIndicator")->setText("Mode: Landscape");
  return true;
}

bool EditorApplication::ModeListClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_Lists);
  CEGUI::WindowManager::getSingleton().getWindow("Editor/ModeIndicator")->setText("Mode: Catalogue");
  return true;
}

bool EditorApplication::ObjectConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
    //**** does not work as intended yet ****
    /* should normally be handled seperately;
       Rename also includes disabling all DuskObjects with the previous ID,
       change their ID and enable it with new ID.
       plus: it should delete row with old ID from catalogue and add new row.
       */
    ID_of_object_to_edit = std::string(winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText().c_str());
    ObjectEditFrameSaveClicked(e);
  }
  return true;
}

bool EditorApplication::ObjectConfirmIDChangeNewClicked(const CEGUI::EventArgs &e)
{
  //**** not implemented yet ****
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
  {
    //winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
    //**** does not work as intended yet ****
    /* should normally be handled seperately;
       Creation of new object also includes row updates in catalogue.
       */
  }
  return true;
}

bool EditorApplication::ObjectConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
  }
  return true;
}

}//namespace
