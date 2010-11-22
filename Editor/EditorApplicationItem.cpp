/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2010 thoronador

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

#include "EditorApplicationItem.h"
#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>
#include <iostream>
#include "../Engine/DuskFunctions.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/API.h"

namespace Dusk
{

EditorApplicationItem::EditorApplicationItem()
{
  ID_of_item_to_delete = "";
  ID_of_item_to_edit = "";
}

EditorApplicationItem::~EditorApplicationItem()
{
  //empty
}

void EditorApplicationItem::CreatePopupMenuItemTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for items' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/ItemPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ItemPopUp/New"));
  menu_item->setText("New item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationItem::ItemNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ItemPopUp/Edit"));
  menu_item->setText("Edit selected item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationItem::ItemEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ItemPopUp/Delete"));
  menu_item->setText("Delete selected item");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationItem::ItemDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Item/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationItem::ItemNewClicked(const CEGUI::EventArgs &e)
{
  showItemNewWindow();
  return true;
}

bool EditorApplicationItem::ItemEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Item/List"));
  CEGUI::ListboxItem* lb_item = mcl->getFirstSelectedItem();
  if (lb_item==NULL)
  {
    showHint("You have not selected an item from the list yet.");
    return true;
  }

  unsigned int row_index = mcl->getItemRowIndex(lb_item);
  lb_item = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
  ID_of_item_to_edit = std::string(lb_item->getText().c_str());
  showItemEditWindow();
  return true;
}

bool EditorApplicationItem::ItemDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Item/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select an item from the list to delete it.");
  }
  else
  {
    unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_item_to_delete = std::string(lbi->getText().c_str());
    showItemConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationItem::showItemNewWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/ItemNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ItemNewFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ItemNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New Item...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    CEGUI::Window * button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemNewFrame/ID_Label");
    button->setText("Item ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemNewFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.35, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for weight
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemNewFrame/Weight_Label");
    button->setText("Weight:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for value
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemNewFrame/Value_Label");
    button->setText("Value:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemNewFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.35, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item weight
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemNewFrame/Weight_Edit");
    button->setText("1.0");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item value
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemNewFrame/Value_Edit");
    button->setText("1");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemNewFrameOKClicked, this));
    frame->addChildWindow(button);

    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.48, 0), CEGUI::UDim(0.22, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

bool EditorApplicationItem::ItemNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if ( winmgr.isWindowPresent("Editor/ItemNewFrame"))
  {
    winmgr.destroyWindow("Editor/ItemNewFrame");
  }
  return true;
}

bool EditorApplicationItem::ItemNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ItemNewFrame"))
  {
    CEGUI::Editbox* id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemNewFrame/ID_Edit"));
    CEGUI::Editbox* name_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemNewFrame/Name_Edit"));
    CEGUI::Editbox* value_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemNewFrame/Value_Edit"));
    CEGUI::Editbox* weight_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemNewFrame/Weight_Edit"));
    CEGUI::Editbox* mesh_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemNewFrame/Mesh_Edit"));
    //make sure we have some data
    if (id_edit->getText()=="")
    {
      showWarning("You have to enter an ID string to create a new item!");
      return true;
    }
    if (name_edit->getText()=="")
    {
      showWarning("You have to enter a name for this new item!");
      return true;
    }
    if (value_edit->getText()=="")
    {
      showWarning("You have to enter a value (integer) for this new item!");
      return true;
    }
    if (weight_edit->getText()=="")
    {
      showWarning("You have to enter a weight (non-negative floating point value) for this new item!");
      return true;
    }
    if (mesh_edit->getText()=="")
    {
      showWarning("You have to enter a mesh path to create a new item!");
      return true;
    }

    //check for presence of item with same ID
    if (ItemBase::getSingleton().hasItem(std::string(id_edit->getText().c_str())))
    {
      showWarning("An Item with the given ID already exists.");
      return true;
    }

    //finally add it to ItemBase
    ItemRecord entered_data;
    entered_data.Name = std::string(name_edit->getText().c_str());
    entered_data.Mesh = std::string(mesh_edit->getText().c_str());
    entered_data.weight = StringToFloat(std::string(weight_edit->getText().c_str()), -1.0f);
    if (entered_data.weight<0.0f)
    {
      showWarning("The entered weight is either negative or not a valid floating point value!");
      return true;
    }
    entered_data.value = StringToInt(std::string(value_edit->getText().c_str()), -1);
    if (entered_data.value<0)
    {
      showWarning("The entered value is either negative or not a valid integer value!");
      return true;
    }

    ItemBase::getSingleton().addItem(std::string(id_edit->getText().c_str()), entered_data.Name,
                                     entered_data.value, entered_data.weight, entered_data.Mesh);
    //update item catalogue
    addItemRecordToCatalogue(std::string(id_edit->getText().c_str()), entered_data);
    //destroy window
    winmgr.destroyWindow("Editor/ItemNewFrame");
  }
  return true;
}

