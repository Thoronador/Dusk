/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-30
 Purpose: EditorApplicationObject class
          EditorApplicationObject is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing DuskObjects.

 History:
     - 2010-04-30 (rev 194)  - initial version (by thoronador)

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

    void CreatePopupMenuObjectTab(void);
    //callbacks for popup menues
    bool ObjectNewClicked(const CEGUI::EventArgs &e);
	bool ObjectEditClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteClicked(const CEGUI::EventArgs &e);

    /* clears object list in catalogue and redisplays every present object */
    void RefreshObjectList(void);

    //windows for creating/ editing objects
	void showObjectNewWindow(void);
	void showObjectEditWindow(void);
	void showObjectConfirmDeleteWindow(void);
	void showObjectEditConfirmIDChangeWindow(void);

	//methods to visually add Items or Objects to catalogue
    //  (real data is not effected, methods only show new row in catalogue)
	void addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh);

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
