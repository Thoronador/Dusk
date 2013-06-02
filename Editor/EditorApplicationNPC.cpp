/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2010, 2011, 2012, 2013  Thoronador

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

#include "EditorApplicationNPC.h"
#include <CEGUI/CEGUI.h>
#include "EditorApplicationBase.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/database/ItemRecord.h"
#include "../Engine/InjectionManager.h"
#include "../Engine/database/Database.h"
#include "../Engine/API.h"

namespace Dusk
{

EditorApplicationNPC::EditorApplicationNPC()
: ID_of_NPC_to_delete(""),
  ID_of_NPC_to_edit(""),
  m_NewNPCTagPoints(NPCTagPoints::getNullTagPoints()),
  m_NewNPCAnimations(NPCAnimations::getNullAnimations()),
  m_EditNPCTagPoints(NPCTagPoints::getNullTagPoints()),
  m_EditNPCAnimations(NPCAnimations::getNullAnimations())
{
}

EditorApplicationNPC::~EditorApplicationNPC()
{
  //empty
}

void EditorApplicationNPC::refreshNPCList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = NULL;
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/NPC/List"))
  {
    showWarning("ERROR: Could not find NPC list window in CEGUI Window Manager!");
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/NPC/List"));
  mcl->resetList();

  Database::Iterator first = Database::getSingleton().getFirst();
  const Database::Iterator end = Database::getSingleton().getEnd();
  while (first != end)
  {
    if (first->second->getRecordType()==NPCRecord::RecordType)
      addNPCRecordToCatalogue(first->first, *static_cast<NPCRecord*>(first->second));
    ++first;
  }//while
  return;
}

void EditorApplicationNPC::addNPCRecordToCatalogue(const std::string& ID, const NPCRecord& Record)
{
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/NPC/List"))
  {
    return;
  }
  CEGUI::MultiColumnList* mcl = NULL;
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(Record.Name);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(Record.Level));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(BoolToString(Record.Female));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(Record.Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
}

void EditorApplicationNPC::createPopupMenuNPCTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for NPCs' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/NPCPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/New"));
  menu_item->setText("New NPC...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/Edit"));
  menu_item->setText("Edit selected NPC...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/Delete"));
  menu_item->setText("Delete selected NPC");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/NPC/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationNPC::NPCTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/NPCPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/NPC/List")->getOuterRectClipper();
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

bool EditorApplicationNPC::NPCNewClicked(const CEGUI::EventArgs &e)
{
  showNPCNewWindow();
  return true;
}

bool EditorApplicationNPC::NPCEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a NPC from the list to edit it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_NPC_to_edit = std::string(lbi->getText().c_str());
    showNPCEditWindow();
  }
  return true;
}

bool EditorApplicationNPC::NPCDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  const CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a NPC from the list to delete it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_NPC_to_delete = std::string(lbi->getText().c_str());
    showNPCConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationNPC::showNPCConfirmDeleteWindow()
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete NPC...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/NPCDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.31, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  winmgr.getWindow("Editor/NPCDeleteFrame/Label")->setText("Do you really want to delete the NPC \""+ID_of_NPC_to_delete
                     +"\" and all of its references?");
  frame->moveToFront();
}

bool EditorApplicationNPC::NPCDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/NPCDeleteFrame");
  }
  return true;
}

