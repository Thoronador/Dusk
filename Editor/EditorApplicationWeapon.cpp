/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2011, 2012, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "EditorApplicationWeapon.h"
#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>
#include "../Engine/database/WeaponRecord.h"
#include "../Engine/database/ProjectileRecord.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/database/Database.h"
#include "../Engine/API.h"

namespace Dusk
{

EditorApplicationWeapon::EditorApplicationWeapon()
: ID_of_Weapon_to_delete(""),
  ID_of_Weapon_to_edit("")
{
}

EditorApplicationWeapon::~EditorApplicationWeapon()
{
  //empty
}

void EditorApplicationWeapon::createWeaponCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //Weapon tab
  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Weapon");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Weapons");
  tab->addTab(pane);

  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                  (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Weapon/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Value", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Weight", 4, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Type", 5, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Range", 6, CEGUI::UDim(0.19, 0));
  mcl->addColumn("ProjectileID", 7, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Time", 8, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Damage", 9, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponTabClicked, this));

  //TODO:
  // -add sample data to the list
}

void EditorApplicationWeapon::createPopupMenuWeaponTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for weapons' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/WeaponPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/WeaponPopUp/New"));
  menu_item->setText("New weapon...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/WeaponPopUp/Edit"));
  menu_item->setText("Edit selected weapon...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/WeaponPopUp/Delete"));
  menu_item->setText("Delete selected weapon");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Weapon/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationWeapon::WeaponTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/WeaponPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Weapon/List")->getOuterRectClipper();
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

bool EditorApplicationWeapon::WeaponNewClicked(const CEGUI::EventArgs &e)
{
  showWeaponNewWindow();
  return true;
}

bool EditorApplicationWeapon::WeaponEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Weapon/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a weapon from the list to edit it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_Weapon_to_edit = std::string(lbi->getText().c_str());
    showWeaponEditWindow();
  }
  return true;
}

bool EditorApplicationWeapon::WeaponDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Weapon/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a weapon from the list to delete it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_Weapon_to_delete = std::string(lbi->getText().c_str());
    showWeaponConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationWeapon::showWeaponConfirmDeleteWindow(void)
{
  CEGUI::FrameWindow* frame;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/WeaponDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/WeaponDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Weapon...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/WeaponDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.31, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  winmgr.getWindow("Editor/WeaponDeleteFrame/Label")->setText("Do you really want to delete the weapon \""+ID_of_Weapon_to_delete
                     +"\" and all of its references?");
  frame->moveToFront();
}

bool EditorApplicationWeapon::WeaponDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponDeleteFrame");
  }
  return true;
}

