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

#include "EditorApplicationJournal.h"
#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>
#include "../Engine/DuskFunctions.h"
#include "../Engine/Journal.h"

namespace Dusk
{

EditorApplicationJournal::EditorApplicationJournal()
{
  //empty
  ID_of_quest_to_delete = "";
  ID_of_quest_to_rename = "";
  ID_of_quest_to_add_entry = "";
  Index_of_entry_to_edit = 0;
  QuestID_of_entry_to_edit = "";
  Index_of_entry_to_delete = 0;
  QuestID_of_entry_to_delete = "";
}

 EditorApplicationJournal::~EditorApplicationJournal()
 {
   //empty
 }

 bool EditorApplicationJournal::JournalEntryNewClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
  const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have not selected a quest from the drop down list!");
    return true;
  }
  if (std::string(lbi->getText().c_str())=="(none)")
  {
    showHint("You have not selected a quest from the drop down list!");
    return true;
  }
  ID_of_quest_to_add_entry = std::string(lbi->getText().c_str());
  showQuestEntryNewWindow();
  return true;
}

bool EditorApplicationJournal::JournalEntryEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
  const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have not selected a quest from the drop down list!");
    return true;
  }
  if (std::string(lbi->getText().c_str())=="(none)")
  {
    showHint("You have not selected a valid quest from the drop down list!");
    return true;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>(winmgr.getWindow("Editor/JournalFrame/EntryList"));
  CEGUI::ListboxItem* index_item = mcl->getFirstSelectedItem();
  if (index_item==NULL)
  {
    showHint("You have not selected a quest from the entry list on the right side!");
    return true;
  }
  const unsigned int row = mcl->getItemRowIndex(index_item);
  index_item = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row, 0));
  const unsigned int i = StringToInt(index_item->getText().c_str(), 0);
  if (i==0)
  {
    showHint("Got invalid index for the selected entry!");
    return true;
  }
  QuestID_of_entry_to_edit = std::string(lbi->getText().c_str());
  Index_of_entry_to_edit = i;
  showQuestEntryEditWindow();
  return true;
}

bool EditorApplicationJournal::JournalEntryDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox")
     and winmgr.isWindowPresent("Editor/JournalFrame/EntryList"))
  {
    const CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
    const CEGUI::ListboxItem* cb_lbi = CBox->getSelectedItem();
    if (cb_lbi==NULL)
    {
      showHint("You have to select a quest from the list before you can delete"
              +std::string(" entries."));
      return true;
    }

    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/JournalFrame/EntryList"));
    CEGUI::ListboxItem* mcl_lbi = mcl->getFirstSelectedItem();
    if (mcl_lbi==NULL)
    {
      showHint("You have to select a quest entry from the list to delete it.");
      return true;
    }

    unsigned int row_index = mcl->getItemRowIndex(mcl_lbi);
    mcl_lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    Index_of_entry_to_delete = StringToInt(std::string(mcl_lbi->getText().c_str()), 0);
    QuestID_of_entry_to_delete = std::string(cb_lbi->getText().c_str());
    showQuestEntryConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationJournal::showQuestEntryConfirmDeleteWindow(void)
{
  if (QuestID_of_entry_to_delete=="" or Index_of_entry_to_delete==0)
  {
    showHint("Quest ID is empty or index is zero!");
    return;
  } //if
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/QuestEntryConfirmDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/QuestEntryConfirmDeleteFrame"));
  }
  else
  {
    //create frame
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/QuestEntryConfirmDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Confirm deletion of quest entry");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add multilinebox for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/QuestEntryConfirmDeleteFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create Yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/QuestEntryConfirmDeleteFrame/Yes");
    button->setText("Yes");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::QuestEntryConfirmDeleteYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/QuestEntryConfirmDeleteFrame/No");
    button->setText("No!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::QuestEntryConfirmDeleteNoClicked, this));

  }
  winmgr.getWindow("Editor/QuestEntryConfirmDeleteFrame/Text")->setText("Do you"
                  +std::string(" really want to delete the entry with ID ")
                  +IntToString(Index_of_entry_to_delete)+" of quest \""
                  +QuestID_of_entry_to_delete+"\"?");
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.25, 0)));
  frame->moveToFront();
}