void EditorApplicationItem::showItemEditConfirmIDChangeWindow(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow*  frame = NULL;

  if (winmgr.isWindowPresent("Editor/ConfirmItemIDChangeFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ConfirmItemIDChangeFrame"));
  }
  else
  {
    //create it
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ConfirmItemIDChangeFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Rename Item?");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox",
                                                        "Editor/ConfirmItemIDChangeFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.12, 0), CEGUI::UDim(0.17, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    if (winmgr.isWindowPresent("Editor/ItemEditFrame/ID_Edit"))
    {
      textbox->setText("The ID of this item has changed.\nDo you want to rename the item \""
                   +ID_of_item_to_edit+"\" to \""
                   +winmgr.getWindow("Editor/ItemEditFrame/ID_Edit")->getText()
                   +"\" or create a new one?");
    }
    else
    {
      textbox->setText("The ID of this item was changed.\nDo you want to rename the item \""
                       +ID_of_item_to_edit+"\" to \">insert new ID here<\" or create a new one?");
    }
    frame->addChildWindow(textbox);

    //buttons: New, Rename, Cancel
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmItemIDChangeFrame/New");
    button->setText("New Item");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.06, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemConfirmIDChangeNewClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmItemIDChangeFrame/Rename");
    button->setText("Rename Item");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemConfirmIDChangeRenameClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmItemIDChangeFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemConfirmIDChangeCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

bool EditorApplicationItem::ItemConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmItemIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmItemIDChangeFrame");
  }
  return true;
}

bool EditorApplicationItem::ItemConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmItemIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/ID_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Mesh_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Name_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Value_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Weight_Edit"))
  {
    winmgr.destroyWindow("Editor/ConfirmItemIDChangeFrame");
    //get the editboxes with the needed entries
    std::string ItemID;
    ItemRecord ir;
    ir.value = 0;
    ir.weight = 0.0f;
    ItemID = std::string(winmgr.getWindow("Editor/ItemEditFrame/ID_Edit")->getText().c_str());
    ir.Mesh = std::string(winmgr.getWindow("Editor/ItemEditFrame/Mesh_Edit")->getText().c_str());
    ir.Name = std::string(winmgr.getWindow("Editor/ItemEditFrame/Name_Edit")->getText().c_str());
    ir.value = StringToInt(std::string(winmgr.getWindow("Editor/ItemEditFrame/Value_Edit")->getText().c_str()), -1);
    ir.weight = StringToFloat(std::string(winmgr.getWindow("Editor/ItemEditFrame/Weight_Edit")->getText().c_str()), -1.0f);

    if (ItemBase::getSingleton().hasItem(ItemID))
    {
      showWarning("An Item with the ID \""+ItemID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" item the same ID.");
      return true;
    }//if

    //"rename", i.e. create item with new ID and delete item with old ID
    ItemBase::getSingleton().addItem(ItemID, ir.Name, ir.value, ir.weight, ir.Mesh);
    ItemBase::getSingleton().deleteItem(ID_of_item_to_edit);
    //update all items
    ObjectManager::getSingleton().updateReferencesAfterIDChange(ID_of_item_to_edit, ItemID, getAPI().getOgreSceneManager());
    //add row for new item to catalogue
    addItemRecordToCatalogue(ItemID, ir);
    //remove row of old ID
    CEGUI::MultiColumnList * mcl;
    CEGUI::ListboxItem * lb_item = NULL;
    mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Item/List"));
    lb_item = mcl->findColumnItemWithText(ID_of_item_to_edit, 0, NULL);
    mcl->removeRow(mcl->getItemRowIndex(lb_item));
    //close edit window
    winmgr.destroyWindow("Editor/ItemEditFrame");
    ID_of_item_to_edit = "";
  }
  return true;
}

