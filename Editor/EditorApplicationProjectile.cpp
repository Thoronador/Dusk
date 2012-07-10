/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2011, 2012  thoronador

    The Dusk Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Editor.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "EditorApplicationProjectile.h"
#include <CEGUI/CEGUI.h>
#include "EditorApplicationBase.h"
#include "../Engine/WeaponRecord.h"
#include "../Engine/ProjectileRecord.h"
#include "../Engine/InjectionManager.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/Database.h"
#include "../Engine/API.h"

namespace Dusk
{

EditorApplicationProjectile::EditorApplicationProjectile()
{
  ID_of_Projectile_to_delete = "";
  ID_of_Projectile_to_edit = "";
}

EditorApplicationProjectile::~EditorApplicationProjectile()
{
  //empty
}

void EditorApplicationProjectile::createProjectileCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //Projectile tab
  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Projectile");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Projectiles");
  tab->addTab(pane);

  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                  (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Projectile/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("TTL", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Velocity", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Damage", 4, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileTabClicked, this));

  //TODO:
  // -add sample data to the list
}

void EditorApplicationProjectile::createPopupMenuProjectileTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for projectiles' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/ProjectilePopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ProjectilePopUp/New"));
  menu_item->setText("New projectile...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ProjectilePopUp/Edit"));
  menu_item->setText("Edit selected projectile...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ProjectilePopUp/Delete"));
  menu_item->setText("Delete selected projectile");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Projectile/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationProjectile::ProjectileTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/ProjectilePopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Projectile/List")->getPixelRect();
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

bool EditorApplicationProjectile::ProjectileNewClicked(const CEGUI::EventArgs &e)
{
  showProjectileNewWindow();
  return true;
}

bool EditorApplicationProjectile::ProjectileEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Projectile/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a projectile from the list to edit it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_Projectile_to_edit = std::string(lbi->getText().c_str());
    showProjectileEditWindow();
  }
  return true;
}

bool EditorApplicationProjectile::ProjectileDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Projectile/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a Projectile from the list to delete it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_Projectile_to_delete = std::string(lbi->getText().c_str());
    showProjectileConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationProjectile::showProjectileConfirmDeleteWindow()
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ProjectileDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ProjectileDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Projectile...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/ProjectileDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.31, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  winmgr.getWindow("Editor/ProjectileDeleteFrame/Label")->setText("Do you really want to delete the Projectile \""+ID_of_Projectile_to_delete
                     +"\" and all of its references?");
  frame->moveToFront();
}

bool EditorApplicationProjectile::ProjectileDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileDeleteFrame");
  }
  return true;
}

bool EditorApplicationProjectile::ProjectileDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_Projectile_to_delete.empty())
  {
    showWarning("Error: Projectile ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ProjectileDeleteFrame");
    return true;
  }
  if (!Database::getSingleton().deleteRecord(ID_of_Projectile_to_delete))
  {
    showHint("Database class holds no Projectile of the given ID ("
             +ID_of_Projectile_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ProjectileDeleteFrame");
    ID_of_Projectile_to_delete.clear();
    return true;
  }
  //kill references
  const unsigned int refs_deleted = InjectionManager::getSingleton().deleteReferencesOfAnimatedObject(ID_of_Projectile_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Projectile \""+ID_of_Projectile_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Projectile \""+ID_of_Projectile_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Projectile/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_Projectile_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_Projectile_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ProjectileDeleteFrame");
  return true;
}

void EditorApplicationProjectile::closeEditWindowsProjectile(void)
{
  //delete deletion window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileDeleteFrame");
    ID_of_Projectile_to_delete = "";
  }
  //delete creation window
  if (winmgr.isWindowPresent("Editor/ProjectileNewFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileNewFrame");
  }
  //delete edit window
  if (winmgr.isWindowPresent("Editor/ProjectileEditFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileEditFrame");
    ID_of_Projectile_to_edit = "";
  }
}

void EditorApplicationProjectile::showProjectileNewWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ProjectileNewFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ProjectileNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New Projectile...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/21.0f; //height of one element
    const float d = 0.5f/21.0f; //distance between two adjacent elements
    const float o = 2.0f/21.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for TTL
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/TTL_Label");
    button->setText("TTL (seconds):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for TTL
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileNewFrame/TTL_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for velocity
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/Velocity_Label");
    button->setText("Velocity:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for velocity
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileNewFrame/Velocity_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Damage
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/Damage_Label");
    button->setText("Damage");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for number of dice
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ProjectileNewFrame/Times_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(0.0f);
    frame->addChildWindow(spin);

    //static text for d
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileNewFrame/d_Label");
    button->setText("d");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for dice
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ProjectileNewFrame/Dice_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("6");
    spin->setMaximumValue(20.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileNewFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.45, 0)));
  frame->moveToFront();
}