bool EditorApplicationJournal::QuestEntryConfirmDeleteNoClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/QuestEntryConfirmDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/QuestEntryConfirmDeleteFrame");
    QuestID_of_entry_to_delete = "";
    Index_of_entry_to_delete = 0;
  }
  return true;
}

bool EditorApplicationJournal::QuestEntryConfirmDeleteYesClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/QuestEntryConfirmDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/QuestEntryConfirmDeleteFrame");
    if ( Journal::getSingleton().deleteEntry(QuestID_of_entry_to_delete, Index_of_entry_to_delete))
    {
      showHint("Entry successfully deleted.");
      if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
      {
        const CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
        const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
        if (lbi!=NULL)
        {
          UpdateQuestEntryList(std::string(lbi->getText().c_str()));
        }
      }
    }
    else
    {
      showHint("Could not delete entry!");
    }
    QuestID_of_entry_to_delete = "";
    Index_of_entry_to_delete = 0;
  }
  return true;
}

void EditorApplicationJournal::UpdateQuestList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
  {
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
    CBox->resetList();
    CEGUI::ListboxTextItem* lbi;
    std::vector<std::string> qVec = Journal::getSingleton().listAllQuestIDs();
    unsigned int i;
    for (i=0; i<qVec.size(); ++i)
    {
      lbi = new CEGUI::ListboxTextItem(qVec[i]);
      CBox->addItem(lbi);
    }//for
    if (qVec.empty())
    {
      lbi = new CEGUI::ListboxTextItem("(none)");
      CBox->addItem(lbi);
    }
    CBox->setText("");
  }
}

void EditorApplicationJournal::UpdateQuestEntryList(const std::string& questID)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalFrame/EntryList"))
  {
    CEGUI::MultiColumnList* mcl;
    mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/JournalFrame/EntryList"));
    mcl->resetList();
    std::vector<unsigned int> indexVec;
    indexVec.clear();
    indexVec = Journal::getSingleton().listAllIndicesOfQuest(questID);
    CEGUI::ListboxItem *lbi;
    unsigned int i, row;
    for (i=0; i<indexVec.size(); ++i)
    {
      lbi = new CEGUI::ListboxTextItem(IntToString(indexVec[i]));
      lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
      row = mcl->addRow(lbi, 0);
      lbi = new CEGUI::ListboxTextItem(JournalRecord::flagsToString(Journal::getSingleton().getFlags(questID, indexVec[i])));
      lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
      mcl->setItem(lbi, 1, row);
      lbi = new CEGUI::ListboxTextItem(Journal::getSingleton().getText(questID, indexVec[i]));
      lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
      mcl->setItem(lbi, 2, row);
    }//for
  }
}

bool EditorApplicationJournal::JournalFrameNewQuestClicked(const CEGUI::EventArgs &e)
{
  //we just a quest with generic ID and name to the list
  unsigned int i = 0;
  while (Journal::getSingleton().hasQuest("new_quest"+IntToString(i)))
  {
    ++i;
  }//while
  Journal::getSingleton().setQuestName("new_quest"+IntToString(i),
                                       "new quest #"+IntToString(i));
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
  CEGUI::ListboxTextItem* lbi = new CEGUI::ListboxTextItem("new_quest"+IntToString(i));
  CBox->addItem(lbi);
  return true;
}

bool EditorApplicationJournal::JournalFrameDeleteQuestClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
  CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have not selected a quest from the list!");
    return true;
  }
  if (lbi->getText()=="(none)" or !Journal::getSingleton().hasQuest(std::string(lbi->getText().c_str())))
  {
    showHint("You have not selected a valid quest!");
    return true;
  }
  ID_of_quest_to_delete = std::string(lbi->getText().c_str());
  showJournalConfirmDeleteQuestWindow();
  return true;
}

