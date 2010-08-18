#include "EditorApplication.h"
#include "EditorApplicationBase.h"
#include "EditorCamera.h"
#include "../Engine/API.h"
#include "../Engine/DuskConstants.h"
#include "../Engine/DataLoader.h"
#include "../Engine/Journal.h"
#include "../Engine/Landscape.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/ObjectBase.h"
#include "../Engine/ItemBase.h"
#include "../Engine/Weather.h"
#include "../Engine/InjectionManager.h"
#include <OgreVector3.h>

namespace Dusk
{

const float cRotationFactor = 2.5f;
const float cMovementFactor = 3.5f;
const float cTerraformDelta = 1.25f;

//helps us to locate a item within column lists
CEGUI::ListboxItem * getLbItemAtPoint(const CEGUI::Point& pt, CEGUI::MultiColumnList* mcl)
{
  if (mcl == NULL)
  {
    return NULL;
  }

  CEGUI::Rect listArea(mcl->getListRenderArea());
  if (!listArea.isPointInRect(pt))
  {
    return NULL;
  }
  float y = listArea.d_top - mcl->getVertScrollbar()->getScrollPosition();

  for (unsigned int i = 0; i < mcl->getRowCount(); ++i)
  {
    y += mcl->getHighestRowItemHeight(i);
    if (pt.d_y < y)
    {
      return mcl->getItemAtGridReference(CEGUI::MCLGridRef(i,0));
    }
  }
  return NULL;
}



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
  ID_of_item_to_delete = "";
  ID_of_object_to_edit = "";
  ID_of_item_to_edit = "";
  ID_of_light_to_delete = "";
  ID_of_light_to_edit = "";
  ID_of_NPC_to_delete = "";
  ID_of_quest_to_delete = "";
  ID_of_quest_to_rename = "";
  ID_of_quest_to_add_entry = "";
  Index_of_entry_to_delete = 0;
  QuestID_of_entry_to_delete = "";
  Index_of_entry_to_edit = 0;
  QuestID_of_entry_to_edit = "";
  LandscapeColour.red = 254;
  LandscapeColour.green = 128;
  LandscapeColour.blue = 0;

  mouse.LeftButton.down = mouse.LeftButton.up = CEGUI::Point(0.0f, 0.0f);
  mouse.RightButton.down = mouse.RightButton.up = CEGUI::Point(0.0f, 0.0f);
  mouse_object = edit_object = NULL;

  getAPI().setApplication(this);
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

  getAPI().setApplication(NULL);
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

  getAPI().setOgreObjects(mRoot, mWindow, mSceneMgr);

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

  sheet->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&EditorApplication::RootMouseDown, this));
  sheet->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplication::RootMouseUp, this));
  sheet->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&EditorApplication::RootMouseMove, this));
  sheet->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&EditorApplication::RootDoubleClicked, this));

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

  //"file" menu
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
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/File/PopUp/Stats"));
  menu_item->setText("Statistics");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::StatsButtonClicked, this));
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
  menu_item->setText("Landscape Editing >");
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Cata"));
  menu_item->setText("Catalogue");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeListClicked, this));
  popup->addItem(menu_item);

  //add landscape submenues
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.getWindow("Editor/MenuBar/Mode/PopUp/Land"));
  popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/Mode/PopUp/Land/PopUp2"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Land/PopUp2/Colour"));
  menu_item->setText("Edit Colours");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeLandColourClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Land/PopUp2/Up"));
  menu_item->setText("Terraform: Up");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeLandUpClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Mode/PopUp/Land/PopUp2/Down"));
  menu_item->setText("Terraform: Down");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::ModeLandDownClicked, this));
  popup->addItem(menu_item);

  //"quests" menu
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Quests"));
  menu_item->setText("Quests");
  menu_item->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.8, 0)));
  menu_item->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.10, 0)));
  menu->addChildWindow(menu_item);

  popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/Quests/PopUp"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Quests/PopUp/Journal"));
  menu_item->setText("Journal");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::JournalClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Quests/PopUp/Dialogue"));
  menu_item->setText("Dialogue (not implemented yet)");
  popup->addItem(menu_item);

  //"weather" menu
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Weather"));
  menu_item->setText("Weather");
  menu_item->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.8, 0)));
  menu_item->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.10, 0)));
  menu->addChildWindow(menu_item);

  popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/Weather/PopUp"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Weather/PopUp/Fog"));
  menu_item->setText("Toggle fog");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::WeatherToggleClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Weather/PopUp/Rain"));
  menu_item->setText("Toggle rain");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::WeatherToggleClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Weather/PopUp/Snow"));
  menu_item->setText("Toggle snow");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::WeatherToggleClicked, this));
  popup->addItem(menu_item);

  //"Extra" menu
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Extra"));
  menu_item->setText("Extra");
  menu_item->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.8, 0)));
  menu_item->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.10, 0)));
  menu->addChildWindow(menu_item);

  popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/MenuBar/Extra/PopUp"));
  menu_item->setPopupMenu(popup);

  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Extra/PopUp/WireFrame"));
  menu_item->setText("Enable wire frame landscape");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::WireframeToggleClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/MenuBar/Extra/PopUp/NoWireFrame"));
  menu_item->setText("Disable wire frame landscape");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplication::WireframeToggleClicked, this));
  popup->addItem(menu_item);


  //static text to show mode
  CEGUI::Window* mode_indicator = wmgr.createWindow("TaharezLook/StaticText", "Editor/ModeIndicator");
  mode_indicator->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.05, 0)));
  mode_indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.00, 0)));
  mode_indicator->setText("Mode: Catalogue");
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
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplication::ItemTabClicked, this));

  //sample data
  ItemRecord ir;
  ir.Name = "Fresh Apple";
  ir.value = 5;
  ir.weight = 0.2;
  ir.Mesh = "food/golden_delicious.mesh";
  addItemRecordToCatalogue("apple", ir);
  ItemBase::GetSingleton().addItem("apple", ir);

  //Light tab
  pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Light");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Lights");
  tab->addTab(pane);

  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Light/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Red", 1, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Green", 2, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Blue", 3, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Radius", 4, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Type", 5, CEGUI::UDim(0.16, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplication::LightTabClicked, this));

  //sample data
  LightBase::GetSingleton().addLight("light_red", LightRecord::GetRed(123.4));
  LightBase::GetSingleton().addLight("light_green", LightRecord::GetGreen(23.4));
  LightBase::GetSingleton().addLight("light_blue", LightRecord::GetBlue(3.4));
  RefreshLightList();

  //NPC tab
  pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/NPC");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("NPCs");
  tab->addTab(pane);

  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/NPC/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Level", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Female", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 4, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplication::NPCTabClicked, this));

  //Sample data
  // ---- no data yet, just refresh the list
  RefreshNPCList();
}