bool EditorApplicationNPC::NPCDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_NPC_to_delete.empty())
  {
    showWarning("Error: NPC ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
    return true;
  }
  if (!Database::getSingleton().deleteRecord(ID_of_NPC_to_delete))
  {
    showHint("Database class holds no NPC of the given ID ("
             +ID_of_NPC_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
    ID_of_NPC_to_delete.clear();
    return true;
  }
  //kill references
  const unsigned int refs_deleted = InjectionManager::getSingleton().deleteReferencesOfAnimatedObject(ID_of_NPC_to_delete);
  if (refs_deleted == 0)
  {
    showHint("NPC \""+ID_of_NPC_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("NPC \""+ID_of_NPC_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_NPC_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_NPC_to_delete.clear();
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
  return true;
}

void EditorApplicationNPC::showNPCNewWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New NPC...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/36.0f; //height of one element
    const float d = 0.5f/36.0f; //distance between two adjacent elements
    const float o = 2.0f/36.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for gender
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Gender_Label");
    button->setText("Gender:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    CEGUI::RadioButton* radio = NULL;
    //radio button for male
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioMale"));
    radio->setText("Male");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(true);
    radio->setGroupID(888);
    frame->addChildWindow(radio);
    //radio button for female
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioFemale"));
    radio->setText("Female");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(false);
    radio->setGroupID(888);
    frame->addChildWindow(radio);

    //static text for Attributes
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Attributes_Label");
    button->setText("Attributes");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Inventory
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Inventory_Label");
    button->setText("Inventory");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Str
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Str_Label");
    button->setText("Str.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Strength
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Str_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Level
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Lvl_Label");
    button->setText("Level:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Level
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Level_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Agility
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Agi_Label");
    button->setText("Agi.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Agility
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Agi_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Willpower
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Will_Label");
    button->setText("Will:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Willpower
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Will_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Vitality
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Vit_Label");
    button->setText("Vit.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Vitality
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Vit_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Charisma
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Cha_Label");
    button->setText("Cha.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Charisma
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Cha_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Intelligence
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Int_Label");
    button->setText("Int.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Intelligence
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Int_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Luck
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Luck_Label");
    button->setText("Luck:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Luck
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Luck_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //multi column list for inventory content
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
        (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/NPCNewFrame/InventoryList"));
    mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(4*h+3*d, 0)));
    mcl->addColumn("#", 0, CEGUI::UDim(0.22, 0));
    mcl->addColumn("Item ID", 1, CEGUI::UDim(0.72, 0));
    mcl->setUserColumnDraggingEnabled(false);
    mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameInventoryListClicked, this));
    frame->addChildWindow(mcl);
    //adds a popup menu + callbacks for clicks
    createPopupMenuNPCNewFrameList();

    //animations button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/AnimButton");
    button->setText("Animations...");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameAnimsClicked, this));
    frame->addChildWindow(button);

    //tag points button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/Tagpoints");
    button->setText("Tagpoints...");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameTagpointsClicked, this));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+10*h+10*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+10*h+10*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.38, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
  frame->moveToFront();
  //reset animations and tagpoints, just to be sure we don't use old values set
  // during a previous creation
  m_NewNPCAnimations = NPCAnimations::getNullAnimations();
  m_NewNPCTagPoints = NPCTagPoints::getNullTagPoints();
}

bool EditorApplicationNPC::NPCNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame");
  }
  if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewTagsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewAnimsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Name_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/RadioFemale") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Str_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Level_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Agi_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Will_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Vit_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Cha_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Int_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Luck_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList"))
  {
    NPCRecord npc_rec;
    npc_rec.ID = winmgr.getWindow("Editor/NPCNewFrame/ID_Edit")->getText().c_str();
    npc_rec.Name = winmgr.getWindow("Editor/NPCNewFrame/Name_Edit")->getText().c_str();
    npc_rec.Mesh = winmgr.getWindow("Editor/NPCNewFrame/Mesh_Edit")->getText().c_str();
    if (npc_rec.ID.empty() or npc_rec.Name.empty() or npc_rec.Mesh.empty())
    {
      showHint("You have to enter an ID, Name and Mesh path for the NPC.");
      return true;
    }
    if (Database::getSingleton().hasRecord(npc_rec.ID))
    {
      showHint("An NPC or other object with the ID \""+npc_rec.ID+"\" already exists. Please "
              +"choose a different ID or delete the other NPC first.\n");
      return true;
    }
    npc_rec.Level = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/NPCNewFrame/Level_Spin")))->getCurrentValue());

    npc_rec.Attributes.Str = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Str_Spin")))->getCurrentValue());
    npc_rec.Attributes.Agi = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Agi_Spin")))->getCurrentValue());
    npc_rec.Attributes.Vit = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Vit_Spin")))->getCurrentValue());
    npc_rec.Attributes.Int = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Int_Spin")))->getCurrentValue());
    npc_rec.Attributes.Will = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Will_Spin")))->getCurrentValue());
    npc_rec.Attributes.Cha = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Cha_Spin")))->getCurrentValue());
    npc_rec.Attributes.Luck = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Luck_Spin")))->getCurrentValue());
    npc_rec.Female = (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/NPCNewFrame/RadioFemale")))->isSelected();
    npc_rec.InventoryAtStart = MCLToInventory(static_cast<CEGUI::MultiColumnList*>(winmgr.getWindow("Editor/NPCNewFrame/InventoryList")));

    Database::getSingleton().addRecord(npc_rec);
    winmgr.destroyWindow("Editor/NPCNewFrame");
    if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
    {
      winmgr.destroyWindow("Editor/NPCNewTagsFrame");
    }//if
    if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
    {
      winmgr.destroyWindow("Editor/NPCNewAnimsFrame");
    }//if
    if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
    {
      winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
    }//if
    refreshNPCList();
  }//if
  else
  {
    showHint("Error: At least one required CEGUI window element is not present.\n");
  }
  return true;
}

void EditorApplicationNPC::createPopupMenuNPCNewFrameList(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for inventory list
  if (wmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList/PopUp"))
  {
    return;
  }
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/NPCNewFrame/InventoryList/PopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Add"));
  menu_item->setText("Add item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListAddClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Edit"));
  menu_item->setText("Edit selected item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Delete"));
  menu_item->setText("Delete selected item");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/NPCNewFrame/InventoryList")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationNPC::NPCNewFrameInventoryListClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/NPCNewFrame/InventoryList/PopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/NPCNewFrame/InventoryList")->getOuterRectClipper();
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

bool EditorApplicationNPC::InventoryListAddClicked(const CEGUI::EventArgs &e)
{
  showInventoryListAddWindow();
  return true;
}

bool EditorApplicationNPC::InventoryListEditClicked(const CEGUI::EventArgs &e)
{
  //not implemented yet
  #warning Not implemented yet!
  //not implemented yet
}

bool EditorApplicationNPC::InventoryListDeleteClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                         (winmgr.getWindow("Editor/NPCNewFrame/InventoryList"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select an item before you can delete it.");
    return true;
  }
  mcl->removeRow(mcl->getItemRowIndex(lbi));
  return true;
}

void EditorApplicationNPC::showInventoryListAddWindow(void)
{
  //not implemented yet (adjust size and position of child windows)
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewFrame/AddInventoryFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New NPC > Add items to inventory");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //spinner for items
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/AddInventoryFrame/Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.2, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMinimumValue(1.0f);
    spin->setStepSize(1.0f);
    frame->addChildWindow(spin);
    //list box for items
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.createWindow("TaharezLook/Combobox", "Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox"));
    CBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.5, 0)));
    CBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
    CBox->setReadOnly(true);
    frame->addChildWindow(CBox);
    updateItemList(CBox);
    //create Add button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/AddInventoryFrame/Add");
    button->setText("Add");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::AddInventoryFrameAddClicked, this));
    //create Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/AddInventoryFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::AddInventoryFrameCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.3, 0)));
  frame->moveToFront();
}

