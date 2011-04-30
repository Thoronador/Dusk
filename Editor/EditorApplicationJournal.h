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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-01
 Purpose: EditorApplicationJournal class
          EditorApplicationJournal is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Journal entries.

 History:
     - 2010-05-01 (rev 195)  - initial version (by thoronador)
     - 2011-03-25 (rev 281)  - flag for quest failure can now be set, too
     - 2011-04-30 (rev 282)  - separate window for creating new quests

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONJOURNAL_H
#define EDITORAPPLICATIONJOURNAL_H

#include <string>
#include <CEGUI/CEGUIEventArgs.h>

namespace Dusk
{

class EditorApplicationJournal
{
  public:
    //constructor
    EditorApplicationJournal();
    //destructor
    virtual ~EditorApplicationJournal();
  protected:
    //journal
    std::string ID_of_quest_to_delete;
    std::string ID_of_quest_to_rename;

    std::string ID_of_quest_to_add_entry;
    unsigned int Index_of_entry_to_edit;
    std::string QuestID_of_entry_to_edit;

    unsigned int Index_of_entry_to_delete;
    std::string QuestID_of_entry_to_delete;

    //callbacks for popup menus
    bool JournalEntryNewClicked(const CEGUI::EventArgs &e);
    bool JournalEntryEditClicked(const CEGUI::EventArgs &e);
    bool JournalEntryDeleteClicked(const CEGUI::EventArgs &e);

    //window for journal entries
    void showJournalWindow(void);
    void showJournalNewQuestWindow(void);
    void showJournalConfirmDeleteQuestWindow(void);
    void showJournalRenameQuestWindow(void);
    void showQuestEntryNewWindow(void);
    void showQuestEntryConfirmDeleteWindow(void);
    void showQuestEntryEditWindow(void);

    //loads all quest IDs into list/ combobox in JournalFrame
    void UpdateQuestList(void);
    void UpdateQuestEntryList(const std::string& questID);

    bool JournalEntryListClicked(const CEGUI::EventArgs &e);

    //callbacks of window to delete quest entries
    bool QuestEntryConfirmDeleteNoClicked(const CEGUI::EventArgs &e);
    bool QuestEntryConfirmDeleteYesClicked(const CEGUI::EventArgs &e);
    //callbacks for Journal window
    bool JournalFrameNewQuestClicked(const CEGUI::EventArgs &e);
    bool JournalFrameDeleteQuestClicked(const CEGUI::EventArgs &e);
    bool JournalFrameRenameQuestClicked(const CEGUI::EventArgs &e);
    bool JournalFrameCloseClicked(const CEGUI::EventArgs &e);
    bool JournalFrameComboboxChanged(const CEGUI::EventArgs &e);
    //callbacks for quest creation window
    bool JournalNewQuestFrameCreateClicked(const CEGUI::EventArgs &e);
    bool JournalNewQuestFrameCancelClicked(const CEGUI::EventArgs &e);
    //callbacks for quest deletion window
    bool JournalDeleteQuestFrameYesClicked(const CEGUI::EventArgs &e);
    bool JournalDeleteQuestFrameNoClicked(const CEGUI::EventArgs &e);
    //callbacks for new quest entry window
    bool NewQuestEntryFrameOKClicked(const CEGUI::EventArgs &e);
    bool NewQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e);
    //callbacks for edit quest entry window
    bool EditQuestEntryFrameOKClicked(const CEGUI::EventArgs &e);
    bool EditQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e);
    //callbacks for rename quest window
    bool QuestRenameFrameCancelClicked(const CEGUI::EventArgs &e);
    bool QuestRenameFrameOKClicked(const CEGUI::EventArgs &e);
}; //class

} //namespace

#endif // EDITORAPPLICATIONJOURNAL_H