void EditorApplication::CreatePopupMenus(void)
{
  //PopUp Menu for static objects' tab
  CreatePopupMenuObjectTab();

  //PopUp Menu for items' tab
  CreatePopupMenuItemTab();

  //PopUp Menu for lights' tab
  CreatePopupMenuLightTab();

  //PopUp Menu for NPCs' tab
  CreatePopupMenuNPCTab();
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
    // Button "OK" - will load selected file
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
    for (i=0; i<LoadFrameFiles.size(); ++i)
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
      lbi->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
      FileBox->addItem(lbi);
    }//for
    FileBox->requestRedraw();
}

void EditorApplication::ClearCatalogue(void)
{
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
             (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Item/List"));
  mcl->resetList();
  mcl = static_cast<CEGUI::MultiColumnList*>
           (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
  mcl->resetList();
  mcl = static_cast<CEGUI::MultiColumnList*>
           (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Light/List"));
  mcl->resetList();
  mcl = static_cast<CEGUI::MultiColumnList*>
           (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/NPC/List"));
  mcl->resetList();
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

bool EditorApplication::StatsButtonClicked(const CEGUI::EventArgs &e)
{
  showHint( "Current statistics:\n  Landscape records: "
           + IntToString(Landscape::GetSingleton().RecordsAvailable())+"\n"
           +"  Object records: "  + IntToString(ObjectBase::GetSingleton().NumberOfObjects())+"\n"
           +"  Items: " + IntToString(ItemBase::GetSingleton().NumberOfItems())+"\n"
           +"  Lights: " + IntToString(LightBase::GetSingleton().NumberOfLights())+"\n"
           +"    Object, Light & Item references: "+ IntToString(ObjectManager::GetSingleton().NumberOfReferences())
           +"\n  NPCs: " + IntToString(NPCBase::GetSingleton().NumberOfNPCs())
           +"\n    Animated Object & NPC references: "+ IntToString(InjectionManager::GetSingleton().NumberOfReferences())
           +"\n  Journal:\n"
           +"    quests: "+ IntToString(Journal::GetSingleton().NumberOfDistinctQuests())
           +"\n    entries: "+ IntToString(Journal::GetSingleton().NumberOfEntries()), true);
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
    closeAllEditWindows();
    LoadedDataFile = "";
    ID_of_object_to_delete = "";
    mouse_object = edit_object = NULL;
    ClearCatalogue();
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
    // show data in grid
    // --- items
    RefreshItemList();
    // --- objects
    RefreshObjectList();
    // --- lights
    RefreshLightList();

    //  --- make loaded stuff visible via Ogre
    std::cout << "DEBUG: SendToEngine...\n";
    Landscape::GetSingleton().SendToEngine(mSceneMgr, false);
    std::cout << "DEBUG: EnableAllObjects...\n";
    ObjectManager::GetSingleton().EnableAllObjects(mSceneMgr);
    EditorCamera::GetSingleton().resetToOrigin();
  }//else branch
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
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Object/List")->getPixelRect();
      const float pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      const float pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

bool EditorApplication::ItemTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/ItemPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Item/List")->getPixelRect();
      const float pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      const float pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

bool EditorApplication::LightTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/LightPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Light/List")->getPixelRect();
      const float pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      const float pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

bool EditorApplication::ModeMoveClicked(const CEGUI::EventArgs &e)
{
  LandscapeFrameFinishClicked(e);
  mFrameListener->setEditorMode(EM_Movement);
  SetCatalogueVisibility(true);
  return true;
}

bool EditorApplication::ModeLandUpClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_LandscapeUp);
  SetCatalogueVisibility(false);
  showLandscapeEditWindow();
  return true;
}

bool EditorApplication::ModeLandDownClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_LandscapeDown);
  SetCatalogueVisibility(false);
  showLandscapeEditWindow();
  return true;
}

bool EditorApplication::ModeLandColourClicked(const CEGUI::EventArgs &e)
{
  mFrameListener->setEditorMode(EM_LandscapeColour);
  SetCatalogueVisibility(false);
  showLandscapeEditWindow();
  return true;
}

bool EditorApplication::ModeListClicked(const CEGUI::EventArgs &e)
{
  LandscapeFrameFinishClicked(e);
  mFrameListener->setEditorMode(EM_Lists);
  SetCatalogueVisibility(true);
  return true;
}

void EditorApplication::closeAllEditWindows(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //frame window for new lights
  if (winmgr.isWindowPresent("Editor/LightNewFrame"))
  {
    winmgr.destroyWindow("Editor/LightNewFrame");
  }
  //frame window for new items
  if (winmgr.isWindowPresent("Editor/ItemNewFrame"))
  {
    winmgr.destroyWindow("Editor/ItemNewFrame");
  }
  //frame window for new objects
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  //frame window for editing lights
  if (winmgr.isWindowPresent("Editor/LightEditFrame"))
  {
    winmgr.destroyWindow("Editor/LightEditFrame");
  }
  //frame window for editing items
  if (winmgr.isWindowPresent("Editor/ItemEditFrame"))
  {
    winmgr.destroyWindow("Editor/ItemEditFrame");
  }
  //frame window for editing objects
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
  }
  //frame for deleting items
  if (winmgr.isWindowPresent("Editor/ItemDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ItemDeleteFrame");
  }
  //frame for deleting Lights
  if (winmgr.isWindowPresent("Editor/LightDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/LightDeleteFrame");
  }
  //frame for deleting objects
  if (winmgr.isWindowPresent("Editor/ObjectDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectDeleteFrame");
  }
  //frame to change ID of objects
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
  }
  //frame to change ID of items
  if (winmgr.isWindowPresent("Editor/ConfirmItemIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmItemIDChangeFrame");
  }
  //frame to change ID of lights
  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmLightIDChangeFrame");
  }
  //frame for editing object references
  if (winmgr.isWindowPresent("Editor/ObjectReference"))
  {
    winmgr.destroyWindow("Editor/ObjectReference");
  }
  //frame for journal editing
  if (winmgr.isWindowPresent("Editor/JournalFrame"))
  {
    winmgr.destroyWindow("Editor/JournalFrame");
  }
  //frame for confirming quest deletion
  if (winmgr.isWindowPresent("Editor/JournalDeleteQuestFrame"))
  {
    winmgr.destroyWindow("Editor/JournalDeleteQuestFrame");
  }
  //frame for creating new quest entries
  if (winmgr.isWindowPresent("Editor/NewQuestEntryFrame"))
  {
    winmgr.destroyWindow("Editor/NewQuestEntryFrame");
  }
  //frame for editing quest entries
  if (winmgr.isWindowPresent("Editor/EditQuestEntryFrame"))
  {
    winmgr.destroyWindow("Editor/EditQuestEntryFrame");
  }
  //frame for renaming quests
  if (winmgr.isWindowPresent("Editor/QuestRenameFrame"))
  {
    winmgr.destroyWindow("Editor/QuestRenameFrame");
    ID_of_quest_to_rename = "";
  }
  //frame for deleting NPCs
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/NPCDeleteFrame");
  }
  //frame for adding items to NPC inventory
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
  }
}