bool EditorApplicationJournal::JournalFrameRenameQuestClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
  CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have not selected a quest from the list!");
    return true;
  }
  if (lbi->getText()=="(none)" or !Journal::getSingleton().hasQuest(std::string(lbi->getText().c_str())))
  {
    showHint("You have not selected a valid quest from the list!");
    return true;
  }
  ID_of_quest_to_rename = std::string(lbi->getText().c_str());
  showJournalRenameQuestWindow();
  return true;
}

bool EditorApplicationJournal::JournalFrameCloseClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalFrame"))
  {
    winmgr.destroyWindow("Editor/JournalFrame");
  }
  return true;
}

void EditorApplicationJournal::showJournalConfirmDeleteQuestWindow(void)
{
  if (ID_of_quest_to_delete=="") return;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (!winmgr.isWindowPresent("Editor/JournalDeleteQuestFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/JournalDeleteQuestFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Deleting a quest");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/JournalDeleteQuestFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);
    //button "yes"
    CEGUI::Window* button;
    button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalDeleteQuestFrame/Yes");
    button->setText("Yes, do it.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalDeleteQuestFrameYesClicked, this));
    //button "no"
    button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalDeleteQuestFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalDeleteQuestFrameNoClicked, this));
  }
  else
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/JournalDeleteQuestFrame"));
  }
  winmgr.getWindow("Editor/JournalDeleteQuestFrame/Text")->setText(
                   std::string("Do you really want to delete the quest \"")
                  +ID_of_quest_to_delete+"\" and all related journal entries?");
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.3, 0)));
  frame->moveToFront();
}

bool EditorApplicationJournal::JournalDeleteQuestFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_quest_to_delete!="")
  {
    if (Journal::getSingleton().deleteQuest(ID_of_quest_to_delete))
    {
      showHint("Quest with ID \""+ID_of_quest_to_delete+"\" was successfully "
              +"deleted.");
      //remove the deleted quest from list via update
      UpdateQuestList();
    }
    else
    {
      showHint("There is no quest with ID \""+ID_of_quest_to_delete+"\", thus "
              +"nothing will be deleted!");
    }
    ID_of_quest_to_delete = "";
  }
  else
  {
    showHint("No quest ID specified for deletion!");
  }
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalDeleteQuestFrame"))
  {
    winmgr.destroyWindow("Editor/JournalDeleteQuestFrame");
  }
  return true;
}

bool EditorApplicationJournal::JournalDeleteQuestFrameNoClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalDeleteQuestFrame"))
  {
    winmgr.destroyWindow("Editor/JournalDeleteQuestFrame");
  }
  return true;
}

void EditorApplicationJournal::showQuestEntryNewWindow(void)
{
  if (ID_of_quest_to_add_entry == "") return;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame = NULL;
  if (winmgr.isWindowPresent("Editor/NewQuestEntryFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NewQuestEntryFrame"));
  }
  else
  {
    //create stuff here
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NewQuestEntryFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("New quest entry...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for showing quest ID
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NewQuestEntryFrame/QID_Label");
    button->setText("Quest ID: \""+ID_of_quest_to_add_entry+"\"");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for index
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NewQuestEntryFrame/Index_Label");
    button->setText("Index:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.25, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for index
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NewQuestEntryFrame/Index_Edit");
    button->setText(IntToString(Journal::getSingleton().getMaximumAvailabeIndex(ID_of_quest_to_add_entry)+1));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.25, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //checkbox for Finished flag
    CEGUI::Checkbox* check;
    check = static_cast<CEGUI::Checkbox*> (winmgr.createWindow("TaharezLook/Checkbox", "Editor/NewQuestEntryFrame/Finished"));
    check->setText("Finishes Quest");
    check->setSelected(false);
    check->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.25, 0)));
    check->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(check);

    //static text for text
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NewQuestEntryFrame/Text_Label");
    button->setText("Text:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for quest entry text
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/NewQuestEntryFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.3, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.55, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(false);
    textbox->setText("> put text here <");
    frame->addChildWindow(textbox);

    //button OK
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NewQuestEntryFrame/OK");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.875, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::NewQuestEntryFrameOKClicked, this));

    //button Cancel
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NewQuestEntryFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.875, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::NewQuestEntryFrameCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();
}

