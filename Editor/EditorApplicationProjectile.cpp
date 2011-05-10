/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2011 thoronador

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
#include "../Engine/ProjectileBase.h"
#include "../Engine/InjectionManager.h"
#include "../Engine/DuskFunctions.h"

namespace Dusk
{

EditorApplicationProjectile::EditorApplicationProjectile()
{
  ID_of_Projectile_to_delete = "";
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
  #warning Not implemented yet!
  return true;
}

bool EditorApplicationProjectile::ProjectileEditClicked(const CEGUI::EventArgs &e)
{
  #warning Not implemented yet!
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
  if (ID_of_Projectile_to_delete == "")
  {
    showWarning("Error: Projectile ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ProjectileDeleteFrame");
    return true;
  }
  if (!ProjectileBase::getSingleton().deleteProjectile(ID_of_Projectile_to_delete))
  {
    showHint("ProjectileBase class holds no Projectile of the given ID ("
             +ID_of_Projectile_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ProjectileDeleteFrame");
    ID_of_Projectile_to_delete = "";
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
  }
}

} //namespace