bool EditorApplication::RootMouseDown(const CEGUI::EventArgs &e)
{
  mouse_object = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MouseEventArgs& mouse_ea = static_cast<const CEGUI::MouseEventArgs&> (e);
  std::cout << "DEBUG: mouse down,  pos: x: "<<mouse_ea.position.d_x << "; y: "
              << mouse_ea.position.d_y << "\n";
  CEGUI::Window * child = winmgr.getWindow("Editor/Root")->getChildAtPosition(mouse_ea.position);
  if ( child != NULL )
  { //got something
    std::cout << "       Window: "<<child->getName().c_str()<< "\n";
    return true;
  }//if
  std::cout << "       Window: NONE\n";

  switch (mouse_ea.button)
  {
    case CEGUI::LeftButton:
         mouse.LeftButton.down = mouse_ea.position;
         break;
    case CEGUI::RightButton:
         mouse.RightButton.down = mouse_ea.position;
         break;
    default: break;
  }//swi

  //check for entity / object at mouse position
  if (mouse_ea.button == CEGUI::LeftButton or mouse_ea.button == CEGUI::RightButton)
  {
    mouse_object = GetObjectAtMouse(mouse_ea.position);
  }//if buttons down
  return true;
}

bool EditorApplication::RootMouseUp(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MouseEventArgs& mouse_ea = static_cast<const CEGUI::MouseEventArgs&> (e);
  std::cout << "DEBUG: mouse up, pos: x: "<<mouse_ea.position.d_x << "; y: "
            << mouse_ea.position.d_y << "\n";
  mouse_object = NULL;
  //get window at mouse cursor
  CEGUI::Window * child = winmgr.getWindow("Editor/Root")->getChildAtPosition(mouse_ea.position);
  if ( child != NULL )
  { //got something
    std::cout << "       Window: "<< child->getName().c_str()<< "\n";
    return true;
  }//if
  std::cout << "       Window: NONE\n";

  if (mouse_ea.button == CEGUI::LeftButton)
  {
    //now handle event
    mouse.LeftButton.up = mouse_ea.position;
    //Left CTRL pressed? -> place object
    if (mFrameListener->IsKeyDown(OIS::KC_LCONTROL))
    {
      //get tab and active pane
      CEGUI::TabControl* tab = static_cast<CEGUI::TabControl*> (winmgr.getWindow("Editor/Catalogue/Tab"));
      CEGUI::Window* wnd = tab->getTabContentsAtIndex(tab->getSelectedTabIndex());
      std::cout << "Debug: Got window named \""<<wnd->getName()<<"\"\n";
      //now get the list
      CEGUI::MultiColumnList* mcl = NULL;
      if (winmgr.isWindowPresent(wnd->getName()+"/List"))
      {
        mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow(wnd->getName()+"/List"));
      }//if
      else
      {
        std::cout << "Debug: no Window with the name \""<< wnd->getName()+"/List" << "\" found.\n";
        return true;
      }
      if (mcl->getName() != "Editor/Catalogue/Tab/Object/List" &&
          mcl->getName() != "Editor/Catalogue/Tab/Light/List" &&
          mcl->getName() != "Editor/Catalogue/Tab/Item/List")
      {
        std::cout << "Debug: List \""<< mcl->getName()+"/List" << "\" found, "
                  << "but that's not the Object, Light or Item list.\n";
        return true;
      }

      //CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Object/List"));
      CEGUI::ListboxItem * lbi = NULL;
      lbi = mcl->getFirstSelectedItem();
      if (lbi != NULL)
      {
        //we got something, i.e. user wants to place an object
        std::cout << "DEBUG: placing new referenced object/ light/ item of ID \""
                  <<std::string(lbi->getText().c_str())<<"\"\n";
        ObjectTypes PlaceType = otUndefined;
        //bool PlaceLight = (mcl->getName() == "Editor/Catalogue/Tab/Light/List");
        if (mcl->getName() == "Editor/Catalogue/Tab/Light/List")
        {
          PlaceType = otLight;
        }
        else if (mcl->getName() == "Editor/Catalogue/Tab/Item/List")
        {
           PlaceType = otItem;
        }
        else
        {
           PlaceType = otStatic;
        }

        if (PlaceType==otLight)
        { //place a light
          if (!LightBase::GetSingleton().hasLight(std::string(lbi->getText().c_str())))
          {
            showWarning("There is no Light with the ID \""
                        +std::string(lbi->getText().c_str())+"\", thus you can't "
                        +"place it.");
            return true;
          }
        }
        else if (PlaceType==otStatic)
        { //place an object
          if (!ObjectBase::GetSingleton().hasObject(std::string(lbi->getText().c_str())))
          {
            showWarning("There is no Object with the ID \""
                        +std::string(lbi->getText().c_str())+"\", thus you can't "
                        +"place it.");
            return true;
          }
        }
        else
        { //place an item
          if (!ItemBase::GetSingleton().hasItem(std::string(lbi->getText().c_str())))
          {
            showWarning("There is no Item with the ID \""
                        +std::string(lbi->getText().c_str())+"\", thus you can't "
                        +"place it.");
            return true;
          }
        }

        Ogre::Quaternion quat = EditorCamera::GetSingleton().getOrientation();
        DuskObject* temp = NULL;
        if (PlaceType==otStatic)
        {
          temp =
          ObjectManager::GetSingleton().addObjectReference(std::string(lbi->getText().c_str()),
                     EditorCamera::GetSingleton().getPosition() + quat*Ogre::Vector3(0.0f, 0.0f, -100.0f),
                     Ogre::Vector3::ZERO, 1.0f);
        }
        else if (PlaceType==otLight)
        {
          temp =
          ObjectManager::GetSingleton().addLightReference(std::string(lbi->getText().c_str()),
                     EditorCamera::GetSingleton().getPosition() + quat*Ogre::Vector3(0.0f, 0.0f, -100.0f));
        }
        else
        {
          temp =
          ObjectManager::GetSingleton().addItemReference( std::string(lbi->getText().c_str()),
                     EditorCamera::GetSingleton().getPosition() + quat*Ogre::Vector3(0.0f, 0.0f, -100.0f),
                     Ogre::Vector3::ZERO, 1.0f);
        }
        temp->Enable(mSceneMgr);
      }
      else
      {
        std::cout << "DEBUG: no selected ListboxItem found.\n";
      }
    }//if left CTRL down
  }//if left button
  else if (mouse_ea.button == CEGUI::RightButton)
  {
    mouse.RightButton.up = mouse_ea.position;
  }
  return true;
}