bool EditorApplicationJournal::NewQuestEntryFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NewQuestEntryFrame")
     && winmgr.isWindowPresent("Editor/NewQuestEntryFrame/Text")
     && winmgr.isWindowPresent("Editor/NewQuestEntryFrame/Index_Edit")
     && winmgr.isWindowPresent("Editor/NewQuestEntryFrame/Finished"))
  {
    CEGUI::Editbox* IndexEdit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/NewQuestEntryFrame/Index_Edit"));
    if (StringToInt(IndexEdit->getText().c_str(), 0)<=0)
    {
      showWarning("Invalid index! The index has to be an integer value greater"
                 +std::string(" than zero!"));
      return true;
    }
    const unsigned int newIndex = StringToInt(IndexEdit->getText().c_str(), 0);
    if (Journal::getSingleton().hasEntry(ID_of_quest_to_add_entry, newIndex))
    {
      showWarning("An entry with ID "+IntToString(newIndex)+" for quest \""
                  +ID_of_quest_to_add_entry+"\" already exists. Please choose"
                  +" a different index.");
      return true;
    }
    const std::string newText = trim(winmgr.getWindow("Editor/NewQuestEntryFrame/Text")->getText().c_str());
    if (newText=="" or newText == "> put text here <")
    {
      showHint("Please enter the text for this entry!");
      return true;
    }
    uint8 flags = 0;
    CEGUI::Checkbox* check = static_cast<CEGUI::Checkbox*> (winmgr.getWindow("Editor/NewQuestEntryFrame/Finished"));
    if (check->isSelected())
    {
      flags = JournalRecord::FinishedFlag;
    }
    //add it to journal
    if (Journal::getSingleton().addEntry(ID_of_quest_to_add_entry, newIndex, newText, flags))
    {
      showHint("New entry added successfully!");
      winmgr.destroyWindow("Editor/NewQuestEntryFrame");
      const CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.getWindow("Editor/JournalFrame/QuestCombobox"));
      const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
      if (lbi!=NULL)
      {
        UpdateQuestEntryList(std::string(lbi->getText().c_str()));
      }
      ID_of_quest_to_add_entry = "";
    }
    else
    {
      showHint("Could not create new entry!");
    }
  } //if
  return true;
}

bool EditorApplicationJournal::NewQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NewQuestEntryFrame"))
  {
    winmgr.destroyWindow("Editor/NewQuestEntryFrame");
    ID_of_quest_to_add_entry = "";
  }
  return true;
}

void EditorApplicationJournal::showQuestEntryEditWindow(void)
{
  if ( QuestID_of_entry_to_edit == "" or Index_of_entry_to_edit==0) return;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::Checkbox* check;
  if (winmgr.isWindowPresent("Editor/EditQuestEntryFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/EditQuestEntryFrame"));
  }
  else
  {
    //create stuff here
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/EditQuestEntryFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Edit quest entry...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for showing quest ID
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/StaticText", "Editor/EditQuestEntryFrame/QID_Label");
    button->setText("Quest ID: \""+QuestID_of_entry_to_edit+"\"");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for index
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/EditQuestEntryFrame/Index_Label");
    button->setText("Index:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.25, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for index
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/EditQuestEntryFrame/Index_Edit");
    button->setText(IntToString(Index_of_entry_to_edit));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.25, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //checkbox for Finished flag
    check = static_cast<CEGUI::Checkbox*> (winmgr.createWindow("TaharezLook/Checkbox", "Editor/EditQuestEntryFrame/Finished"));
    check->setText("Finishes Quest");
    check->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.25, 0)));
    check->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(check);

    //static text for text
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/EditQuestEntryFrame/Text_Label");
    button->setText("Text:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for quest entry text
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/EditQuestEntryFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.3, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.55, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(false);
    textbox->setText("(no text given)");
    frame->addChildWindow(textbox);

    //button OK
    button = winmgr.createWindow("TaharezLook/Button", "Editor/EditQuestEntryFrame/OK");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.875, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::EditQuestEntryFrameOKClicked, this));

    //button Cancel
    button = winmgr.createWindow("TaharezLook/Button", "Editor/EditQuestEntryFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.875, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::EditQuestEntryFrameCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.22, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();

  winmgr.getWindow("Editor/EditQuestEntryFrame/QID_Label")->setText(
                     "Quest ID: \""+QuestID_of_entry_to_edit+"\"");
  winmgr.getWindow("Editor/EditQuestEntryFrame/Index_Edit")->setText(
                     IntToString(Index_of_entry_to_edit));
  check = static_cast<CEGUI::Checkbox*> (winmgr.getWindow("Editor/EditQuestEntryFrame/Finished"));
  check->setSelected((Journal::getSingleton().getFlags(QuestID_of_entry_to_edit,
                     Index_of_entry_to_edit)&JournalRecord::FinishedFlag)>0);
  winmgr.getWindow("Editor/EditQuestEntryFrame/Text")->setText(
      Journal::getSingleton().getText(QuestID_of_entry_to_edit,Index_of_entry_to_edit));
}

