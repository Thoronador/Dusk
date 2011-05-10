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
 Purpose: EditorApplicationLight class
          EditorApplicationLight is an ancestor of EditorApplication and holds
          the methods which are needed to show and manage the (CE)GUI elements
          associated with creating/ editing Lights.

 History:
     - 2010-04-28 (rev 192)  - initial version (by thoronador)
     - 2010-08-31 (rev 239)  - naming convention enforced
     - 2011-05-10 (rev 285)  - closeEditWindowsLight(), createLightCatalogueTab()
                               and LightTabClicked() added (moved code from
                               EditorApplication)

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

    /* creates the cataloge tab for lights

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createLightCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);
    //callback for clicking light catalogue tab
    bool LightTabClicked(const CEGUI::EventArgs &e);
    //create the popup menu for the Light tab
    void createPopupMenuLightTab(void);
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

	/* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsLight(void);

    /* clears light list in catalogue and redisplays every present light */
    void refreshLightList(void);
}; //class

} //namespace

#endif // EDITORAPPLICATIONLIGHT_H
