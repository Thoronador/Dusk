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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-30
 Purpose: EditorApplicationObject class
          EditorApplicationObject is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing DuskObjects.

 History:
     - 2010-04-30 (rev 194)  - initial version (by thoronador)
     - 2010-08-18 (rev 230)  - minor adjustment due to renaming of ObjectData
                               to ObjectManager
     - 2010-08-31 (rev 239)  - naming conventions (engine) enforced
     - 2010-11-26 (rev 260)  - "compatibility update" for collision flag
                               (setting that flag still needs to be implemented)
     - 2010-11-27 (rev 261)  - closeEditWindowsObject() added
                             - collision flag of objects can now be set and is
                               displayed in catalogue, too
     - 2011-05-10 (rev 285)  - createObjectCatalogueTab() and ObjectTabClicked()
                               added (moved code from EditorApplication)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONOBJECT_H
#define EDITORAPPLICATIONOBJECT_H

#include <CEGUI/CEGUIEventArgs.h>
#include <string>

namespace Dusk
{

class EditorApplicationObject
{
  public:
    //constructor
    EditorApplicationObject();
    //destructor
    virtual ~EditorApplicationObject();
  protected:
    //id of item for deletion
    std::string ID_of_object_to_delete;
    //id for editing
    std::string ID_of_object_to_edit;

    /* creates the cataloge tab for objects

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createObjectCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);
    //callback for clicking on object tab
    bool ObjectTabClicked(const CEGUI::EventArgs &e);
    //creates the popup menu on the object tab
    void createPopupMenuObjectTab(void);
    //callbacks for popup menues
    bool ObjectNewClicked(const CEGUI::EventArgs &e);
	bool ObjectEditClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteClicked(const CEGUI::EventArgs &e);


    /* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsObject(void);

    /* clears object list in catalogue and redisplays every present object */
    void refreshObjectList(void);

    //windows for creating/ editing objects
	void showObjectNewWindow(void);
	void showObjectEditWindow(void);
	void showObjectConfirmDeleteWindow(void);
	void showObjectEditConfirmIDChangeWindow(void);

	//methods to visually add Items or Objects to catalogue
    //  (real data is not effected, methods only show new row in catalogue)
	void addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh, const bool collision);

	//callbacks of window for creating new objects
	bool ObjectNewFrameCancelClicked(const CEGUI::EventArgs &e);
	bool ObjectNewFrameOKClicked(const CEGUI::EventArgs &e);
	//callbacks of window for editing objects
	bool ObjectEditFrameCancelClicked(const CEGUI::EventArgs &e);
	bool ObjectEditFrameSaveClicked(const CEGUI::EventArgs &e);
	//callbacks of window to delete objects
	bool ObjectDeleteFrameNoClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteFrameYesClicked(const CEGUI::EventArgs &e);
	//callbacks for window to confirm ID change (objects)
	bool ObjectConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e);
	bool ObjectConfirmIDChangeNewClicked(const CEGUI::EventArgs &e);
	bool ObjectConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e);
}; //class

} //namespace

#endif // EDITORAPPLICATIONOBJECT_H