bool EditorApplicationJournal::EditQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/EditQuestEntryFrame"))
  {
    winmgr.destroyWindow("Editor/EditQuestEntryFrame");
    QuestID_of_entry_to_edit = "";
    Index_of_entry_to_edit = 0;
  }
  return true;
}

bool EditorApplicationJournal::EditQuestEntryFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/EditQuestEntryFrame") or
      !winmgr.isWindowPresent("Editor/EditQuestEntryFrame/Index_Edit") or
      !winmgr.isWindowPresent("Editor/EditQuestEntryFrame/Finished") or
      !winmgr.isWindowPresent("Editor/EditQuestEntryFrame/Text"))
  {
    return true;
  }
  JournalRecord temp;
  temp.Text = winmgr.getWindow("Editor/EditQuestEntryFrame/Text")->getText().c_str();
  if (temp.Text.empty())
  {
    showHint("You have to enter some text for this journal entry!");
    return true;
  }
  CEGUI::Checkbox* check = static_cast<CEGUI::Checkbox*> (winmgr.getWindow("Editor/EditQuestEntryFrame/Finished"));
  if (check->isSelected())
  {
    temp.Flags = JournalRecord::FinishedFlag;
  }
  else
  {
    temp.Flags = 0;
  }
  const unsigned int new_index = StringToInt(winmgr.getWindow("Editor/EditQuestEntryFrame/Index_Edit")->getText().c_str(), 0);
  if (new_index==0)
  {
    showHint("Please enter a positive integer value for the index!");
    return true;
  }
  //Did the user change the index?
  if (new_index!=Index_of_entry_to_edit)
  {
    if (Journal::getSingleton().hasEntry(QuestID_of_entry_to_edit, new_index))
    {
      showHint("You are trying to change the index of this entry, but an entry "
               +std::string("with the same index already exists for the quest \"")
               +QuestID_of_entry_to_edit+"\". Please choose a different index.");
      return true;
    }
    //we have a valid index change here, so remove old index first...
    if (!Journal::getSingleton().deleteEntry(QuestID_of_entry_to_edit, Index_of_entry_to_edit))
    {
      showWarning("An error occured while changing the index of the entry!");
      QuestID_of_entry_to_edit = "";
      Index_of_entry_to_edit = 0;
      winmgr.destroyWindow("Editor/EditQuestEntryFrame");
      //update entry list
      if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
      {
        UpdateQuestEntryList(winmgr.getWindow("Editor/JournalFrame/QuestCombobox")->getText().c_str());
      }
      return true;
    }
    //...and add new index after that.
    if (!Journal::getSingleton().addEntry(QuestID_of_entry_to_edit, new_index, temp))
    {
      showWarning("An error occured while adding the modified entry!");
    }
    QuestID_of_entry_to_edit = "";
    Index_of_entry_to_edit = 0;
    winmgr.destroyWindow("Editor/EditQuestEntryFrame");
    //update entry list
    if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
    {
      UpdateQuestEntryList(winmgr.getWindow("Editor/JournalFrame/QuestCombobox")->getText().c_str());
    }
    return true;
  }
  //add entry
  if (!Journal::getSingleton().addEntry(QuestID_of_entry_to_edit,
        Index_of_entry_to_edit, temp))
  {
    showWarning("Could not change entry!");
  }
  QuestID_of_entry_to_edit = "";
  Index_of_entry_to_edit = 0;
  winmgr.destroyWindow("Editor/EditQuestEntryFrame");
  //update entry list
  if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
  {
    UpdateQuestEntryList(winmgr.getWindow("Editor/JournalFrame/QuestCombobox")->getText().c_str());
  }
  return true;
}

