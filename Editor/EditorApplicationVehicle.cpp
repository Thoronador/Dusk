

#include "EditorApplicationVehicle.h"
#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>
#include "../Engine/VehicleBase.h"
#include "../Engine/InjectionManager.h"
#include "../Engine/DuskFunctions.h"

namespace Dusk
{

EditorApplicationVehicle::EditorApplicationVehicle()
{
  ID_of_vehicle_to_edit = "";
  ID_of_vehicle_to_delete = "";
}

EditorApplicationVehicle::~EditorApplicationVehicle()
{
  //empty
}

void EditorApplicationVehicle::createVehicleCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //Vehicle tab
  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Vehicle");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Vehicles");
  tab->addTab(pane);

  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                  (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Vehicle/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Speed", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mountpoints", 4, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleTabClicked, this));

  //TODO:
  // -add sample data to the list
}

void EditorApplicationVehicle::createPopupMenuVehicleTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for vehicle tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/VehiclePopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/VehiclePopUp/New"));
  menu_item->setText("New vehicle...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/VehiclePopUp/Edit"));
  menu_item->setText("Edit selected vehicle...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/VehiclePopUp/Delete"));
  menu_item->setText("Delete selected vehicle");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Vehicle/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationVehicle::VehicleNewClicked(const CEGUI::EventArgs &e)
{
  showVehicleNewWindow();
  return true;
}

bool EditorApplicationVehicle::VehicleEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Vehicle/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a vehicle from the list to edit it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_vehicle_to_edit = std::string(lbi->getText().c_str());
    showVehicleEditWindow();
  }
  return true;
}

bool EditorApplicationVehicle::VehicleDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Vehicle/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a vehicle from the list to delete it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_vehicle_to_delete = std::string(lbi->getText().c_str());
    showVehicleConfirmDeleteWindow();
  }
  return true;
}

bool EditorApplicationVehicle::VehicleTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/VehiclePopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Vehicle/List")->getPixelRect();
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

void EditorApplicationVehicle::showVehicleConfirmDeleteWindow(void)
{
  CEGUI::FrameWindow* frame;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/VehicleDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/VehicleDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/VehicleDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Vehicle...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/VehicleDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/VehicleDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/VehicleDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationVehicle::VehicleDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.31, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  winmgr.getWindow("Editor/VehicleDeleteFrame/Label")->setText("Do you really want to delete the vehicle \""+ID_of_vehicle_to_delete
                     +"\" and all of its references?");
  frame->moveToFront();
}

bool EditorApplicationVehicle::VehicleDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/VehicleDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/VehicleDeleteFrame");
  }
  return true;
}

bool EditorApplicationVehicle::VehicleDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_vehicle_to_delete == "")
  {
    showWarning("Error: Vehicle ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/VehicleDeleteFrame");
    return true;
  }
  if (!VehicleBase::getSingleton().deleteVehicle(ID_of_vehicle_to_delete))
  {
    showHint("VehicleBase class holds no vehicle of the given ID ("
             +ID_of_vehicle_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/VehicleDeleteFrame");
    ID_of_vehicle_to_delete = "";
    return true;
  }
  //kill references
  const unsigned int refs_deleted = InjectionManager::getSingleton().deleteReferencesOfAnimatedObject(ID_of_vehicle_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Vehicle \""+ID_of_vehicle_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Vehicle \""+ID_of_vehicle_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Vehicle/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_vehicle_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_vehicle_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/VehicleDeleteFrame");
  return true;
}

void EditorApplicationVehicle::closeEditWindowsVehicle(void)
{
  #warning There is still more to do.
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //delete deletion window
  if (winmgr.isWindowPresent("Editor/VehicleDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/VehicleDeleteFrame");
  }
}

void EditorApplicationVehicle::refreshVehicleList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Vehicle/List"))
  {
    showWarning("ERROR: Could not find vehicle list window in CEGUI Window Manager!");
    return;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Vehicle/List"));
  mcl->resetList();

  VehicleBase::Iterator first = VehicleBase::getSingleton().getFirst();
  const VehicleBase::Iterator end = VehicleBase::getSingleton().getEnd();
  while (first != end)
  {
    addVehicleRecordToCatalogue(first->first, first->second);
    ++first;
  }//while
  return;
}

void EditorApplicationVehicle::addVehicleRecordToCatalogue(const std::string& ID, const VehicleRecord& record)
{
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Vehicle/List"))
  {
    return;
  }
  CEGUI::MultiColumnList* mcl;
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Vehicle/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(record.Name);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(record.Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.MaxSpeed));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(record.MountpointCount));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
}

void EditorApplicationVehicle::showVehicleNewWindow(void)
{
  #warning Not implemented yet!
}

void EditorApplicationVehicle::showVehicleEditWindow(void)
{
  #warning Not implemented yet!
}

} //namespace