void EditorApplicationNPC::updateItemList(CEGUI::Combobox* combo)
{
  if (combo!=NULL)
  {
    combo->resetList();
    CEGUI::ListboxItem* lbi = NULL;
    Database::Iterator itemFirst = Database::getSingleton().getFirst();
    const Database::Iterator itemEnd = Database::getSingleton().getEnd();
    while (itemFirst!=itemEnd)
    {
      if (itemFirst->second->getRecordType()==ItemRecord::RecordType)
      {
        lbi = new CEGUI::ListboxTextItem(itemFirst->first);
        lbi->setTooltipText(static_cast<ItemRecord*>(itemFirst->second)->Name);
        combo->addItem(lbi);
      }
      ++itemFirst;
    }//while
  }
}

bool EditorApplicationNPC::AddInventoryFrameAddClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame/Spin") &&
      winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox") &&
      winmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList"))
  {
    //add stuff
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*>
      (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox"));
    const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
    if (lbi==NULL)
    {
      showHint("You have to select an item from the list. If you don't see any"
              +std::string(" items there, you haven't created any items yet.")
              +" Go to the item tab in the catalogue and create one.");
      return true;
    }
    const CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame/Spin"));
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                (winmgr.getWindow("Editor/NPCNewFrame/InventoryList"));
    //add it to catalogue
    unsigned int row;
    CEGUI::ListboxItem* lbi2 = new CEGUI::ListboxTextItem(IntToString(
                          static_cast<unsigned int>(spin->getCurrentValue())));
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    row = mcl->addRow(lbi2, 0);
    lbi2 = new CEGUI::ListboxTextItem(lbi->getText());
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    mcl->setItem(lbi2, 1, row);
    //destroy window
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
    return true;
  }
  return true;
}

bool EditorApplicationNPC::AddInventoryFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
  }
  return true;
}

Inventory EditorApplicationNPC::MCLToInventory(const CEGUI::MultiColumnList* mcl)
{
  Inventory temp;
  temp.makeEmpty();
  if (mcl!=NULL)
  {
    if (mcl->getColumnCount()==2)
    {
      const unsigned int rows = mcl->getRowCount();
      unsigned int i;
      for (i=0; i<rows; ++i)
      {
        temp.addItem(mcl->getItemAtGridReference(CEGUI::MCLGridRef(i,1))->getText().c_str(),
          StringToInt(mcl->getItemAtGridReference(CEGUI::MCLGridRef(i,0))->getText().c_str(),0));
      }
    }
  }
  return temp;
}

void EditorApplicationNPC::InventoryToMCL(const Inventory& invent, CEGUI::MultiColumnList* target)
{
  if (target==NULL) return;
  target->resetList();
  ConstInventoryIterator iter = invent.getFirst();
  const ConstInventoryIterator end_iter = invent.getEnd();
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  while (iter!=end_iter)
  {
    lbi = new CEGUI::ListboxTextItem(IntToString(iter->second));
    lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    row = target->addRow(lbi, 0);
    lbi = new CEGUI::ListboxTextItem(iter->first);
    lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    target->setItem(lbi, 1, row);
    ++iter;
  } //while
}

bool EditorApplicationNPC::NPCNewFrameAnimsClicked(const CEGUI::EventArgs &e)
{
  showNPCNewAnimsWindow();
  return true;
}

bool EditorApplicationNPC::NPCNewFrameTagpointsClicked(const CEGUI::EventArgs &e)
{
  showNPCNewTagpointsWindow();
  return true;
}