void EditorApplicationJournal::showJournalRenameQuestWindow(void)
{
  if (ID_of_quest_to_rename=="") return;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame = NULL;
  if (winmgr.isWindowPresent("Editor/QuestRenameFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/QuestRenameFrame"));
  }
  else
  {
    //create new frame window
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/QuestRenameFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Rename Quest \""+ID_of_quest_to_rename+"\"");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    CEGUI::Window* window;
    //static text
    window = winmgr.createWindow("TaharezLook/StaticText", "Editor/QuestRenameFrame/QID_Label");
    window->setText("Quest ID:");
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(window);

    //editbox for quest ID
    window = winmgr.createWindow("TaharezLook/Editbox", "Editor/QuestRenameFrame/QID_Edit");
    window->setText(ID_of_quest_to_rename);
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.15, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(window);

    //static text for quest name
    window = winmgr.createWindow("TaharezLook/StaticText", "Editor/QuestRenameFrame/Name_Label");
    window->setText("Name:");
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.45, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(window);

    //editbox for quest name
    window = winmgr.createWindow("TaharezLook/Editbox", "Editor/QuestRenameFrame/Name_Edit");
    window->setText(Journal::getSingleton().getQuestName(ID_of_quest_to_rename));
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.45, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(window);

    //OK button
    window = winmgr.createWindow("TaharezLook/Button", "Editor/QuestRenameFrame/OK");
    window->setText("OK");
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.75, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    window->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::QuestRenameFrameOKClicked, this));
    frame->addChildWindow(window);

    //Cancel button
    window = winmgr.createWindow("TaharezLook/Button", "Editor/QuestRenameFrame/Cancel");
    window->setText("Cancel");
    window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.75, 0)));
    window->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    window->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::QuestRenameFrameCancelClicked, this));
    frame->addChildWindow(window);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.3, 0)));
  frame->moveToFront();
}

bool EditorApplicationJournal::QuestRenameFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/QuestRenameFrame"))
  {
    winmgr.destroyWindow("Editor/QuestRenameFrame");
  }
  ID_of_quest_to_rename = "";
  return true;
}

bool EditorApplicationJournal::QuestRenameFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/QuestRenameFrame/Name_Edit") &&
      winmgr.isWindowPresent("Editor/QuestRenameFrame/QID_Edit") &&
      ID_of_quest_to_rename!="")
  {
    const std::string EditQID =
        winmgr.getWindow("Editor/QuestRenameFrame/QID_Edit")->getText().c_str();
    if (EditQID == "")
    {
      showHint("You have to enter a quest ID!");
      return true;
    }
    const std::string EditName =
        winmgr.getWindow("Editor/QuestRenameFrame/Name_Edit")->getText().c_str();
    if (EditName == "")
    {
      showHint("You have to enter a quest name!");
      return true;
    }
    if (ID_of_quest_to_rename==EditQID)
    {
      //just quest name changed
      Journal::getSingleton().setQuestName(ID_of_quest_to_rename, EditName);
    }
    else
    {
      //user tries to change quest ID
      if (Journal::getSingleton().hasQuest(EditQID))
      {
        showWarning("A quest with the given ID already exists. Please rename "
                +std::string("the existing quest or choose another quest ID!"));
        return true;
      }
      //now we change the quest ID
      if (!Journal::getSingleton().changeQuestID(ID_of_quest_to_rename, EditQID))
      {
        showWarning("Could not change quest ID!");
        return true;
      }
      UpdateQuestList();
    }
    //finally update the quest's name
    Journal::getSingleton().setQuestName(EditQID, EditName);
  }
  winmgr.destroyWindow("Editor/QuestRenameFrame");
  ID_of_quest_to_rename = "";
  return true;
}