bool EditorApplication::RootMouseMove(const CEGUI::EventArgs &e)
{
  //not completely implemented yet
  const CEGUI::MouseEventArgs& mouse_ea = static_cast<const CEGUI::MouseEventArgs&> (e);
  if (mFrameListener->IsMouseDown(OIS::MB_Left) or mFrameListener->IsMouseDown(OIS::MB_Right))
  {
    //we are trying to move (left mouse)/rotate (right mouse) an object here
    std::cout << "DEBUG: mouse delta: x: " << mouse_ea.moveDelta.d_x<<", y: "
              << mouse_ea.moveDelta.d_y<<"\n";
    //Do we have an object, which we can move or rotate? Are we in catalogue mode?
    if (mouse_object != NULL and mFrameListener->getEditorMode()==EM_Lists)
    {
      if (mFrameListener->IsMouseDown(OIS::MB_Right))
      { //rotate it
        Ogre::Vector3 temp = Ogre::Vector3(0.0f, 0.0f, 0.0f);
        if (mFrameListener->IsKeyDown(OIS::KC_X))
        { //only x-axis
          temp.x = cRotationFactor * mouse_ea.moveDelta.d_y;
        }
        else if (mFrameListener->IsKeyDown(OIS::KC_Y))
        { //only y-axis
          temp.y = cRotationFactor * mouse_ea.moveDelta.d_x;
        }
        else if (mFrameListener->IsKeyDown(OIS::KC_Z))
        { //only z-axis
          temp.z = cRotationFactor * mouse_ea.moveDelta.d_x;
        }
        else  //any axis
        {
          temp.x = cRotationFactor * mouse_ea.moveDelta.d_y;
          temp.y = cRotationFactor * mouse_ea.moveDelta.d_x;
        }
        mouse_object->SetRotation(mouse_object->GetRotation() + temp);
        return true;
      }//if
      else
      { //move it
          // not implemented yet
          if (mFrameListener->getEditorMode() != EM_Movement)
          {
            Ogre::Vector3 vec(0.0f, 0.0f, 0.0f);
            vec.x = cMovementFactor*mouse_ea.moveDelta.d_x;
            vec.y = -cMovementFactor*mouse_ea.moveDelta.d_y;
            vec = EditorCamera::GetSingleton().getOrientation() * vec;
            std::cout << "vec: "<<vec<<"\n";
            mouse_object->SetPosition(mouse_object->GetPosition()+vec);
          }
          // not implemented yet
      }//else
    }//if mouse_object present and Catalogue mode
  }//if buttons (left | right) down

  //change landscape colours, or perform terraforming activities
  if (mFrameListener->IsMouseDown(OIS::MB_Left) and (mFrameListener->getEditorMode()==EM_LandscapeColour
      or mFrameListener->getEditorMode()==EM_LandscapeUp or mFrameListener->getEditorMode()==EM_LandscapeDown))
  {
    //perform ray scene query
    Ogre::Ray pickRay;
    Ogre::RaySceneQuery * rsc_query = NULL;
    pickRay = EditorCamera::GetSingleton().getOgreCamera()->getCameraToViewportRay(
              mouse_ea.position.d_x/mWindow->getWidth(), mouse_ea.position.d_y/mWindow->getHeight());
    rsc_query = mSceneMgr->createRayQuery(pickRay);
    rsc_query->setRay(pickRay);
    rsc_query->setSortByDistance(true);
    //perform query
    Ogre::RaySceneQueryResult &result = rsc_query->execute();
    Ogre::RaySceneQueryResult::iterator rsq_iter = result.begin();

    if ( rsq_iter!=result.end())
    { //found something
      while (rsq_iter!=result.end())
      {
        if (rsq_iter->movable!=NULL)
        {
          //found movable object
          std::cout << "DEBUG: found movable \"" <<rsq_iter->movable->getName()
                    <<"\" in a distance of "<< rsq_iter->distance <<" units.\n";
          if (LandscapeRecord::IsLandscapeRecordName(rsq_iter->movable->getName()))
          {
            //landscape record found
            LandscapeRecord* land_rec = NULL;
            Ogre::Vector3 vec_i = pickRay.getPoint(rsq_iter->distance);
            land_rec = Landscape::GetSingleton().GetRecordAtXZ(vec_i.x, vec_i.z);
            if (land_rec!= NULL)
            {
              if (land_rec->IsHitByRay(pickRay, vec_i))
              {
                switch(mFrameListener->getEditorMode())
                {
                  case EM_LandscapeColour:
                       land_rec->SetColour(vec_i.x, vec_i.z, LandscapeColour.red,
                                    LandscapeColour.green, LandscapeColour.blue);
                       break;
                  case EM_LandscapeUp:
                       land_rec->Terraform(vec_i.x, vec_i.z, cTerraformDelta);
                       break;
                  case EM_LandscapeDown:
                       land_rec->Terraform(vec_i.x, vec_i.z, -cTerraformDelta);
                       break;
                  default://should never happen, because of if-branch in line 2772
                       std::cout<<"EditorApplication::RootMouseMove: Programming "
                                <<"error, you should not be here.\n"; break;
                }//swi
                mSceneMgr->destroyQuery(rsc_query);
                return true;
              }//if record is hit by ray
            }//if record returned
          }//if object has Landscape name
        }//if movable object
        rsq_iter++;
      }//while
    }
    else
    {
      //nothing found
      std::cout << "DEBUG: Query result was empty.\n";
    }
    mSceneMgr->destroyQuery(rsc_query);
  }//if colour or terraform
  //not completely implemented yet
  return true;
}