void EditorApplicationNPC::showNPCNewAnimsWindow()
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewAnimsFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewAnimsFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Set NPC Animations...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/30.0f; //height of one element
    const float d = 0.5f/30.0f; //distance between two adjacent elements
    const float o = 2.0f/30.0f; //offset of first element

    //static info text
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/NPCNewAnimsFrame/Info_Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(2*h, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    textbox->setText(std::string("Info:\n")
                    +"You can enter multiple animation names, if needed. Just "
                    +"separate them by commatas.");
    frame->addChildWindow(textbox);

    CEGUI::Window * button = NULL;
    //static text for walk
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Walk_Label");
    button->setText("Walk:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for walk
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Walk_Edit");
    button->setText(m_NewNPCAnimations.Walk);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for idle
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Idle_Label");
    button->setText("Idle:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for idle
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Idle_Edit");
    button->setText(m_NewNPCAnimations.Idle);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for melee
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Melee_Label");
    button->setText("Melee:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for melee attack animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Melee_Edit");
    button->setText(m_NewNPCAnimations.MeleeAttack);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for projectile attack animation
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Projectile_Label");
    button->setText("Projectile:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for projectile attack animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Projectile_Edit");
    button->setText(m_NewNPCAnimations.ProjectileAttack);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for jump
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Jump_Label");
    button->setText("Jump:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for jump animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Jump_Edit");
    button->setText(m_NewNPCAnimations.Jump);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for death
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewAnimsFrame/Death_Label");
    button->setText("Death:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for death animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewAnimsFrame/Death_Edit");
    button->setText(m_NewNPCAnimations.Death);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewAnimsFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+8*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewAnimsFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewAnimsFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+8*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewAnimsFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.55, 0)));
  frame->moveToFront();
}