void EditorApplicationJournal::showJournalWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/JournalFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/JournalFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/JournalFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Journal Editor");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    CEGUI::MultiColumnList* mcl;
    mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/JournalFrame/EntryList"));

    mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.9, 0)));
    mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.05, 0)));
    mcl->addColumn("Index", 0, CEGUI::UDim(0.15, 0));
    mcl->addColumn("Flags", 1, CEGUI::UDim(0.15, 0));
    mcl->addColumn("Text", 2, CEGUI::UDim(0.65, 0));
    mcl->setUserColumnDraggingEnabled(false);
    mcl->setUserColumnSizingEnabled(true);
    mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalEntryListClicked, this));
    frame->addChildWindow(mcl);

    //popup for quest entries
    CEGUI::PopupMenu* popup;
    popup = static_cast<CEGUI::PopupMenu*> (winmgr.createWindow("TaharezLook/PopupMenu", "Editor/JournalFrame/EntryPopUp"));
    popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
    popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
    CEGUI::MenuItem* menu_item;
    menu_item = static_cast<CEGUI::MenuItem*> (winmgr.createWindow("TaharezLook/MenuItem", "Editor/JournalFrame/EntryPopUp/New"));
    menu_item->setText("New entry...");
    menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalEntryNewClicked, this));
    popup->addItem(menu_item);
    menu_item = static_cast<CEGUI::MenuItem*> (winmgr.createWindow("TaharezLook/MenuItem", "Editor/JournalFrame/EntryPopUp/Edit"));
    menu_item->setText("Edit selected entry...");
    menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalEntryEditClicked, this));
    popup->addItem(menu_item);
    menu_item = static_cast<CEGUI::MenuItem*> (winmgr.createWindow("TaharezLook/MenuItem", "Editor/JournalFrame/EntryPopUp/Delete"));
    menu_item->setText("Delete selected entry");
    menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalEntryDeleteClicked, this));
    popup->addItem(menu_item);
    mcl->addChildWindow(popup);
    popup->closePopupMenu();

    //combobox/ dropdown for quest IDs
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.createWindow("TaharezLook/Combobox", "Editor/JournalFrame/QuestCombobox"));
    CBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.5, 0)));
    CBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.025, 0), CEGUI::UDim(0.07, 0)));
    CBox->setReadOnly(true);
    CBox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalFrameComboboxChanged, this));
    frame->addChildWindow(CBox);

    UpdateQuestList();

    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalFrame/NewQuestButton");
    button->setText("New Quest");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.025, 0), CEGUI::UDim(0.2, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalFrameNewQuestClicked, this));
    button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalFrame/DeleteQuestButton");
    button->setText("Delete Quest");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.025, 0), CEGUI::UDim(0.35, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalFrameDeleteQuestClicked, this));
    button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalFrame/RenameQuestButton");
    button->setText("Rename Quest");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.025, 0), CEGUI::UDim(0.50, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalFrameRenameQuestClicked, this));
    button = winmgr.createWindow("TaharezLook/Button", "Editor/JournalFrame/CloseButton");
    button->setText("Close");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.025, 0), CEGUI::UDim(0.85, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationJournal::JournalFrameCloseClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.7, 0), CEGUI::UDim(0.7, 0)));
  frame->moveToFront();
}

bool EditorApplicationJournal::JournalFrameComboboxChanged(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/JournalFrame/QuestCombobox"))
  {
    UpdateQuestEntryList(winmgr.getWindow("Editor/JournalFrame/QuestCombobox")->getText().c_str());
  }
  return true;
}

bool EditorApplicationJournal::JournalEntryListClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/JournalFrame/EntryPopUp"))
  {
    return true;
  }
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/JournalFrame/EntryPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/JournalFrame/EntryList")->getPixelRect();
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