bool EditorApplication::RootDoubleClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MouseEventArgs& mouse_ea = static_cast<const CEGUI::MouseEventArgs&> (e);
  std::cout << "DEBUG: mouse double, pos: x: "<<mouse_ea.position.d_x << "; y: "
            << mouse_ea.position.d_y << "\n";
  //get window at mouse cursor
  CEGUI::Window * child = winmgr.getWindow("Editor/Root")->getChildAtPosition(mouse_ea.position);
  if (child != NULL)
  { //got something
    std::cout << "       Window: "<< child->getName().c_str()<< "\n";
    return true;
  }//if
  showObjectReferenceEditWindow(mouse_ea.position);
  return true;
}

bool EditorApplication::ObjectListMouseDown(const CEGUI::EventArgs &e)
{
  const CEGUI::MouseEventArgs& mouse_ea = static_cast<const CEGUI::MouseEventArgs&> (e);
  if (mouse_ea.button == CEGUI::LeftButton)
  {
    mouse.LeftButton.down = mouse_ea.position;
  }
  return true;
}

void EditorApplication::showObjectReferenceEditWindow(const CEGUI::Point& pt)
{
  DuskObject* target = GetObjectAtMouse(pt);
  if (target==NULL)
  { //no object found
    return;
  }
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::Window* element = NULL;
  CEGUI::Spinner* spin = NULL;
  if (winmgr.isWindowPresent("Editor/ObjectReference"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ObjectReference"));
  }
  else
  { //create window
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ObjectReference"));
    switch(target->GetType())
    {
      case otStatic:
           frame->setText("Object"); break;
      case otLight:
           frame->setText("Light"); break;
      case otContainer:
           frame->setText("Container"); break;
      case otItem:
           frame->setText("Item"); break;
      default:
           frame->setText("Object (undefined type)"); break;
    }//swi
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/ID");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    element->setText("ID: <not specified>");
    frame->addChildWindow(element);

    //static text for scale
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/ScaleLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.22, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    element->setText("3D Scale");
    frame->addChildWindow(element);
    //editbox for scale
    element = winmgr.createWindow("TaharezLook/Editbox", "Editor/ObjectReference/ScaleEdit");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.22, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.16, 0), CEGUI::UDim(0.1, 0)));
    element->setText("1.0(?)");
    frame->addChildWindow(element);

    //static text for position
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/PosLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.34, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Position");
    frame->addChildWindow(element);
    //static text for rotation
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/RotLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.34, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Rotation");
    frame->addChildWindow(element);

    //static text X
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/PosXLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.46, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("X");
    frame->addChildWindow(element);
    //static text Y
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/PosYLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.58, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Y");
    frame->addChildWindow(element);
    //static text Z
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/PosZLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.7, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Z");
    frame->addChildWindow(element);

    //static text X
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/RotXLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.46, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("X");
    frame->addChildWindow(element);
    //static text Y
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/RotYLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.58, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Y");
    frame->addChildWindow(element);
    //static text Z
    element = winmgr.createWindow("TaharezLook/StaticText", "Editor/ObjectReference/RotZLabel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.7, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Z");
    frame->addChildWindow(element);

    //spinner for position X
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/PosXSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.22, 0), CEGUI::UDim(0.46, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);
    //spinner for position Y
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/PosYSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.22, 0), CEGUI::UDim(0.58, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);
    //spinner for position Z
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/PosZSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.22, 0), CEGUI::UDim(0.7, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);

    //spinner for rotation X
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/RotXSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.67, 0), CEGUI::UDim(0.46, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);
    //spinner for rotation Y
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/RotYSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.67, 0), CEGUI::UDim(0.58, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);
    //spinner for rotation Z
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ObjectReference/RotZSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.67, 0), CEGUI::UDim(0.7, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.23, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setText("123.4");
    frame->addChildWindow(spin);

    //Save button
    element = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectReference/Save");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.85, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Save");
    frame->addChildWindow(element);
    element->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectReferenceEditSaveClicked, this));
    //Cancel button
    element = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectReference/Cancel");
    element->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.85, 0)));
    element->setSize(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.1, 0)));
    element->setText("Cancel");
    frame->addChildWindow(element);
    element->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::ObjectReferenceEditCancelClicked, this));

  }//else
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();
  //content
  winmgr.getWindow("Editor/ObjectReference/ID")->setText("ID: "+target->GetID());
  winmgr.getWindow("Editor/ObjectReference/ScaleEdit")->setText(FloatToString(target->GetScale()));
  Ogre::Vector3 vec = target->GetPosition();
  winmgr.getWindow("Editor/ObjectReference/PosXSpin")->setText(FloatToString(vec.x));
  winmgr.getWindow("Editor/ObjectReference/PosYSpin")->setText(FloatToString(vec.y));
  winmgr.getWindow("Editor/ObjectReference/PosZSpin")->setText(FloatToString(vec.z));
  vec = target->GetRotation();
  winmgr.getWindow("Editor/ObjectReference/RotXSpin")->setText(FloatToString(vec.x));
  winmgr.getWindow("Editor/ObjectReference/RotYSpin")->setText(FloatToString(vec.y));
  winmgr.getWindow("Editor/ObjectReference/RotZSpin")->setText(FloatToString(vec.z));
  edit_object = target;
}

