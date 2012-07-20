/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2009, 2010, 2011 thoronador

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

#include "EditorApplicationObject.h"
#include "EditorApplicationBase.h"
#include <iostream>
#include <CEGUI/CEGUI.h>
#include "../Engine/database/Database.h"
#include "../Engine/database/ObjectRecord.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/DuskConstants.h"
#include "../Engine/API.h"

namespace Dusk
{

EditorApplicationObject::EditorApplicationObject()
{
  ID_of_object_to_delete = "";

  ID_of_object_to_edit = "";
}

EditorApplicationObject::~EditorApplicationObject()
{
  //empty
}

void EditorApplicationObject::createPopupMenuObjectTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for static objects' tab
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/ObjectPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/New"));
  menu_item->setText("New object...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/Edit"));
  menu_item->setText("Edit selected object...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/ObjectPopUp/Delete"));
  menu_item->setText("Delete selected object");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Object/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

void EditorApplicationObject::closeEditWindowsObject(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //frame window for new objects
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  //frame window for editing objects
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
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
  //reset IDs for windows
  ID_of_object_to_delete = "";
  ID_of_object_to_edit = "";
}

void EditorApplicationObject::refreshObjectList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Object/List"))
  {
    showWarning("ERROR: Could not find object list window in Window Manager!");
    return;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Object/List"));
  mcl->resetList();

  Database::Iterator start = Database::getSingleton().getFirst();
  const Database::Iterator end = Database::getSingleton().getEnd();
  while (start != end)
  {
    if (start->second->getRecordType() == ObjectRecord::RecordType)
      addObjectRecordToCatalogue(start->first, static_cast<ObjectRecord*>(start->second)->Mesh, static_cast<ObjectRecord*>(start->second)->collide);
    ++start;
  }//while
  return;
}

void EditorApplicationObject::showObjectNewWindow(void)
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
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.4, 0)));
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
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //checkbox for collision flag
    button = winmgr.createWindow("TaharezLook/Checkbox", "Editor/ObjectNewFrame/CollisionCheck");
    button->setText("Activate collision detection");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.6, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    // ---- buttons at bottom ----
    // OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectNewFrameOKClicked, this));
    frame->addChildWindow(button);

    //cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
  static_cast<CEGUI::Checkbox*>(winmgr.getWindow("Editor/ObjectNewFrame/CollisionCheck"))->setSelected(true);
}

void EditorApplicationObject::showObjectEditWindow(void)
{
  if (ID_of_object_to_edit=="")
  {
    std::cout << "ObjectEditWindow: No ID given.\n";
    return;
  }

  if (!Database::getSingleton().hasTypedRecord<ObjectRecord>(ID_of_object_to_edit))
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
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.4, 0)));
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
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //checkbox for collision flag
    button = winmgr.createWindow("TaharezLook/Checkbox", "Editor/ObjectEditFrame/CollisionCheck");
    button->setText("Activate collision detection");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.6, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //buttons at bottom
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectEditFrame/Save");
    button->setText("Save");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectEditFrameSaveClicked, this));
    frame->addChildWindow(button);

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
  //set data
  winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->setText(ID_of_object_to_edit);
  winmgr.getWindow("Editor/ObjectEditFrame/Mesh_Edit")->setText(
      Database::getSingleton().getTypedRecord<ObjectRecord>(ID_of_object_to_edit).Mesh);
  static_cast<CEGUI::Checkbox*>(winmgr.getWindow(
      "Editor/ObjectEditFrame/CollisionCheck"))->setSelected(
          Database::getSingleton().getTypedRecord<ObjectRecord>(ID_of_object_to_edit).collide);
}

void EditorApplicationObject::showObjectConfirmDeleteWindow(void)
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
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/ObjectDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplicationObject::showObjectEditConfirmIDChangeWindow(void)
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

    //buttons: New, Rename, Cancel
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/New");
    button->setText("New Object");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.06, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectConfirmIDChangeNewClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/Rename");
    button->setText("Rename Object");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectConfirmIDChangeRenameClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmObjectIDChangeFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectConfirmIDChangeCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
  //adjust text
  CEGUI::Window* tb = winmgr.getWindow("Editor/ConfirmObjectIDChangeFrame/Text");
  tb->setText("The ID of this object has changed.\nDo you want to rename the object \""
             +ID_of_object_to_edit+"\" to \">insert new ID here<\" or create a new one?");

  if (winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
  {
    tb->setText("The ID of this object has changed.\nDo you want to rename the object \""
               +ID_of_object_to_edit+"\" to \""
               +winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText()
               +"\" or create a new one?");
  }
}