bool EditorApplicationProjectile::ProjectileNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileNewFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileNewFrame");
  }
  return true;
}

bool EditorApplicationProjectile::ProjectileNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileNewFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileNewFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileNewFrame/TTL_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileNewFrame/Velocity_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileNewFrame/Times_Spin") and
      winmgr.isWindowPresent("Editor/ProjectileNewFrame/Dice_Spin"))
  {
    ProjectileRecord proj_rec;
    proj_rec.ID = winmgr.getWindow("Editor/ProjectileNewFrame/ID_Edit")->getText().c_str();
    proj_rec.Mesh = winmgr.getWindow("Editor/ProjectileNewFrame/Mesh_Edit")->getText().c_str();
    if (proj_rec.ID.empty() or proj_rec.Mesh.empty())
    {
      showHint("You have to enter an ID and Mesh path for the Projectile!");
      return true;
    }
    if (Database::getSingleton().hasRecord(proj_rec.ID))
    {
      showHint("A projectile or other record with the ID \""+proj_rec.ID+"\" already exists. Please "
              +"choose a different ID or delete the other projectile first.");
      return true;
    }
    proj_rec.DefaultVelocity = StringToFloat(winmgr.getWindow("Editor/ProjectileNewFrame/Velocity_Edit")->getText().c_str(), -1.0f);
    if (proj_rec.DefaultVelocity<=0.0f)
    {
      showHint("The velocity for the projectile has to be floating point value greater than zero!");
      return true;
    }
    proj_rec.DefaultTTL = StringToFloat(winmgr.getWindow("Editor/ProjectileNewFrame/TTL_Edit")->getText().c_str(), -1.0f);
    if (proj_rec.DefaultTTL<=0.0f)
    {
      showHint("The TTL for the projectile has to be floating point value greater than zero!");
      return true;
    }
    proj_rec.times = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/ProjectileNewFrame/Times_Spin")))->getCurrentValue());
    proj_rec.dice = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/ProjectileNewFrame/Dice_Spin")))->getCurrentValue());
    Database::getSingleton().addRecord(proj_rec);
    winmgr.destroyWindow("Editor/ProjectileNewFrame");
    refreshProjectileList();
    return true;
  }
  else
  {
    showWarning("Some window elements are missing for creating new projectiles!");
  }
  return true;
}


void EditorApplicationProjectile::refreshProjectileList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Projectile/List"))
  {
    showWarning("ERROR: Could not find Projectile list window in CEGUI Window Manager!");
    return;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Projectile/List"));
  mcl->resetList();

  Database::Iterator first = Database::getSingleton().getFirst();
  const Database::Iterator end = Database::getSingleton().getEnd();
  while (first != end)
  {
    if (first->second->getRecordType()==ProjectileRecord::RecordType)
      addProjectileRecordToCatalogue(first->first, *(static_cast<ProjectileRecord*>(first->second)));
    ++first;
  }//while
  return;
}

void EditorApplicationProjectile::addProjectileRecordToCatalogue(const std::string& ID, const ProjectileRecord& record)
{
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Projectile/List"))
  {
    return;
  }
  CEGUI::MultiColumnList* mcl;
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Projectile/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(record.Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.DefaultTTL));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(record.DefaultVelocity));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(record.times)+"d"+IntToString(record.dice));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
}

void EditorApplicationProjectile::showProjectileEditWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ProjectileEditFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ProjectileEditFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit Projectile...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/21.0f; //height of one element
    const float d = 0.5f/21.0f; //distance between two adjacent elements
    const float o = 2.0f/21.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileEditFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileEditFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for TTL
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/TTL_Label");
    button->setText("TTL (seconds):");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for TTL
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileEditFrame/TTL_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for velocity
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/Velocity_Label");
    button->setText("Velocity:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for velocity
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ProjectileEditFrame/Velocity_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Damage
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/Damage_Label");
    button->setText("Damage");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for number of dice
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ProjectileEditFrame/Times_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(0.0f);
    frame->addChildWindow(spin);

    //static text for d
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ProjectileEditFrame/d_Label");
    button->setText("d");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for dice
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/ProjectileEditFrame/Dice_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("6");
    spin->setMaximumValue(20.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileEditFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileEditFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ProjectileEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationProjectile::ProjectileEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.45, 0)));
  frame->moveToFront();

  //fill data into it

  const ProjectileRecord& p_rec = Database::getSingleton().getTypedRecord<ProjectileRecord>(ID_of_Projectile_to_edit);
  // ---- ID
  winmgr.getWindow("Editor/ProjectileEditFrame/ID_Edit")->setText(ID_of_Projectile_to_edit);
  // ---- mesh
  winmgr.getWindow("Editor/ProjectileEditFrame/Mesh_Edit")->setText(p_rec.Mesh);
  // ---- TTL
  winmgr.getWindow("Editor/ProjectileEditFrame/TTL_Edit")->setText(FloatToString(p_rec.DefaultTTL));
  // ---- velocity
  winmgr.getWindow("Editor/ProjectileEditFrame/Velocity_Edit")->setText(FloatToString(p_rec.DefaultVelocity));
  // ---- number of dice
  CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ProjectileEditFrame/Times_Spin"));
  spin->setText(IntToString(p_rec.times));
  spin->setCurrentValue(p_rec.times);
  // ---- dice type
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/ProjectileEditFrame/Dice_Spin"));
  spin->setText(IntToString(p_rec.dice));
  spin->setCurrentValue(p_rec.dice);
  return;
}

