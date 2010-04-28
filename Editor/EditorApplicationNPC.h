/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-28
 Purpose: EditorApplicationNPC class
          EditorApplicationNPC is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing NPCs.

 History:
     - 2010-04-28 (rev 191)  - initial version (by thoronador)

 ToDo list:
     - implement possibility to create/ edit NPCs
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONNPC_H
#define EDITORAPPLICATIONNPC_H

#include <string>
#include <CEGUI/CEGUIEventArgs.h>
#include "EditorApplicationBase.h"
#include "../Engine/NPCBase.h"

namespace Dusk
{

class EditorApplicationNPC: public EditorApplicationBase
{
  public:
    // constructor
    EditorApplicationNPC();

    //destructor
    virtual ~EditorApplicationNPC();

  protected:
    //ID for NPC deletion
    std::string ID_of_NPC_to_delete;

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
}; //class

} //namespace

#endif // EDITORAPPLICATIONNPC_H