bool EditorApplication::ObjectReferenceEditCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectReference"))
  {
    winmgr.destroyWindow("Editor/ObjectReference");
  }
  return true;
}

bool EditorApplication::ObjectReferenceEditSaveClicked(const CEGUI::EventArgs &e)
{
  if (edit_object == NULL)
  {
    showHint("No object was chosen! (Got NULL pointer.)");
    return true;
  }
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.getWindow("Editor/ObjectReference/ScaleEdit") &&
      winmgr.getWindow("Editor/ObjectReference/PosXSpin") &&
      winmgr.getWindow("Editor/ObjectReference/PosYSpin") &&
      winmgr.getWindow("Editor/ObjectReference/PosZSpin") &&
      winmgr.getWindow("Editor/ObjectReference/RotXSpin") &&
      winmgr.getWindow("Editor/ObjectReference/RotYSpin") &&
      winmgr.getWindow("Editor/ObjectReference/RotZSpin"))
  {
    float new_scale = StringToFloat( std::string(winmgr.getWindow("Editor/ObjectReference/ScaleEdit")->getText().c_str()), -1.0f);
    if (new_scale<=0.0f)
    {
      showWarning("The scaling factor has to be a valid floating point value and must not be zero or less!");
      return true;
    }
    Ogre::Vector3 pos, rot;
    CEGUI::Spinner* spin = NULL;
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/PosXSpin"));
    pos.x = spin->getCurrentValue();
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/PosYSpin"));
    pos.y = spin->getCurrentValue();
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/PosZSpin"));
    pos.z = spin->getCurrentValue();
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/RotXSpin"));
    rot.x = spin->getCurrentValue();
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/RotYSpin"));
    rot.y = spin->getCurrentValue();
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ObjectReference/RotZSpin"));
    rot.z = spin->getCurrentValue();
    //set the new values
    edit_object->SetPosition(pos);
    edit_object->SetRotation(rot);
    if (edit_object->IsEnabled())
    {
      edit_object->Disable();
      edit_object->SetScale(new_scale);
      edit_object->Enable(mSceneMgr);
    }
    else
    {
      edit_object->SetScale(new_scale);
    }
    winmgr.destroyWindow("Editor/ObjectReference");
    edit_object = NULL;
  }//if
  return true;
}

