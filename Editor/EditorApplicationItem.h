/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-30
 Purpose: EditorApplicationItem class
          EditorApplicationItem is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Items.

 History:
     - 2010-04-30 (rev 193)  - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONITEM_H
#define EDITORAPPLICATIONITEM_H

#include <CEGUI/CEGUIEventArgs.h>
#include <string>
#include "../Engine/ItemBase.h"

namespace Dusk
{

class EditorApplicationItem
{
  public:
    //constructor
    EditorApplicationItem();
    //destructor
    virtual ~EditorApplicationItem();
  protected:
    //ID for item deletion
    std::string ID_of_item_to_delete;
    //ID for editing
    std::string ID_of_item_to_edit;
    //create the popup menu for the Item tab
    void CreatePopupMenuItemTab(void);
    //callbacks for popup menue
    bool ItemNewClicked(const CEGUI::EventArgs &e);
    bool ItemEditClicked(const CEGUI::EventArgs &e);
    bool ItemDeleteClicked(const CEGUI::EventArgs &e);

    //methods to visually add Items to catalogue
    //  (real data is not effected, methods only show new row in catalogue)
    void addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData);

    //windows for creating/ editing items
    void showItemNewWindow(void);
    void showItemEditWindow(void);
	void showItemConfirmDeleteWindow(void);
	void showItemEditConfirmIDChangeWindow(void);

	//callbacks of window for creating new items
	bool ItemNewFrameCancelClicked(const CEGUI::EventArgs &e);
	bool ItemNewFrameOKClicked(const CEGUI::EventArgs &e);
	//callbacks of windwow for editing items
	bool ItemEditFrameCancelClicked(const CEGUI::EventArgs &e);
	bool ItemEditFrameSaveClicked(const CEGUI::EventArgs &e);
    //callbacks of window to delete items
	bool ItemDeleteFrameNoClicked(const CEGUI::EventArgs &e);
	bool ItemDeleteFrameYesClicked(const CEGUI::EventArgs &e);
	//callbacks for window to confirm ID change (items)
	bool ItemConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e);
	bool ItemConfirmIDChangeNewClicked(const CEGUI::EventArgs &e);
	bool ItemConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e);

    /* clears item list in catalogue and redisplays every present item */
	void RefreshItemList(void);
}; //class

} //namespace

#endif // EDITORAPPLICATIONITEM_H