void EditorApplicationNPC::showNPCNewTagpointsWindow()
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewTagsFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewTagsFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Set NPC Tag Points...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/18.0f; //height of one element
    const float d = 0.5f/18.0f; //distance between two adjacent elements
    const float o = 2.0f/18.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text left hand
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewTagsFrame/LeftHand_Label");
    button->setText("Left hand:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for left hand
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewTagsFrame/LeftHand_Edit");
    button->setText(m_NewNPCTagPoints.HandLeft);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text right hand
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewTagsFrame/RightHand_Label");
    button->setText("Right hand:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for right hand
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewTagsFrame/RightHand_Edit");
    button->setText(m_NewNPCTagPoints.HandRight);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text left sheath
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewTagsFrame/LeftSheath_Label");
    button->setText("Left sheath:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for left sheath
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewTagsFrame/LeftSheath_Edit");
    button->setText(m_NewNPCTagPoints.SheathLeft);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text right sheath
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewTagsFrame/RightSheath_Label");
    button->setText("Right sheath:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for right sheath
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewTagsFrame/RightSheath_Edit");
    button->setText(m_NewNPCTagPoints.SheathRight);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewTagsFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewTagsFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewTagsFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewTagsFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

bool EditorApplicationNPC::NPCNewAnimsFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewAnimsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCNewAnimsFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
  {
    //save new animations
    m_NewNPCAnimations.Walk = winmgr.getWindow("Editor/NPCNewAnimsFrame/Walk_Edit")->getText().c_str();
    m_NewNPCAnimations.Idle = winmgr.getWindow("Editor/NPCNewAnimsFrame/Idle_Edit")->getText().c_str();
    m_NewNPCAnimations.Jump = winmgr.getWindow("Editor/NPCNewAnimsFrame/Jump_Edit")->getText().c_str();
    m_NewNPCAnimations.MeleeAttack = winmgr.getWindow("Editor/NPCNewAnimsFrame/Melee_Edit")->getText().c_str();
    m_NewNPCAnimations.ProjectileAttack = winmgr.getWindow("Editor/NPCNewAnimsFrame/Projectile_Edit")->getText().c_str();
    m_NewNPCAnimations.Death = winmgr.getWindow("Editor/NPCNewAnimsFrame/Death_Edit")->getText().c_str();
    //destroy window
    winmgr.destroyWindow("Editor/NPCNewAnimsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCNewTagsFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewTagsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCNewTagsFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
  {
    //save values
    m_NewNPCTagPoints.HandLeft = winmgr.getWindow("Editor/NPCNewTagsFrame/LeftHand_Edit")->getText().c_str();
    m_NewNPCTagPoints.HandRight = winmgr.getWindow("Editor/NPCNewTagsFrame/RightHand_Edit")->getText().c_str();
    m_NewNPCTagPoints.SheathLeft = winmgr.getWindow("Editor/NPCNewTagsFrame/LeftSheath_Edit")->getText().c_str();
    m_NewNPCTagPoints.SheathRight = winmgr.getWindow("Editor/NPCNewTagsFrame/RightSheath_Edit")->getText().c_str();
    //destroy the window
    winmgr.destroyWindow("Editor/NPCNewTagsFrame");
  }//if
  return true;
}

void EditorApplicationNPC::closeEditWindowsNPC(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewTagsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewTagsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCNewAnimsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewAnimsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/NPCDeleteFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCNewFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCEditFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditFrame/AddInventoryFrame");
  }//if
  //reset tagpoints and animations
  m_NewNPCAnimations = m_EditNPCAnimations = NPCAnimations::getNullAnimations();
  m_NewNPCTagPoints = m_EditNPCTagPoints = NPCTagPoints::getNullTagPoints();
  ID_of_NPC_to_delete = "";
  ID_of_NPC_to_edit = "";
}

void EditorApplicationNPC::showNPCEditWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCEditFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCEditFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit NPC...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/36.0f; //height of one element
    const float d = 0.5f/36.0f; //distance between two adjacent elements
    const float o = 2.0f/36.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for gender
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Gender_Label");
    button->setText("Gender:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    CEGUI::RadioButton* radio = NULL;
    //radio button for male
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCEditFrame/RadioMale"));
    radio->setText("Male");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(true);
    radio->setGroupID(888);
    frame->addChildWindow(radio);
    //radio button for female
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCEditFrame/RadioFemale"));
    radio->setText("Female");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(false);
    radio->setGroupID(888);
    frame->addChildWindow(radio);

    //static text for Attributes
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Attributes_Label");
    button->setText("Attributes");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Inventory
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Inventory_Label");
    button->setText("Inventory");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Str
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Str_Label");
    button->setText("Str.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Strength
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Str_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Level
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Lvl_Label");
    button->setText("Level:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Level
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Level_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Agility
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Agi_Label");
    button->setText("Agi.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Agility
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Agi_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Willpower
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Will_Label");
    button->setText("Will:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Willpower
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Will_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Vitality
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Vit_Label");
    button->setText("Vit.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Vitality
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Vit_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Charisma
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Cha_Label");
    button->setText("Cha.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Charisma
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Cha_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Intelligence
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Int_Label");
    button->setText("Int.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Intelligence
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Int_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Luck
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditFrame/Luck_Label");
    button->setText("Luck:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Luck
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/Luck_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //multi column list for inventory content
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
        (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/NPCEditFrame/InventoryList"));
    mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(4*h+3*d, 0)));
    mcl->addColumn("#", 0, CEGUI::UDim(0.22, 0));
    mcl->addColumn("Item ID", 1, CEGUI::UDim(0.72, 0));
    mcl->setUserColumnDraggingEnabled(false);
    mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditFrameInventoryListClicked, this));
    frame->addChildWindow(mcl);
    //adds a popup menu + callbacks for clicks
    createPopupMenuNPCEditFrameList();

    //animations button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/AnimButton");
    button->setText("Animations...");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditFrameAnimsClicked, this));
    frame->addChildWindow(button);

    //tag points button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/Tagpoints");
    button->setText("Tagpoints...");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditFrameTagpointsClicked, this));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+10*h+10*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+10*h+10*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
  frame->moveToFront();
  const NPCRecord& npc_info = Database::getSingleton().getTypedRecord<NPCRecord>(ID_of_NPC_to_edit);
  //reset animations and tagpoints, to be sure we don't use old values set
  // during a previous edit
  m_EditNPCAnimations = npc_info.Animations;
  m_EditNPCTagPoints = npc_info.TagPoints;
  // -- display NPC values --
  // ---- ID
  winmgr.getWindow("Editor/NPCEditFrame/ID_Edit")->setText(ID_of_NPC_to_edit);
  // ---- name
  winmgr.getWindow("Editor/NPCEditFrame/Name_Edit")->setText(npc_info.Name);
  // ---- mesh
  winmgr.getWindow("Editor/NPCEditFrame/Mesh_Edit")->setText(npc_info.Mesh);
  // ---- gender
  (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/NPCEditFrame/RadioFemale")))->setSelected(npc_info.Female);
  (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/NPCEditFrame/RadioMale")))->setSelected(!npc_info.Female);
  // ---- level
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Level_Spin")))->setCurrentValue(npc_info.Level);
  // ---- attributes
  const NPCAttributes attr = npc_info.Attributes;
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Agi_Spin")))->setCurrentValue(attr.Agi);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Cha_Spin")))->setCurrentValue(attr.Cha);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Int_Spin")))->setCurrentValue(attr.Int);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Luck_Spin")))->setCurrentValue(attr.Luck);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Str_Spin")))->setCurrentValue(attr.Str);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Vit_Spin")))->setCurrentValue(attr.Vit);
  (static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Will_Spin")))->setCurrentValue(attr.Will);
  //inventory
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                       (winmgr.getWindow("Editor/NPCEditFrame/InventoryList"));
  InventoryToMCL(npc_info.InventoryAtStart , mcl);
}