bool EditorApplicationItem::ItemConfirmIDChangeNewClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmItemIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/ID_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Mesh_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Name_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Value_Edit") &&
      winmgr.isWindowPresent("Editor/ItemEditFrame/Weight_Edit"))
  {
    //close confirmation window
    winmgr.destroyWindow("Editor/ConfirmItemIDChangeFrame");
    //get the editboxes with the needed entries
    std::string ItemID;
    ItemRecord i_rec;

    ItemID = std::string(winmgr.getWindow("Editor/ItemEditFrame/ID_Edit")->getText().c_str());
    i_rec.Mesh = std::string(winmgr.getWindow("Editor/ItemEditFrame/Mesh_Edit")->getText().c_str());
    i_rec.Name = std::string(winmgr.getWindow("Editor/ItemEditFrame/Name_Edit")->getText().c_str());
    i_rec.value = StringToInt(std::string(winmgr.getWindow("Editor/ItemEditFrame/Value_Edit")->getText().c_str()), -1);
    i_rec.weight = StringToFloat(std::string(winmgr.getWindow("Editor/ItemEditFrame/Weight_Edit")->getText().c_str()), -1.0f);

    if (ItemBase::getSingleton().hasItem(ItemID))
    {
      showWarning("An Item with the ID \""+ItemID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" item the same ID.");
      return true;
    }//if
    //add new row to catalogue
    addItemRecordToCatalogue(ItemID, i_rec);
    //add new object to database (ItemBase)
    ItemBase::getSingleton().addItem(ItemID, i_rec.Name, i_rec.value, i_rec.weight, i_rec.Mesh);
    //close edit window
    winmgr.destroyWindow("Editor/ItemEditFrame");
    ID_of_item_to_edit = "";
  }
  return true;
}

