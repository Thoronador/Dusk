/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2010, 2011 thoronador

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
 Date:    2010-04-28
 Purpose: EditorApplicationNPC class
          EditorApplicationNPC is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing NPCs.

 History:
     - 2010-04-28 (rev 191) - initial version (by thoronador)
     - 2010-04-28 (rev 192) - dependency to EditorApplicationBase removed
     - 2010-05-05 (rev 196) - new NPCs can now be created
     - 2010-05-13 (rev 202) - new NPCs can now have items in their inventory
     - 2010-06-03 (rev 214) - small improvements to get rid of compiler warnings
     - 2010-08-31 (rev 239) - adjustemts (due to change of some function names
                              in some Engine classes)
     - 2010-09-03 (rev 240) - new NPCs can now have tag point names and
                              animation names
                            - closeEditWindowsNPC() added
     - 2010-09-10 (rev 241) - preparations for editing NPCs
     - 2010-09-13 (rev 242) - editing NPCs is now possible
     - 2011-05-10 (rev 285) - createNPCCatalogueTab() an NPCTabClicked() added
                              (moved code from EditorApplication)

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
    //ID for editing
    std::string ID_of_NPC_to_edit;

    NPCTagPoints m_NewNPCTagPoints;
    NPCAnimations m_NewNPCAnimations;

    NPCTagPoints m_EditNPCTagPoints;
    NPCAnimations m_EditNPCAnimations;

    /* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsNPC(void);

    /* clears NPCs in catalogue and re-lists every present NPC */
    void refreshNPCList(void);

    /* creates the cataloge tab for NPCs

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createNPCCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);

    //callback for clicking on NPC tab
    bool NPCTabClicked(const CEGUI::EventArgs &e);

    //windows for creating/ editing NPCs
    void showNPCNewWindow(void);
	void showNPCConfirmDeleteWindow(void);
	void showNPCNewAnimsWindow(void);
	void showNPCNewTagpointsWindow(void);

	void showNPCEditWindow(void);
	void showNPCEditAnimsWindow(void);
	void showNPCEditTagpointsWindow(void);

	//method to visually add a NPC record to the catalogue
	void addNPCRecordToCatalogue(const std::string& ID, const NPCRecord& Record);

    //create the popup menu for the NPC tab
    void createPopupMenuNPCTab(void);
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
	bool NPCNewFrameAnimsClicked(const CEGUI::EventArgs &e);
	bool NPCNewFrameTagpointsClicked(const CEGUI::EventArgs &e);
	//callbacks for animation window (new NPC)
	bool NPCNewAnimsFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCNewAnimsFrameOKClicked(const CEGUI::EventArgs &e);
	//callbacks for tagpoint window (new NPC)
	bool NPCNewTagsFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCNewTagsFrameOKClicked(const CEGUI::EventArgs &e);

	//create popup menu for inventory list in NPCNewFrame
	void createPopupMenuNPCNewFrameList(void);
	//callbacks for popup menu
    bool InventoryListAddClicked(const CEGUI::EventArgs &e);
    bool InventoryListEditClicked(const CEGUI::EventArgs &e);
    bool InventoryListDeleteClicked(const CEGUI::EventArgs &e);

    //window for adding new items to inventory list
    void showInventoryListAddWindow(void);
    //callbacks for that window
    bool AddInventoryFrameAddClicked(const CEGUI::EventArgs &e);
    bool AddInventoryFrameCancelClicked(const CEGUI::EventArgs &e);

    //callbacks of window for editing NPCs
	bool NPCEditFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCEditFrameOKClicked(const CEGUI::EventArgs &e);
	bool NPCEditFrameAnimsClicked(const CEGUI::EventArgs &e);
	bool NPCEditFrameTagpointsClicked(const CEGUI::EventArgs &e);
	//callbacks for animation window (editing NPC)
	bool NPCEditAnimsFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCEditAnimsFrameOKClicked(const CEGUI::EventArgs &e);
	//callbacks for tagpoint window (new NPC)
	bool NPCEditTagsFrameCancelClicked(const CEGUI::EventArgs &e);
	bool NPCEditTagsFrameOKClicked(const CEGUI::EventArgs &e);

	//window for adding new items to inventory list
    void showEditInventoryListAddWindow(void);
    //callbacks for that window
    bool EditAddInventoryFrameAddClicked(const CEGUI::EventArgs &e);
    bool EditAddInventoryFrameCancelClicked(const CEGUI::EventArgs &e);

	//create popup menu for inventory list in NPCEditFrame
	void createPopupMenuNPCEditFrameList(void);
	//callbacks for popup menu
    bool EditInventoryListAddClicked(const CEGUI::EventArgs &e);
    bool EditInventoryListEditClicked(const CEGUI::EventArgs &e);
    bool EditInventoryListDeleteClicked(const CEGUI::EventArgs &e);

    //helper function to list all available item IDs in a Combobox
    void updateItemList(CEGUI::Combobox* combo);

    //gets data from MCL and transforms it into inventory
    Inventory MCLToInventory(const CEGUI::MultiColumnList* mcl);
    //gets data from Inventory and puts it into MultiColumnist
    void InventoryToMCL(const Inventory& invent, CEGUI::MultiColumnList* target);

	//callback for clicking inventory list in NPCNewFrame
	bool NPCNewFrameInventoryListClicked(const CEGUI::EventArgs &e);

	//callback for clicking inventory list in NPCEditFrame
	bool NPCEditFrameInventoryListClicked(const CEGUI::EventArgs &e);
}; //class

} //namespace

#endif // EDITORAPPLICATIONNPC_H