void EditorApplicationNPC::createPopupMenuNPCEditFrameList(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for inventory list
  if (wmgr.isWindowPresent("Editor/NPCEditFrame/InventoryList/PopUp"))
  {
    return;
  }
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/NPCEditFrame/InventoryList/PopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCEditFrame/InventoryList/PopUp/Add"));
  menu_item->setText("Add item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::EditInventoryListAddClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCEditFrame/InventoryList/PopUp/Edit"));
  menu_item->setText("Edit selected item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::EditInventoryListEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCEditFrame/InventoryList/PopUp/Delete"));
  menu_item->setText("Delete selected item");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::EditInventoryListDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/NPCEditFrame/InventoryList")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationNPC::NPCEditFrameInventoryListClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/NPCEditFrame/InventoryList/PopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/NPCEditFrame/InventoryList")->getOuterRectClipper();
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

bool EditorApplicationNPC::NPCEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditFrame");
  }
  if (winmgr.isWindowPresent("Editor/NPCEditTagsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditTagsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCEditAnimsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditAnimsFrame");
  }//if
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditFrame/AddInventoryFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCEditFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Name_Edit") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/RadioFemale") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Str_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Level_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Agi_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Will_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Vit_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Cha_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Int_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/Luck_Spin") and
      winmgr.isWindowPresent("Editor/NPCEditFrame/InventoryList"))
  {
    const std::string NPC_ID = winmgr.getWindow("Editor/NPCEditFrame/ID_Edit")->getText().c_str();
    const std::string NPC_Name = winmgr.getWindow("Editor/NPCEditFrame/Name_Edit")->getText().c_str();
    const std::string NPC_Mesh = winmgr.getWindow("Editor/NPCEditFrame/Mesh_Edit")->getText().c_str();
    if (NPC_ID.empty() or NPC_Name.empty() or NPC_Mesh.empty())
    {
      showHint("You have to enter an ID, Name and Mesh path for the NPC.");
      return true;
    }
    if (NPC_ID!=ID_of_NPC_to_edit)
    {
      if (Database::getSingleton().hasRecord(NPC_ID))
      {
        showHint("An NPC or other record with the ID \""+NPC_ID+"\" already exists. Please "
                +"choose a different ID or delete the other record first.\n");
        return true;
      }//if
    }//if
    else
    {
      //ID remained the same, but the user might have deleted the NPC.
      if (!Database::getSingleton().hasTypedRecord<NPCRecord>(NPC_ID))
      {
        showHint("An NPC with the ID \""+NPC_ID+"\" does not exist. You "
                +"possibly deleted the NPC. Press \"Cancel\", if that is the "
                +"case.\n");
        return true;
      }//if
    }

    const uint8_t level = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/NPCEditFrame/Level_Spin")))->getCurrentValue());
    NPCAttributes attr;
    attr.Str = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Str_Spin")))->getCurrentValue());
    attr.Agi = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Agi_Spin")))->getCurrentValue());
    attr.Vit = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Vit_Spin")))->getCurrentValue());
    attr.Int = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Int_Spin")))->getCurrentValue());
    attr.Will = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Will_Spin")))->getCurrentValue());
    attr.Cha = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Cha_Spin")))->getCurrentValue());
    attr.Luck = static_cast<uint8_t>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCEditFrame/Luck_Spin")))->getCurrentValue());
    const bool female = (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/NPCEditFrame/RadioFemale")))->isSelected();
    const Inventory tempInv = MCLToInventory(static_cast<CEGUI::MultiColumnList*>(winmgr.getWindow("Editor/NPCEditFrame/InventoryList")));

    const bool meshChanged = Database::getSingleton().getTypedRecord<NPCRecord>(ID_of_NPC_to_edit).Mesh != NPC_Mesh;
    const bool idChanged = NPC_ID != ID_of_NPC_to_edit;
    //get changes into database
    NPCRecord npc_rec;
    npc_rec.ID = NPC_ID;
    npc_rec.Name = NPC_Name;
    npc_rec.Mesh = NPC_Mesh;
    npc_rec.Level = level;
    npc_rec.Attributes = attr;
    npc_rec.Female = female;
    npc_rec.InventoryAtStart = tempInv;
    npc_rec.Animations = m_EditNPCAnimations;
    npc_rec.TagPoints = m_EditNPCTagPoints;
    Database::getSingleton().addRecord(npc_rec);
    //update enabled NPCs that are affected by changes
    unsigned int affected_references = 0;
    if (idChanged)
    {
      //change IDs
      affected_references =
      InjectionManager::getSingleton().updateReferencesAfterIDChange(ID_of_NPC_to_edit, NPC_ID, getAPI().getOgreSceneManager());
    }
    else if (meshChanged)
    {
      //update it
      affected_references =
      InjectionManager::getSingleton().reenableReferencesOfObject(ID_of_NPC_to_edit, getAPI().getOgreSceneManager());
    }
    if (meshChanged or idChanged)
    {
      showHint("Changes accepted, "+IntToString(affected_references)+" references have been updated.");
    }
    winmgr.destroyWindow("Editor/NPCEditFrame");
    if (winmgr.isWindowPresent("Editor/NPCEditTagsFrame"))
    {
      winmgr.destroyWindow("Editor/NPCEditTagsFrame");
    }//if
    if (winmgr.isWindowPresent("Editor/NPCEditAnimsFrame"))
    {
      winmgr.destroyWindow("Editor/NPCEditAnimsFrame");
    }//if
    if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame"))
    {
      winmgr.destroyWindow("Editor/NPCEditFrame/AddInventoryFrame");
    }//if
    refreshNPCList();
    ID_of_NPC_to_edit = "";
  }//if
  else
  {
    showHint("Error: At least one required CEGUI window element is not present.\n");
  }
  return true;
}