//scene query wrapper
DuskObject* EditorApplication::GetObjectAtMouse(const CEGUI::Point& pt)
{
  DuskObject* mo = NULL;

  Ogre::Ray pickRay;
  Ogre::RaySceneQuery * rsc_query = NULL;
  pickRay = EditorCamera::GetSingleton().getOgreCamera()->getCameraToViewportRay(
            pt.d_x/mWindow->getWidth(), pt.d_y/ mWindow->getHeight());
  rsc_query = mSceneMgr->createRayQuery(pickRay);
  rsc_query->setRay(pickRay);
  //perform query
  Ogre::RaySceneQueryResult &result = rsc_query->execute();
  Ogre::RaySceneQueryResult::iterator rsq_iter = result.begin();

  //hit something?
  if (rsq_iter != result.end())
  {
     //we got something
     while (rsq_iter != result.end() and (mo == NULL))
     {
       if (rsq_iter->movable!=NULL)
       {
         //found movable object
         std::cout << "DEBUG: found movable \"" <<rsq_iter->movable->getName()<<"\" in a distance of "
                   << rsq_iter->distance << " units.\n";
         if (rsq_iter->distance>0.1f)
         { //try moving/rotating it?
           if ( rsq_iter->movable->getUserObject() != NULL)
           {
              DuskObject * d_obj = static_cast<DuskObject*> (rsq_iter->movable->getUserObject());
              std::cout << "DEBUG: found object of ID \""<< d_obj->GetID() <<"\" at position ("
                        << d_obj->GetPosition().x<<","<< d_obj->GetPosition().y<<","
                        << d_obj->GetPosition().z<<")\n";
              std::cout << "       Rotation: V3("<< d_obj->GetRotation().x
                        <<","<< d_obj->GetRotation().y<<","<< d_obj->GetRotation().z<<")\n";
              mo = d_obj;
           }
         }
       }
       else
       {
         std::cout << "DEBUG: RSQ result: entity is not a movable.\n";
       }
       rsq_iter++;
     }//while
  }
  else
  {
    std::cout << "DEBUG: Query result was empty.\n";
    mo = NULL;
  }
  //destroy query object
  mSceneMgr->destroyQuery(rsc_query);
  return mo;
}

void EditorApplication::SetCatalogueVisibility(const bool visible)
{
  CEGUI::Window* win = CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue");
  win->setVisible(visible);
  if (visible)
  {
    win->moveToFront();
  }
}

void EditorApplication::showLandscapeEditWindow(void)
{
  //not completely implemented yet
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow*  frame = NULL;
  CEGUI::Spinner* spin = NULL;
  CEGUI::RadioButton* radio = NULL;

  if (winmgr.isWindowPresent("Editor/LandscapeFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/LandscapeFrame"));
  }
  else
  {
    //create it
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/LandscapeFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Landscape Editing");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    // -- radio buttons
    // ---- raise
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LandscapeFrame/TerraUp"));
    radio->setText("Raise");
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05, 0)));
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    radio->setGroupID(555);
    frame->addChildWindow(radio);
    radio->setSelected(true);
    radio->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameRadioButtonClicked, this));
    //---- lower
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LandscapeFrame/TerraDown"));
    radio->setText("Lower");
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05, 0)));
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.16, 0)));
    radio->setGroupID(555);
    frame->addChildWindow(radio);
    radio->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameRadioButtonClicked, this));
    //---- colour radio button
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LandscapeFrame/Colour"));
    radio->setText("Colour");
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.05, 0)));
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.22, 0)));
    radio->setGroupID(555);
    frame->addChildWindow(radio);
    radio->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameRadioButtonClicked, this));
    //--radius
    //---- static text
    CEGUI::Window* button = NULL;
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/RadiusLabel");
    button->setText("Radius:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //---- edit/spinner
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LandscapeFrame/RadiusSpin"));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setMinimumValue(1.0f);
    spin->setMaximumValue(200.0f); //Let's see, if these limits are appropriate.
    spin->setText("20.0");
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(spin);
    //--falloff
    //---- static text
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/FalloffLabel");
    button->setText("Falloff (%):");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(button);
    //---- spinner
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LandscapeFrame/FalloffSpin"));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setMinimumValue(0.0f);
    spin->setMaximumValue(100.0f);
    spin->setText("80");
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(spin);

    //--colour values
    //---- caption
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/ColourLabel");
    button->setText("Colour:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.3, 0)));
    frame->addChildWindow(button);
    //---- red label
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/RedLabel");
    button->setText("Red:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.125, 0), CEGUI::UDim(0.4, 0)));
    frame->addChildWindow(button);
    //---- green label
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/GreenLabel");
    button->setText("Green:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.125, 0), CEGUI::UDim(0.5, 0)));
    frame->addChildWindow(button);
    //---- blue label
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LandscapeFrame/BlueLabel");
    button->setText("Blue:");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.08, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.125, 0), CEGUI::UDim(0.6, 0)));
    frame->addChildWindow(button);
    //---red spinner
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LandscapeFrame/RedSpin"));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setMinimumValue(0.0f);
    spin->setMaximumValue(255.0f);
    spin->setText("255");
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
    spin->subscribeEvent(CEGUI::Spinner::EventValueChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameColourChanged, this));
    frame->addChildWindow(spin);
    //---green spinner
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LandscapeFrame/GreenSpin"));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setMinimumValue(0.0f);
    spin->setMaximumValue(255.0f);
    spin->setText("127");
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    spin->subscribeEvent(CEGUI::Spinner::EventValueChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameColourChanged, this));
    frame->addChildWindow(spin);
    //---blue spinner
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LandscapeFrame/BlueSpin"));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setMinimumValue(0.0f);
    spin->setMaximumValue(255.0f);
    spin->setText("0");
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.08, 0)));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.6, 0)));
    spin->subscribeEvent(CEGUI::Spinner::EventValueChanged,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameColourChanged, this));
    frame->addChildWindow(spin);

    //---- exit button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LandscapeFrame/Finish");
    button->setText("Exit Landscape Editing");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.8, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplication::LandscapeFrameFinishClicked, this));
  }//else
  //values
  //-- mode
  switch (mFrameListener->getEditorMode())
  {
    case EM_LandscapeUp:
         radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LandscapeFrame/TerraUp"));
         break;
    case EM_LandscapeDown:
         radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LandscapeFrame/TerraDown"));
         break;
    default: //EM_LandscapeColour
         radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LandscapeFrame/Colour"));
         break;
  }//switch
  radio->setSelected(true);
  //-- colour
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/RedSpin"));
  spin->setCurrentValue(LandscapeColour.red);
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/GreenSpin"));
  spin->setCurrentValue(LandscapeColour.green);
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/BlueSpin"));
  spin->setCurrentValue(LandscapeColour.blue);
  //positioning
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.6, 0)));
  frame->moveToFront();
  //not completely implemented yet
}