bool EditorApplicationWeapon::WeaponDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_Weapon_to_delete.empty())
  {
    showWarning("Error: Weapon ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/WeaponDeleteFrame");
    return true;
  }
  if (!Database::getSingleton().deleteRecord(ID_of_Weapon_to_delete))
  {
    showHint("Database class holds no Weapon of the given ID ("
             +ID_of_Weapon_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/WeaponDeleteFrame");
    ID_of_Weapon_to_delete.clear();
    return true;
  }
  //kill references
  const unsigned int refs_deleted = ObjectManager::getSingleton().deleteReferencesOfObject(ID_of_Weapon_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Weapon \""+ID_of_Weapon_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Weapon \""+ID_of_Weapon_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Weapon/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_Weapon_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_Weapon_to_delete.clear();
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/WeaponDeleteFrame");
  return true;
}

void EditorApplicationWeapon::closeEditWindowsWeapon(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //delete deletion window
  if (winmgr.isWindowPresent("Editor/WeaponDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponDeleteFrame");
  }
  //delete window for new weapons
  if (winmgr.isWindowPresent("Editor/WeaponNewFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponNewFrame");
  }
  //delete window for editing weapons
  if (winmgr.isWindowPresent("Editor/WeaponEditFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponEditFrame");
  }
}

void EditorApplicationWeapon::showWeaponNewWindow(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/WeaponNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/WeaponNewFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/WeaponNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New Weapon...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/33.0f; //height of one element
    const float d = 0.5f/33.0f; //distance between two adjacent elements
    const float o = 2.25f/33.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for value
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Value_Label");
    button->setText("Value:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for value
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Value_Edit");
    button->setText("0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for weight
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Weight_Label");
    button->setText("Weight:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for weight
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Weight_Edit");
    button->setText("1.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for time between attacks
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/TimeBetween_Label");
    button->setText("Attack Time:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for time between attacks
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/TimeBetween_Edit");
    button->setText("1.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for type
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Type_Label");
    button->setText("Type:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //radiobox for melee type
    CEGUI::RadioButton* radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/WeaponNewFrame/RadioTypeMelee"));
    radio->setText("Melee");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(true);
    radio->setGroupID(55555);
    frame->addChildWindow(radio);

    //radiobox for gun type
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/WeaponNewFrame/RadioTypeGun"));
    radio->setText("Gun");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(false);
    radio->setGroupID(55555);
    frame->addChildWindow(radio);

    //static text for range (melee only)
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Range_Label");
    button->setText("Range (melee):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for time range
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Range_Edit");
    button->setText("128.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for projectile (gun only)
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Projectile_Label");
    button->setText("Projectile (gun):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for projectile
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponNewFrame/Projectile_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Damage
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/Damage_Label");
    button->setText("Damage");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for number of dice
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/WeaponNewFrame/Times_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(0.0f);
    frame->addChildWindow(spin);

    //static text for d
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponNewFrame/d_Label");
    button->setText("d");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for dice
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/WeaponNewFrame/Dice_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("6");
    spin->setMaximumValue(20.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponNewFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.6, 0)));
  frame->moveToFront();
}

bool EditorApplicationWeapon::WeaponNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponNewFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponNewFrame");
  }
  return true;
}

bool EditorApplicationWeapon::WeaponNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponNewFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Name_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Value_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Weight_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/TimeBetween_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/RadioTypeGun") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/RadioTypeMelee") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Range_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Projectile_Edit") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Times_Spin") and
      winmgr.isWindowPresent("Editor/WeaponNewFrame/Dice_Spin"))
  {
    const std::string weapID = winmgr.getWindow("Editor/WeaponNewFrame/ID_Edit")->getText().c_str();
    if (weapID.empty())
    {
      showHint("You have to enter an ID for the new weapon!");
      return true;
    }
    //weapon record that will be filled with the data
    WeaponRecord weapRec;
    weapRec.Mesh = winmgr.getWindow("Editor/WeaponNewFrame/Mesh_Edit")->getText().c_str();
    if (weapRec.Mesh.empty())
    {
      showHint("You have to enter a mesh path for the new weapon!");
      return true;
    }
    weapRec.Name = winmgr.getWindow("Editor/WeaponNewFrame/Name_Edit")->getText().c_str();
    if (weapRec.Name.empty())
    {
      showHint("You have to enter a name for the new weapon!");
      return true;
    }
    weapRec.value = StringToInt(winmgr.getWindow("Editor/WeaponNewFrame/Value_Edit")->getText().c_str(), -1);
    if (weapRec.value<0)
    {
      showHint("The value of the weapon has to be a non-negative integer!");
      return true;
    }
    weapRec.weight = StringToFloat(winmgr.getWindow("Editor/WeaponNewFrame/Weight_Edit")->getText().c_str(), -1.0f);
    if (weapRec.weight<0.0f)
    {
      showHint("The weight of the weapon has to be a non-negative floating point value!");
      return true;
    }
    weapRec.TimeBetweenAttacks = StringToFloat(winmgr.getWindow("Editor/WeaponNewFrame/TimeBetween_Edit")->getText().c_str(), -1.0f);
    if (weapRec.TimeBetweenAttacks<=0.0f)
    {
      showHint("The attack time of the weapon has to be a positive floating point value!");
      return true;
    }
    //type check
    if (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponNewFrame/RadioTypeGun"))->isSelected())
    {
      weapRec.Type = wtGun;
      weapRec.Range = 0.0f;
      weapRec.ProjectileID = winmgr.getWindow("Editor/WeaponNewFrame/Projectile_Edit")->getText().c_str();
      if (weapRec.ProjectileID.empty())
      {
        showHint("You have to enter a projectile ID for the new weapon!");
        return true;
      }
      if (!Database::getSingleton().hasTypedRecord<ProjectileRecord>(weapRec.ProjectileID))
      {
        showHint("There is no projectile with the ID \""+weapRec.ProjectileID
                 +"\"! Check the ID for proper spelling or create a new "
                 +"projectile with that ID.");
        return true;
      }
    }
    else
    {
      weapRec.Type = wtMelee;
      weapRec.ProjectileID = "";
      weapRec.Range = StringToFloat(winmgr.getWindow("Editor/WeaponNewFrame/Range_Edit")->getText().c_str(), -1.0f);
      if (weapRec.Range<=0.0f)
      {
        showHint("The range of the weapon has to be a positive floating point value!");
        return true;
      }
    }
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponNewFrame/Times_Spin"));
    weapRec.DamageTimes = static_cast<uint8_t>(spin->getCurrentValue());
    spin = static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponNewFrame/Dice_Spin"));
    weapRec.DamageDice = static_cast<uint8_t>(spin->getCurrentValue());
    //now check for presence of such a weapon/record
    if (Database::getSingleton().hasRecord(weapID))
    {
      showHint("A weapon or other record with the ID \""+weapID+"\" already exists! Please "
              +"choose a different ID or delete the other record first.");
      return true;
    }
    //add weapon to Database
    weapRec.ID = weapID;
    Database::getSingleton().addRecord(weapRec);
    // ...and to the cataogue
    addWeaponRecordToCatalogue(weapID, weapRec);
    //delete window
    winmgr.destroyWindow("Editor/WeaponNewFrame");
    return true;
  }
  else
  {
    showWarning("Some window elements are missing for creating new weapon!");
  }
  return true;
}

void EditorApplicationWeapon::showWeaponEditWindow(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/WeaponEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/WeaponEditFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/WeaponEditFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit Weapon...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/33.0f; //height of one element
    const float d = 0.5f/33.0f; //distance between two adjacent elements
    const float o = 2.25f/33.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for value
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Value_Label");
    button->setText("Value:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for value
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Value_Edit");
    button->setText("0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for weight
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Weight_Label");
    button->setText("Weight:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for weight
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Weight_Edit");
    button->setText("1.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for time between attacks
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/TimeBetween_Label");
    button->setText("Attack Time:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for time between attacks
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/TimeBetween_Edit");
    button->setText("1.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for type
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Type_Label");
    button->setText("Type:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //radiobox for melee type
    CEGUI::RadioButton* radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/WeaponEditFrame/RadioTypeMelee"));
    radio->setText("Melee");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(true);
    radio->setGroupID(666666);
    frame->addChildWindow(radio);

    //radiobox for gun type
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/WeaponEditFrame/RadioTypeGun"));
    radio->setText("Gun");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(false);
    radio->setGroupID(666666);
    frame->addChildWindow(radio);

    //static text for range (melee only)
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Range_Label");
    button->setText("Range (melee):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for time range
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Range_Edit");
    button->setText("128.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for projectile (gun only)
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Projectile_Label");
    button->setText("Projectile (gun):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for projectile
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/WeaponEditFrame/Projectile_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Damage
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/Damage_Label");
    button->setText("Damage");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for number of dice
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/WeaponEditFrame/Times_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(0.0f);
    frame->addChildWindow(spin);

    //static text for d
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/WeaponEditFrame/d_Label");
    button->setText("d");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for dice
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/WeaponEditFrame/Dice_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("6");
    spin->setMaximumValue(20.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponEditFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponEditFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/WeaponEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationWeapon::WeaponEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  //set fields
  winmgr.getWindow("Editor/WeaponEditFrame/ID_Edit")->setText(ID_of_Weapon_to_edit);
  const WeaponRecord& wRec = Database::getSingleton().getTypedRecord<WeaponRecord>(ID_of_Weapon_to_edit);
  winmgr.getWindow("Editor/WeaponEditFrame/Name_Edit")->setText(wRec.Name);
  winmgr.getWindow("Editor/WeaponEditFrame/Mesh_Edit")->setText(wRec.Mesh);
  winmgr.getWindow("Editor/WeaponEditFrame/Value_Edit")->setText(IntToString(wRec.value));
  winmgr.getWindow("Editor/WeaponEditFrame/Weight_Edit")->setText(FloatToString(wRec.weight));
  winmgr.getWindow("Editor/WeaponEditFrame/TimeBetween_Edit")->setText(FloatToString(wRec.TimeBetweenAttacks));
  switch (wRec.Type)
  {
    case wtMelee:
         static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponEditFrame/RadioTypeMelee"))->setSelected(true);
         static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponEditFrame/RadioTypeGun"))->setSelected(false);
         break;
    case wtGun:
         static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponEditFrame/RadioTypeGun"))->setSelected(true);
         static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponEditFrame/RadioTypeMelee"))->setSelected(false);
         break;
  }//swi
  winmgr.getWindow("Editor/WeaponEditFrame/Range_Edit")->setText(FloatToString(wRec.Range));
  winmgr.getWindow("Editor/WeaponEditFrame/Projectile_Edit")->setText(wRec.ProjectileID);
  static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponEditFrame/Times_Spin"))->setCurrentValue(wRec.DamageTimes);
  static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponEditFrame/Dice_Spin"))->setCurrentValue(wRec.DamageDice);

  //move and resize window/set position and size
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.6, 0)));
  frame->moveToFront();
}

bool EditorApplicationWeapon::WeaponEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponEditFrame"))
  {
    winmgr.destroyWindow("Editor/WeaponEditFrame");
  }
  return true;
}

bool EditorApplicationWeapon::WeaponEditFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WeaponEditFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Name_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Value_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Weight_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/TimeBetween_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/RadioTypeGun") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/RadioTypeMelee") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Range_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Projectile_Edit") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Times_Spin") and
      winmgr.isWindowPresent("Editor/WeaponEditFrame/Dice_Spin"))
  {
    const std::string weapID = winmgr.getWindow("Editor/WeaponEditFrame/ID_Edit")->getText().c_str();
    if (weapID.empty())
    {
      showHint("You have to enter an ID for the weapon!");
      return true;
    }
    //weapon record that will be filled with the data
    WeaponRecord weapRec;
    weapRec.Mesh = winmgr.getWindow("Editor/WeaponEditFrame/Mesh_Edit")->getText().c_str();
    if (weapRec.Mesh.empty())
    {
      showHint("You have to enter a mesh path for the weapon!");
      return true;
    }
    weapRec.Name = winmgr.getWindow("Editor/WeaponEditFrame/Name_Edit")->getText().c_str();
    if (weapRec.Name.empty())
    {
      showHint("You have to enter a name for the weapon!");
      return true;
    }
    weapRec.value = StringToInt(winmgr.getWindow("Editor/WeaponEditFrame/Value_Edit")->getText().c_str(), -1);
    if (weapRec.value<0)
    {
      showHint("The value of the weapon has to be a non-negative integer!");
      return true;
    }
    weapRec.weight = StringToFloat(winmgr.getWindow("Editor/WeaponEditFrame/Weight_Edit")->getText().c_str(), -1.0f);
    if (weapRec.weight<0.0f)
    {
      showHint("The weight of the weapon has to be a non-negative floating point value!");
      return true;
    }
    weapRec.TimeBetweenAttacks = StringToFloat(winmgr.getWindow("Editor/WeaponEditFrame/TimeBetween_Edit")->getText().c_str(), -1.0f);
    if (weapRec.TimeBetweenAttacks<=0.0f)
    {
      showHint("The attack time of the weapon has to be a positive floating point value!");
      return true;
    }
    //type check
    if (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/WeaponEditFrame/RadioTypeGun"))->isSelected())
    {
      weapRec.Type = wtGun;
      weapRec.Range = 0.0f;
      weapRec.ProjectileID = winmgr.getWindow("Editor/WeaponEditFrame/Projectile_Edit")->getText().c_str();
      if (weapRec.ProjectileID.empty())
      {
        showHint("You have to enter a projectile ID for the weapon!");
        return true;
      }
      if (!Database::getSingleton().hasTypedRecord<ProjectileRecord>(weapRec.ProjectileID))
      {
        showHint("There is no projectile with the ID \""+weapRec.ProjectileID
                 +"\"! Check the ID for proper spelling or create a new "
                 +"projectile with that ID.");
        return true;
      }
    }
    else
    {
      weapRec.Type = wtMelee;
      weapRec.ProjectileID = "";
      weapRec.Range = StringToFloat(winmgr.getWindow("Editor/WeaponEditFrame/Range_Edit")->getText().c_str(), -1.0f);
      if (weapRec.Range<=0.0f)
      {
        showHint("The range of the weapon has to be a positive floating point value!");
        return true;
      }
    }
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponEditFrame/Times_Spin"));
    weapRec.DamageTimes = static_cast<uint8_t>(spin->getCurrentValue());
    spin = static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/WeaponEditFrame/Dice_Spin"));
    weapRec.DamageDice = static_cast<uint8_t>(spin->getCurrentValue());


    const bool idChanged = (weapID!=ID_of_Weapon_to_edit);
    if (idChanged)
    {
      if (Database::getSingleton().hasRecord(weapID))
      {
        showHint("A weapon or other record with the ID \""+weapID+"\" already exists. Please "
                +"choose a different ID or delete the other record first.\n");
        return true;
      }//if
    }//if
    else
    {
      //ID remained the same, but the user might have deleted the weapon.
      if (!Database::getSingleton().hasTypedRecord<WeaponRecord>(weapID))
      {
        showHint("A weapon with the ID \""+weapID+"\" does not exist. You "
                +"have possibly deleted the weapon.");
        return true;
      }//if
    }

    const bool meshChanged = Database::getSingleton().getTypedRecord<WeaponRecord>(ID_of_Weapon_to_edit).Mesh != weapRec.Mesh;
    //get changes into Database
    weapRec.ID = weapID;
    Database::getSingleton().addRecord(weapRec);

    //update enabled Weapons that are affected by changes
    unsigned int affected_references = 0;
    if (idChanged)
    {
      //change IDs
      affected_references =
      ObjectManager::getSingleton().updateReferencesAfterIDChange(ID_of_Weapon_to_edit, weapID, getAPI().getOgreSceneManager());
      //delete data record for old ID
      Database::getSingleton().deleteRecord(ID_of_Weapon_to_edit);
    }
    else if (meshChanged)
    {
      //update it
      affected_references =
      ObjectManager::getSingleton().reenableReferencesOfObject(ID_of_Weapon_to_edit, getAPI().getOgreSceneManager());
    }
    if (meshChanged or idChanged)
    {
      showHint("Changes accepted, "+IntToString(affected_references)+" references have been updated.");
    }
    winmgr.destroyWindow("Editor/WeaponEditFrame");
    ID_of_Weapon_to_edit.clear();
    refreshWeaponList();
    return true;
  }
  else
  {
    showWarning("Some window elements are missing for editing weapon!");
  }
  return true;
}

void EditorApplicationWeapon::refreshWeaponList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Weapon/List"))
  {
    showWarning("ERROR: Could not find Weapon list window in CEGUI Window Manager!");
    return;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Weapon/List"));
  mcl->resetList();

  Database::Iterator first = Database::getSingleton().getFirst();
  const Database::Iterator end = Database::getSingleton().getEnd();
  while (first != end)
  {
    if (first->second->getRecordType() == WeaponRecord::RecordType)
      addWeaponRecordToCatalogue(first->first, *static_cast<WeaponRecord*>(first->second));
    ++first;
  }//while
  return;
}

void EditorApplicationWeapon::addWeaponRecordToCatalogue(const std::string& ID, const WeaponRecord& record)
{
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Weapon/List"))
  {
    return;
  }
  CEGUI::MultiColumnList* mcl;
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Weapon/List"));
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
  lbi = new CEGUI::ListboxTextItem(IntToString(record.value));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.weight));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
  lbi = new CEGUI::ListboxTextItem("<type>");
  switch (record.Type)
  {
    case wtMelee:
         lbi->setText("melee");
         break;
    case wtGun:
         lbi->setText("gun");
         break;
  }//swi
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 5, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.Range));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 6, row);
  lbi = new CEGUI::ListboxTextItem(record.ProjectileID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 7, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.TimeBetweenAttacks));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 8, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(record.DamageTimes)+"d"+IntToString(record.DamageDice));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 9, row);
}

} //namespace
