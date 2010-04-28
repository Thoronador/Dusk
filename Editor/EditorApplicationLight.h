/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-28
 Purpose: EditorApplicationLight class
          EditorApplicationLight is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Lights.

 History:
     - 2010-04-28 (rev 192)  - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONLIGHT_H
#define EDITORAPPLICATIONLIGHT_H

#include <string>
#include <CEGUI/CEGUIEventArgs.h>
#include "../Engine/LightBase.h"

namespace Dusk
{

class EditorApplicationLight
{
  public:
    // constructor
    EditorApplicationLight();

    //destructor
    virtual ~EditorApplicationLight();
  protected:
    //ID for light deletion
    std::string ID_of_light_to_delete;
    //ID for editing
    std::string ID_of_light_to_edit;
    /*methods to visually add Lights to catalogue
      (real data is not effected, methods only show new row in catalogue) */
    void addLightRecordToCatalogue(const std::string& ID, const LightRecord& Record);

    //create the popup menu for the Light tab
    void CreatePopupMenuLightTab(void);
    //callbacks for popup menu
    bool LightNewClicked(const CEGUI::EventArgs &e);
	bool LightEditClicked(const CEGUI::EventArgs &e);
	bool LightDeleteClicked(const CEGUI::EventArgs &e);

	//windows for creating/ editing lights
	void showLightNewWindow(void);
    void showLightEditWindow(void);
	void showLightConfirmDeleteWindow(void);
	void showLightEditConfirmIDChangeWindow(void);

	//callbacks of window for creating new lights
	bool LightNewFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LightNewFrameOKClicked(const CEGUI::EventArgs &e);
	//callbacks of windwow for editing light
	bool LightEditFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LightEditFrameSaveClicked(const CEGUI::EventArgs &e);
	//callbacks of window to delete lights
	bool LightDeleteFrameNoClicked(const CEGUI::EventArgs &e);
	bool LightDeleteFrameYesClicked(const CEGUI::EventArgs &e);
	//callbacks for window to confirm ID change (light)
	bool LightConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e);
	bool LightConfirmIDChangeNewClicked(const CEGUI::EventArgs &e);
	bool LightConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e);

    /* clears light list in catalogue and redisplays every present light */
    void RefreshLightList(void);
}; //class

} //namespace

#endif // EDITORAPPLICATIONLIGHT_H