bool EditorApplication::LandscapeFrameFinishClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/LandscapeFrame"))
  {
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/RedSpin"));
    LandscapeColour.red = static_cast<unsigned char> (spin->getCurrentValue());
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/GreenSpin"));
    LandscapeColour.green = static_cast<unsigned char> (spin->getCurrentValue());
    spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LandscapeFrame/BlueSpin"));
    LandscapeColour.blue = static_cast<unsigned char> (spin->getCurrentValue());

    winmgr.destroyWindow("Editor/LandscapeFrame");
    ModeListClicked(e);
  }//if
  return true;
}

bool EditorApplication::LandscapeFrameRadioButtonClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowEventArgs& win_event = static_cast<const CEGUI::WindowEventArgs&> (e);
  if (win_event.window==NULL)
  {
    return true;
  }
  CEGUI::RadioButton* radio = static_cast<CEGUI::RadioButton*> (win_event.window);
  if (radio->isSelected())
  {
    if (radio->getName()=="Editor/LandscapeFrame/TerraUp")
    {
      mFrameListener->setEditorMode(EM_LandscapeUp);
    }
    else if (radio->getName()=="Editor/LandscapeFrame/TerraDown")
    {
      mFrameListener->setEditorMode(EM_LandscapeDown);
    }
    else //"Editor/LandscapeFrame/Colour"
    {
      mFrameListener->setEditorMode(EM_LandscapeColour);
    }//else
  }
  return true;
}

bool EditorApplication::LandscapeFrameColourChanged(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowEventArgs& winevent = static_cast<const CEGUI::WindowEventArgs&> (e);
  if (winevent.window!=NULL)
  {
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winevent.window);
    if (spin->getName() == "Editor/LandscapeFrame/RedSpin")
    {
      LandscapeColour.red = static_cast<unsigned char>(spin->getCurrentValue());
    }
    else if (spin->getName() == "Editor/LandscapeFrame/GreenSpin")
    {
      LandscapeColour.green = static_cast<unsigned char>(spin->getCurrentValue());
    }
    else if (spin->getName() == "Editor/LandscapeFrame/BlueSpin")
    {
      LandscapeColour.blue = static_cast<unsigned char>(spin->getCurrentValue());
    }
  } //window
  return true;
}

bool EditorApplication::JournalClicked(const CEGUI::EventArgs &e)
{
  showJournalWindow();
  return true;
}

bool EditorApplication::WeatherToggleClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowEventArgs& winevent = static_cast<const CEGUI::WindowEventArgs&> (e);
  if (winevent.window!=NULL)
  {
    if (winevent.window->getName() == "Editor/MenuBar/Weather/PopUp/Fog")
    {
      if (Weather::getSingelton().isFoggy())
      {
        Weather::getSingelton().stopFog();
      }
      else
      {
        Weather::getSingelton().setFogColour(0.8, 0.8, 0.8);
        Weather::getSingelton().startLinearFog(100, 400);
      }
    }
    else if (winevent.window->getName() == "Editor/MenuBar/Weather/PopUp/Rain")
    {
      if (Weather::getSingelton().isRaining())
      {
        Weather::getSingelton().stopRain();
      }
      else
      {
        Weather::getSingelton().startRain();
      }
    }
    else if (winevent.window->getName() == "Editor/MenuBar/Weather/PopUp/Snow")
    {
      if (Weather::getSingelton().isSnowing())
      {
        Weather::getSingelton().stopSnow();
      }
      else
      {
        Weather::getSingelton().startSnow();
      }
    }
  } //window
  return true;
}

bool EditorApplication::WireframeToggleClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowEventArgs& winevent = static_cast<const CEGUI::WindowEventArgs&> (e);
  if (winevent.window!=NULL)
  {
     mFrameListener->setWireFrame(winevent.window->getName()=="Editor/MenuBar/Extra/PopUp/WireFrame");
  }
  return true;
}

bool EditorApplication::NPCTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/NPCPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/NPC/List")->getPixelRect();
      const float pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      const float pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

}//namespace
