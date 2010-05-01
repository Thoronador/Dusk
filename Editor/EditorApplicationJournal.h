/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-01
 Purpose: EditorApplicationJournal class
          EditorApplicationJournal is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Journal entries.

 History:
     - 2010-05-01 (rev 195)  - initial version (by thoronador)

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