//for catalogue:

void EditorApplicationObject::addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh, const bool collision)
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
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(BoolToString(collision));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
}

bool EditorApplicationObject::ObjectConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
    //get the editboxes with the needed entries
    ObjectRecord obj_rec;
    obj_rec.ID = std::string(winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText().c_str());
    obj_rec.Mesh = std::string(winmgr.getWindow("Editor/ObjectEditFrame/Mesh_Edit")->getText().c_str());
    obj_rec.collide = static_cast<CEGUI::Checkbox*>(winmgr.getWindow(
        "Editor/ObjectEditFrame/CollisionCheck"))->isSelected();

    if (Database::getSingleton().hasRecord(obj_rec.ID))
    {
      showWarning("An Object or some other record with the ID \""+obj_rec.ID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" object the same ID.");
      return true;
    }//if

    //"rename", i.e. create object with new ID and delete object with old ID
    Database::getSingleton().addRecord(obj_rec);
    Database::getSingleton().deleteRecord(ID_of_object_to_edit);
    //update all objects
    ObjectManager::getSingleton().updateReferencesAfterIDChange(ID_of_object_to_edit, obj_rec.ID, getAPI().getOgreSceneManager()/*mSceneMgr*/);
    //add row for new object to catalogue
    addObjectRecordToCatalogue(obj_rec.ID, obj_rec.Mesh, obj_rec.collide);
    //remove row of old ID
    CEGUI::MultiColumnList * mcl;
    CEGUI::ListboxItem * lb_item = NULL;
    mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Object/List"));
    lb_item = mcl->findColumnItemWithText(ID_of_object_to_edit, 0, NULL);
    mcl->removeRow(mcl->getItemRowIndex(lb_item));
    //close edit window
    winmgr.destroyWindow("Editor/ObjectEditFrame");
    ID_of_object_to_edit.clear();
  }
  return true;
}

bool EditorApplicationObject::ObjectConfirmIDChangeNewClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit"))
  {
    //close confirmation window
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
    //get the editboxes with the needed entries
    ObjectRecord obj_rec;
    obj_rec.ID = std::string(winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText().c_str());
    obj_rec.Mesh = std::string(winmgr.getWindow("Editor/ObjectEditFrame/Mesh_Edit")->getText().c_str());

    if (Database::getSingleton().hasRecord(obj_rec.ID))
    {
      showWarning("An Object with the ID \""+obj_rec.ID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" object the same ID.");
      return true;
    }//if
    obj_rec.collide = static_cast<CEGUI::Checkbox*>(winmgr.getWindow(
        "Editor/ObjectEditFrame/CollisionCheck"))->isSelected();
    //add new row to catalogue
    addObjectRecordToCatalogue(obj_rec.ID, obj_rec.Mesh, obj_rec.collide);
    //add new object to database
    Database::getSingleton().addRecord(obj_rec);
    //close edit window
    winmgr.destroyWindow("Editor/ObjectEditFrame");
    ID_of_object_to_edit.clear();
  }
  return true;
}

bool EditorApplicationObject::ObjectConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
  }
  return true;
}

bool EditorApplicationObject::ObjectNewClicked(const CEGUI::EventArgs &e)
{
  showObjectNewWindow();
  return true;
}

bool EditorApplicationObject::ObjectEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Object/List"));
  CEGUI::ListboxItem* lb_item = mcl->getFirstSelectedItem();
  if (lb_item==NULL)
  {
    showHint("You have to select an object from the list before you can edit it.");
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

bool EditorApplicationObject::ObjectDeleteClicked(const CEGUI::EventArgs &e)
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

bool EditorApplicationObject::ObjectNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  return true;
}

bool EditorApplicationObject::ObjectNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectNewFrame"))
  {
    ObjectRecord obj_rec;
    obj_rec.ID = winmgr.getWindow("Editor/ObjectNewFrame/ID_Edit")->getText().c_str();
    obj_rec.Mesh = winmgr.getWindow("Editor/ObjectNewFrame/Mesh_Edit")->getText().c_str();
    //make sure we have some data
    if (obj_rec.ID.empty())
    {
      showWarning("You have to enter an ID string to create a new object!");
      return true;
    }
    if (obj_rec.Mesh.empty())
    {
      showWarning("You have to enter a mesh path to create a new object!");
      return true;
    }

    //check for presence of object with same ID
    if (Database::getSingleton().hasRecord(obj_rec.ID))
    {
      showWarning("An Object with the given ID already exists.");
      return true;
    }
    obj_rec.collide = static_cast<CEGUI::Checkbox*>(winmgr.getWindow(
        "Editor/ObjectNewFrame/CollisionCheck"))->isSelected();
    //finally add it to database
    Database::getSingleton().addRecord(obj_rec);
    //update catalogue
    addObjectRecordToCatalogue(obj_rec.ID, obj_rec.Mesh, obj_rec.collide);
    //destroy window
    winmgr.destroyWindow("Editor/ObjectNewFrame");
  }
  return true;
}

