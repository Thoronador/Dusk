/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-13
 Purpose: EditorApplicationNPC class
          EditorApplicationNPC is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing NPCs.

 History:
     - 2010-04-28 (rev 191) - initial version (by thoronador)
     - 2010-04-28 (rev 192) - dependency to EditorApplicationBase removed
     - 2010-05-05 (rev 196) - new NPCs can now be created
     - 2010-05-13 (rev 202) - new NPCs can now have items in their inventory

 ToDo list:
     - refine possibility to create NPCs (inventory)
     - implement possibility to edit NPCs
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONNPC_H
#define EDITORAPPLICATIONNPC_H

#include <string>
#include <CEGUI/CEGUIEventArgs.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIMultiColumnList.h>
#include "../Engine/NPCBase.h"

namespace Dusk
{

class EditorApplicationNPC
{
  public:
    // constructor
    EditorApplicationNPC();

    //destructor
    virtual ~EditorApplicationNPC();
  protected:
    //ID for NPC deletion
    std::string ID_of_NPC_to_delete;

    /* clears NPCs in catalogue and re-lists every present NPC */
    void RefreshNPCList(void);
    //windows for creating/ editing NPCs
    void showNPCNewWindow(void);
	void showNPCConfirmDeleteWindow(void);

	//method to visually add a NPC record to the catalogue
	void addNPCRecordToCatalogue(const std::string& ID, const NPCRecord& Record);

    //create the popup menu for the NPC tab
    void CreatePopupMenuNPCTab(void);
	//callbacks for popup menu
	bool NPCNewClicked(const CEGUI::EventArgs &e);
	bool NPCEditClicked(const CEGUI::EventArgs &e);
	bool NPCDeleteClicked(const CEGUI::EventArgs &e);

	//callbacks of window to delete NPCs
	bool NPCDeleteFrameNoClicked(const CEGUI::EventArgs &e);
	bool NPCDeleteFrameYesClicked(const CEGUI::EventArgs &e);
	//callbacks of window for creating new NPCs
	bool NPCNewFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCNewFrameOKClicked(const CEGUI::EventArgs &e);

	//create popup menu for inventory list in NPCNewFrame
	void CreatePopupMenuNPCNewFrameList(void);
	//callbacks for popup menu
    bool InventoryListAddClicked(const CEGUI::EventArgs &e);
    bool InventoryListEditClicked(const CEGUI::EventArgs &e);
    bool InventoryListDeleteClicked(const CEGUI::EventArgs &e);

    //window for adding new items to inventory list
    void showInventoryListAddWindow(void);
    //callbacks for that window
    bool AddInventoryFrameAddClicked(const CEGUI::EventArgs &e);
    bool AddInventoryFrameCancelClicked(const CEGUI::EventArgs &e);

    //helper function to list all available item IDs in a Combobox
    void UpdateItemList(CEGUI::Combobox* combo);

    //gets data from MCL and transforms it into inventory
    Inventory MCLToInventory(const CEGUI::MultiColumnList* mcl);

	//callback for clicking inventory list in NPCNewFrame
	bool NPCNewFrameInventoryListClicked(const CEGUI::EventArgs &e);
}; //class

} //namespace

#endif // EDITORAPPLICATIONNPC_H