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
 Date:    2010-04-30
 Purpose: EditorApplicationItem class
          EditorApplicationItem is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Items.

 History:
     - 2010-04-30 (rev 193)  - initial version (by thoronador)
     - 2010-08-31 (rev 239)  - naming convention enforced
     - 2010-11-22 (rev 259)  - item references are now handled, too
     - 2011-05-10 (rev 285)  - closeEditWindowsItem(), createItemCatalogueTab()
                               and ItemTabClicked() added (moved code from
                               EditorApplication)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONITEM_H
#define EDITORAPPLICATIONITEM_H

#include <CEGUI/CEGUIEventArgs.h>
#include <string>
#include "../Engine/database/ItemRecord.h"

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

    /* creates the cataloge tab for items

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createItemCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);
    //callback for clicking on tab
    bool ItemTabClicked(const CEGUI::EventArgs &e);
    //create the popup menu for the Item tab
    void createPopupMenuItemTab(void);
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

	/* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsItem(void);

    /* clears item list in catalogue and redisplays every present item */
	void refreshItemList(void);
}; //class

} //namespace

#endif // EDITORAPPLICATIONITEM_H