bool EditorApplicationObject::ObjectEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //destroy the edit window
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
  }
  //...and also destroy the window that is used to confirm ID change
  if (winmgr.isWindowPresent("Editor/ConfirmObjectIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmObjectIDChangeFrame");
  }
  return true;
}

bool EditorApplicationObject::ObjectEditFrameSaveClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (!winmgr.isWindowPresent("Editor/ObjectEditFrame/ID_Edit") ||
      !winmgr.isWindowPresent("Editor/ObjectEditFrame/Mesh_Edit") ||
      !winmgr.isWindowPresent("Editor/ObjectEditFrame/CollisionCheck"))
  {
    showWarning("Error: Editbox(es) for ID and/or mesh is/are not registered at window manager!");
    return true;
  }//if

  ObjectRecord obj_rec;
  obj_rec.ID = winmgr.getWindow("Editor/ObjectEditFrame/ID_Edit")->getText().c_str();
  obj_rec.Mesh = winmgr.getWindow("Editor/ObjectEditFrame/Mesh_Edit")->getText().c_str();

  if (obj_rec.ID.empty())
  {
    showHint("You have to enter an ID for this object!");
    return true;
  }
  if (obj_rec.Mesh.empty())
  {
    showHint("You have to enter a mesh path for this object!");
    return true;
  }
  if (obj_rec.ID!=ID_of_object_to_edit)
  {
    //ID was changed
   showObjectEditConfirmIDChangeWindow();
   return true;
  }
  obj_rec.collide = static_cast<CEGUI::Checkbox*>(winmgr.getWindow(
      "Editor/ObjectEditFrame/CollisionCheck"))->isSelected();
  //check if mesh has remained the same
  if (obj_rec.Mesh==Database::getSingleton().getTypedRecord<ObjectRecord>(ID_of_object_to_edit).Mesh and
     (obj_rec.collide==Database::getSingleton().getTypedRecord<ObjectRecord>(ID_of_object_to_edit).collide))
  {
    showHint("You have not changed the data of this object, thus there are no changes to be saved.");
    return true;
  }

  //save it
  Database::getSingleton().addRecord(obj_rec);
  //update list
  refreshObjectList();
  //update shown objects
  ObjectManager::getSingleton().reenableReferencesOfObject(ID_of_object_to_edit, getAPI().getOgreSceneManager()/*mSceneMgr*/);
  //delete window
  if (winmgr.isWindowPresent("Editor/ObjectEditFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectEditFrame");
  }
  ID_of_object_to_edit = "";
  return true;
}

bool EditorApplicationObject::ObjectDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ObjectDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/ObjectDeleteFrame");
  }
  return true;
}

bool EditorApplicationObject::ObjectDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_object_to_delete == "")
  {
    showWarning("Error: object ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ObjectDeleteFrame");
    return true;
  }
  if (!Database::getSingleton().deleteRecord(ID_of_object_to_delete))
  {
    showHint("ObjectBase class holds no object of the given ID ("
             +ID_of_object_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ObjectDeleteFrame");
    return true;
  }
  unsigned int refs_deleted = ObjectManager::getSingleton().deleteReferencesOfObject(ID_of_object_to_delete);
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

void EditorApplicationObject::createObjectCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //Object tab
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
  mcl->addColumn("ID", 0, CEGUI::UDim(0.38, 0));
  mcl->addColumn("Mesh", 1, CEGUI::UDim(0.38, 0));
  mcl->addColumn("Collision", 2, CEGUI::UDim(0.2, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationObject::ObjectTabClicked, this));

  //add some random data
  addObjectRecordToCatalogue("The_ID", "flora/Oak.mesh", false);
  addObjectRecordToCatalogue("static_seat", "YetAnother.mesh", true);
}

bool EditorApplicationObject::ObjectTabClicked(const CEGUI::EventArgs &e)
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

} //namespace