void EditorApplicationItem::showItemEditWindow(void)
{
  if (ID_of_item_to_edit=="")
  {
    std::cout << "ItemEditWindow: No ID given.\n";
    return;
  }

  if (!ItemBase::getSingleton().hasItem(ID_of_item_to_edit))
  {
    std::cout << "ItemEditWindow: Item not present in database.\n";
    showWarning("There seems to be no item with the ID \""+ID_of_item_to_edit
                +"\". Aborting.");
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/ItemEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ItemEditFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ItemEditFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Edit Item...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    CEGUI::Window * button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemEditFrame/ID_Label");
    button->setText("Item ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemEditFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.35, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for weight
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemEditFrame/Weight_Label");
    button->setText("Weight:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for value
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemEditFrame/Value_Label");
    button->setText("Value:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/ItemEditFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemEditFrame/ID_Edit");
    button->setText(ID_of_item_to_edit);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemEditFrame/Name_Edit");
    button->setText(ItemBase::getSingleton().getItemName(ID_of_item_to_edit));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.35, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item weight
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemEditFrame/Weight_Edit");
    button->setText(FloatToString(ItemBase::getSingleton().getItemWeight(ID_of_item_to_edit)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item value
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemEditFrame/Value_Edit");
    button->setText(IntToString(ItemBase::getSingleton().getItemValue(ID_of_item_to_edit)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.5, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for item mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/ItemEditFrame/Mesh_Edit");
    button->setText(ItemBase::getSingleton().getMeshName(ID_of_item_to_edit, false));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemEditFrame/Save");
    button->setText("Save");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemEditFrameSaveClicked, this));
    frame->addChildWindow(button);

    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
  return;
}

bool EditorApplicationItem::ItemEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ItemEditFrame"))
  {
    winmgr.destroyWindow("Editor/ItemEditFrame");
  }
  return true;
}

bool EditorApplicationItem::ItemEditFrameSaveClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Editbox* id_edit;
  CEGUI::Editbox* mesh_edit;
  CEGUI::Editbox* name_edit;
  CEGUI::Editbox* value_edit;
  CEGUI::Editbox* weight_edit;

  if (!winmgr.isWindowPresent("Editor/ItemEditFrame/ID_Edit") ||
      !winmgr.isWindowPresent("Editor/ItemEditFrame/Mesh_Edit") ||
      !winmgr.isWindowPresent("Editor/ItemEditFrame/Name_Edit") ||
      !winmgr.isWindowPresent("Editor/ItemEditFrame/Value_Edit") ||
      !winmgr.isWindowPresent("Editor/ItemEditFrame/Weight_Edit"))
  {
    showWarning("Error: Editboxes for ID, mesh, name, value or weight are not registered at window manager!");
    return true;
  }//if
  id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemEditFrame/ID_Edit"));
  mesh_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemEditFrame/Mesh_Edit"));
  name_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemEditFrame/Name_Edit"));
  value_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemEditFrame/Value_Edit"));
  weight_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/ItemEditFrame/Weight_Edit"));

  if (std::string(id_edit->getText().c_str())=="")
  {
    showHint("You have to enter an ID for this item!");
    return true;
  }
  if (std::string(mesh_edit->getText().c_str())=="")
  {
    showHint("You have to enter a mesh path for this item!");
    return true;
  }
  if (std::string(name_edit->getText().c_str())=="")
  {
    showHint("You have to enter a name for this item!");
    return true;
  }
  if (std::string(value_edit->getText().c_str())=="")
  {
    showHint("You have to enter a value for this item!");
    return true;
  }
  if (std::string(weight_edit->getText().c_str())=="")
  {
    showHint("You have to enter a weight for this item!");
    return true;
  }

  ItemRecord ir;
  ir.Mesh = std::string(mesh_edit->getText().c_str());
  ir.Name = std::string(name_edit->getText().c_str());
  ir.value = StringToInt(std::string(value_edit->getText().c_str()), -1);
  ir.weight = StringToFloat(std::string(weight_edit->getText().c_str()), -1.0f);

  //check data
  if (ir.value<0)
  {
    showHint("The field \"value\" has to be filled with a non-negative integer value!");
    return true;
  }
  if (ir.weight<0.0f)
  {
    showHint("The weight has to be a valid, non-negative floating point value!");
    return true;
  }

  if (std::string(id_edit->getText().c_str())!=ID_of_item_to_edit)
  {
    //ID was changed
   showItemEditConfirmIDChangeWindow();
   return true;
  }
  //check if data has remained the same
  if (ir.Mesh == ItemBase::getSingleton().getMeshName(ID_of_item_to_edit, false) &&
      ir.Name == ItemBase::getSingleton().getItemName(ID_of_item_to_edit) &&
      ir.value == ItemBase::getSingleton().getItemValue(ID_of_item_to_edit) &&
      ir.weight == ItemBase::getSingleton().getItemWeight(ID_of_item_to_edit))
  {
    showHint("You have not changed the data of this item, thus there are no changes to be saved.");
    return true;
  }
  //save it
  ItemBase::getSingleton().addItem(std::string(id_edit->getText().c_str()),
                                   ir.Name, ir.value, ir.weight, ir.Mesh);
  //update list
  RefreshItemList();
  //reference update
  ObjectManager::getSingleton().reenableReferencesOfObject(ID_of_item_to_edit, getAPI().getOgreSceneManager());
  //delete window
  if (winmgr.isWindowPresent("Editor/ItemEditFrame"))
  {
    winmgr.destroyWindow("Editor/ItemEditFrame");
  }
  ID_of_item_to_edit = "";
  return true;
}

bool EditorApplicationItem::ItemDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ItemDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ItemDeleteFrame");
  }
  return true;
}

bool EditorApplicationItem::ItemDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_item_to_delete == "")
  {
    showWarning("Error: item ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ItemDeleteFrame");
    return true;
  }
  if (!ItemBase::getSingleton().deleteItem(ID_of_item_to_delete))
  {
    showHint("ItemBase class holds no item of the given ID ("
             +ID_of_item_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ItemDeleteFrame");
    return true;
  }
  //delete references
  unsigned int refs_deleted = ObjectManager::getSingleton().deleteReferencesOfObject(ID_of_item_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Item \""+ID_of_item_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Item \""+ID_of_item_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }

  //delete row in multi column list of items
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Item/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_item_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_item_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ItemDeleteFrame");
  return true;
}

void EditorApplicationItem::showItemConfirmDeleteWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::MultiLineEditbox* textbox = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ItemDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ItemDeleteFrame"));
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.getWindow("Editor/ItemDeleteFrame/Label"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ItemDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Item...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/ItemDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ItemDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationItem::ItemDeleteFrameNoClicked, this));
  }
  textbox->setText("Do you really want to delete the item \""+ID_of_item_to_delete
                  +"\"and all of its references?");
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.22, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplicationItem::RefreshItemList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = NULL;
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Item/List"))
  {
    showWarning("ERROR: Could not find item list window in Window Manager!");
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Item/List"));
  mcl->resetList();

  std::map<std::string, ItemRecord>::const_iterator first;
  first = ItemBase::getSingleton().getFirst();
  const std::map<std::string, ItemRecord>::const_iterator end = ItemBase::getSingleton().getEnd();
  while (first != end)
  {
    addItemRecordToCatalogue(first->first, first->second);
    ++first;
  }//while
  return;
}

void EditorApplicationItem::addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData)
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
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(ItemData.Name);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(ItemData.weight));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(ItemData.value));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(ItemData.Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
}

} //namespace