bool EditorApplicationNPC::NPCEditFrameAnimsClicked(const CEGUI::EventArgs &e)
{
  showNPCEditAnimsWindow();
  return true;
}

bool EditorApplicationNPC::NPCEditFrameTagpointsClicked(const CEGUI::EventArgs &e)
{
  showNPCEditTagpointsWindow();
  return true;
}

bool EditorApplicationNPC::EditInventoryListAddClicked(const CEGUI::EventArgs &e)
{
  showEditInventoryListAddWindow();
  return true;
}

bool EditorApplicationNPC::EditInventoryListEditClicked(const CEGUI::EventArgs &e)
{
  //not implemented yet
  #warning Not implemented yet!
  //not implemented yet
}

bool EditorApplicationNPC::EditInventoryListDeleteClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                         (winmgr.getWindow("Editor/NPCEditFrame/InventoryList"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select an item before you can delete it.");
    return true;
  }
  mcl->removeRow(mcl->getItemRowIndex(lbi));
  return true;
}

void EditorApplicationNPC::showNPCEditAnimsWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditAnimsFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCEditAnimsFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCEditAnimsFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit NPC Animations...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/30.0f; //height of one element
    const float d = 0.5f/30.0f; //distance between two adjacent elements
    const float o = 2.0f/30.0f; //offset of first element

    //static info text
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/NPCEditAnimsFrame/Info_Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(2*h, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    textbox->setText(std::string("Info:\n")
                    +"You can enter multiple animation names, if needed. Just "
                    +"separate them by commatas.");
    frame->addChildWindow(textbox);

    CEGUI::Window * button = NULL;
    //static text for walk
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Walk_Label");
    button->setText("Walk:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for walk
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Walk_Edit");
    button->setText(m_EditNPCAnimations.Walk);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for idle
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Idle_Label");
    button->setText("Idle:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for idle
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Idle_Edit");
    button->setText(m_EditNPCAnimations.Idle);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for melee
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Melee_Label");
    button->setText("Melee:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for melee attack animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Melee_Edit");
    button->setText(m_EditNPCAnimations.MeleeAttack);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for projectile attack animation
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Projectile_Label");
    button->setText("Projectile:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for projectile attack animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Projectile_Edit");
    button->setText(m_EditNPCAnimations.ProjectileAttack);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for jump
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Jump_Label");
    button->setText("Jump:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for jump animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Jump_Edit");
    button->setText(m_EditNPCAnimations.Jump);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for death
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditAnimsFrame/Death_Label");
    button->setText("Death:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for death animation
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditAnimsFrame/Death_Edit");
    button->setText(m_EditNPCAnimations.Death);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditAnimsFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+8*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditAnimsFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditAnimsFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+8*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditAnimsFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.55, 0)));
  frame->moveToFront();
}