bool EditorApplicationProjectile::ProjectileEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileEditFrame"))
  {
    winmgr.destroyWindow("Editor/ProjectileEditFrame");
  }
  ID_of_Projectile_to_edit.clear();
  return true;
}

bool EditorApplicationProjectile::ProjectileEditFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ProjectileEditFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileEditFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileEditFrame/TTL_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileEditFrame/Velocity_Edit") and
      winmgr.isWindowPresent("Editor/ProjectileEditFrame/Times_Spin") and
      winmgr.isWindowPresent("Editor/ProjectileEditFrame/Dice_Spin"))
  {
    ProjectileRecord proj_rec;
    proj_rec.ID = winmgr.getWindow("Editor/ProjectileEditFrame/ID_Edit")->getText().c_str();
    proj_rec.Mesh = winmgr.getWindow("Editor/ProjectileEditFrame/Mesh_Edit")->getText().c_str();
    if (proj_rec.ID.empty() or proj_rec.Mesh.empty())
    {
      showHint("You have to enter an ID and Mesh path for the Projectile!");
      return true;
    }
    proj_rec.DefaultVelocity = StringToFloat(winmgr.getWindow("Editor/ProjectileEditFrame/Velocity_Edit")->getText().c_str(), -1.0f);
    if (proj_rec.DefaultVelocity<=0.0f)
    {
      showHint("The velocity for the projectile has to be floating point value greater than zero!");
      return true;
    }
    proj_rec.DefaultTTL = StringToFloat(winmgr.getWindow("Editor/ProjectileEditFrame/TTL_Edit")->getText().c_str(), -1.0f);
    if (proj_rec.DefaultTTL<=0.0f)
    {
      showHint("The TTL for the projectile has to be floating point value greater than zero!");
      return true;
    }
    proj_rec.times = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/ProjectileEditFrame/Times_Spin")))->getCurrentValue());
    proj_rec.dice = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/ProjectileEditFrame/Dice_Spin")))->getCurrentValue());

    const bool idChanged = (proj_rec.ID!=ID_of_Projectile_to_edit);
    if (idChanged)
    {
      if (Database::getSingleton().hasRecord(proj_rec.ID))
      {
        showHint("A projectile or other record with the ID \""+proj_rec.ID+"\" already exists. Please "
                +"choose a different ID or delete the other projectile first.\n");
        return true;
      }//if
    }//if
    else
    {
      //ID remained the same, but the user might have deleted the projectile.
      if (!Database::getSingleton().hasTypedRecord<ProjectileRecord>(proj_rec.ID))
      {
        showHint("A projectile with the ID \""+proj_rec.ID+"\" does not exist. You "
                +"have possibly deleted the Projectile.");
        return true;
      }//if
    }
    const bool meshChanged = Database::getSingleton().getTypedRecord<ProjectileRecord>(ID_of_Projectile_to_edit).Mesh != proj_rec.Mesh;
    //get changes into projectile base
    Database::getSingleton().addRecord(proj_rec);

    //update enabled projectiles that are affected by changes
    unsigned int affected_references = 0;
    if (idChanged)
    {
      //change IDs
      affected_references =
      InjectionManager::getSingleton().updateReferencesAfterIDChange(ID_of_Projectile_to_edit, proj_rec.ID, getAPI().getOgreSceneManager());
      //delete data record for old ID
      Database::getSingleton().deleteRecord(ID_of_Projectile_to_edit);
      //update projectile information in related weapons
      Database::getSingleton().updateProjectilesAfterIDChange(ID_of_Projectile_to_edit, proj_rec.ID);
    }
    else if (meshChanged)
    {
      //update it
      affected_references =
      InjectionManager::getSingleton().reenableReferencesOfObject(ID_of_Projectile_to_edit, getAPI().getOgreSceneManager());
    }
    if (meshChanged or idChanged)
    {
      showHint("Changes accepted, "+IntToString(affected_references)+" references have been updated.");
    }
    winmgr.destroyWindow("Editor/ProjectileEditFrame");
    ID_of_Projectile_to_edit = "";
    refreshProjectileList();
    return true;
  }
  else
  {
    showWarning("Some window elements are missing for editing projectiles!");
  }
  return true;
}

} //namespace