void EditorApplicationNPC::showNPCEditTagpointsWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditTagsFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCEditTagsFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCEditTagsFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Change NPC Tag Points...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    const float h = 2.5f/18.0f; //height of one element
    const float d = 0.5f/18.0f; //distance between two adjacent elements
    const float o = 2.0f/18.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text left hand
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditTagsFrame/LeftHand_Label");
    button->setText("Left hand:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for left hand
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditTagsFrame/LeftHand_Edit");
    button->setText(m_EditNPCTagPoints.HandLeft);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text right hand
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditTagsFrame/RightHand_Label");
    button->setText("Right hand:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for right hand
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditTagsFrame/RightHand_Edit");
    button->setText(m_EditNPCTagPoints.HandRight);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text left sheath
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditTagsFrame/LeftSheath_Label");
    button->setText("Left sheath:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for left sheath
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditTagsFrame/LeftSheath_Edit");
    button->setText(m_EditNPCTagPoints.SheathLeft);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text right sheath
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCEditTagsFrame/RightSheath_Label");
    button->setText("Right sheath:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);
    //editbox for right sheath
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCEditTagsFrame/RightSheath_Edit");
    button->setText(m_EditNPCTagPoints.SheathRight);
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditTagsFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditTagsFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditTagsFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+4*(h+d), 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditTagsFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

bool EditorApplicationNPC::NPCEditAnimsFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditAnimsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditAnimsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCEditAnimsFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditAnimsFrame"))
  {
    //save changed animations
    m_EditNPCAnimations.Walk = winmgr.getWindow("Editor/NPCEditAnimsFrame/Walk_Edit")->getText().c_str();
    m_EditNPCAnimations.Idle = winmgr.getWindow("Editor/NPCEditAnimsFrame/Idle_Edit")->getText().c_str();
    m_EditNPCAnimations.Jump = winmgr.getWindow("Editor/NPCEditAnimsFrame/Jump_Edit")->getText().c_str();
    m_EditNPCAnimations.MeleeAttack = winmgr.getWindow("Editor/NPCEditAnimsFrame/Melee_Edit")->getText().c_str();
    m_EditNPCAnimations.ProjectileAttack = winmgr.getWindow("Editor/NPCEditAnimsFrame/Projectile_Edit")->getText().c_str();
    m_EditNPCAnimations.Death = winmgr.getWindow("Editor/NPCEditAnimsFrame/Death_Edit")->getText().c_str();
    //destroy window
    winmgr.destroyWindow("Editor/NPCEditAnimsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCEditTagsFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditTagsFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditTagsFrame");
  }//if
  return true;
}

bool EditorApplicationNPC::NPCEditTagsFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditTagsFrame"))
  {
    //save values
    m_EditNPCTagPoints.HandLeft = winmgr.getWindow("Editor/NPCEditTagsFrame/LeftHand_Edit")->getText().c_str();
    m_EditNPCTagPoints.HandRight = winmgr.getWindow("Editor/NPCEditTagsFrame/RightHand_Edit")->getText().c_str();
    m_EditNPCTagPoints.SheathLeft = winmgr.getWindow("Editor/NPCEditTagsFrame/LeftSheath_Edit")->getText().c_str();
    m_EditNPCTagPoints.SheathRight = winmgr.getWindow("Editor/NPCEditTagsFrame/RightSheath_Edit")->getText().c_str();
    //destroy the window
    winmgr.destroyWindow("Editor/NPCEditTagsFrame");
  }//if
  return true;
}

void EditorApplicationNPC::showEditInventoryListAddWindow(void)
{
  //not implemented yet
  //not implemented yet (adjust size and position of child windows)
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCEditFrame/AddInventoryFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCEditFrame/AddInventoryFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit NPC > Add items to inventory");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //spinner for items
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCEditFrame/AddInventoryFrame/Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.2, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMinimumValue(1.0f);
    spin->setStepSize(1.0f);
    frame->addChildWindow(spin);
    //list box for items
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.createWindow("TaharezLook/Combobox", "Editor/NPCEditFrame/AddInventoryFrame/ItemCombobox"));
    CBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.5, 0)));
    CBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
    CBox->setReadOnly(true);
    frame->addChildWindow(CBox);
    updateItemList(CBox);
    //create Add button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/AddInventoryFrame/Add");
    button->setText("Add");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::EditAddInventoryFrameAddClicked, this));
    //create Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCEditFrame/AddInventoryFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::EditAddInventoryFrameCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.3, 0)));
  frame->moveToFront();
  //not implemented yet
}

bool EditorApplicationNPC::EditAddInventoryFrameAddClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame/Spin") &&
      winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame/ItemCombobox") &&
      winmgr.isWindowPresent("Editor/NPCEditFrame/InventoryList"))
  {
    //add stuff
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*>
      (winmgr.getWindow("Editor/NPCEditFrame/AddInventoryFrame/ItemCombobox"));
    const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
    if (lbi==NULL)
    {
      showHint("You have to select an item from the list. If you don't see any"
              +std::string(" items there, you haven't created any items yet.")
              +" Go to the item tab in the catalogue and create one.");
      return true;
    }
    const CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/NPCEditFrame/AddInventoryFrame/Spin"));
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                (winmgr.getWindow("Editor/NPCEditFrame/InventoryList"));
    //add it to catalogue
    unsigned int row;
    CEGUI::ListboxItem* lbi2 = new CEGUI::ListboxTextItem(IntToString(
                          static_cast<unsigned int>(spin->getCurrentValue())));
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    row = mcl->addRow(lbi2, 0);
    lbi2 = new CEGUI::ListboxTextItem(lbi->getText());
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    mcl->setItem(lbi2, 1, row);
    //destroy window
    winmgr.destroyWindow("Editor/NPCEditFrame/AddInventoryFrame");
    return true;
  }
  return true;
}

bool EditorApplicationNPC::EditAddInventoryFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCEditFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCEditFrame/AddInventoryFrame");
  }
  return true;
}

void EditorApplicationNPC::createNPCCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //NPC tab
  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/NPC");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("NPCs");
  tab->addTab(pane);

  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/NPC/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Name", 1, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Level", 2, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Female", 3, CEGUI::UDim(0.19, 0));
  mcl->addColumn("Mesh", 4, CEGUI::UDim(0.19, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCTabClicked, this));

  //Sample data
  // ---- no data yet, just refresh the list
  refreshNPCList();
}

} //namespace
