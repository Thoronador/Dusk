/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2011 thoronador

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
 Date:    2011-05-18
 Purpose: EditorApplicationVehicle class
          EditorApplicationVehicle is an ancestor of EditorApplication and
          holds the methods which are needed to show and manage the (CE)GUI
          elements associated with creating/ editing vehicles.

 History:
     - 2011-05-18 (rev 291) - initial version (by thoronador)
     - 2011-09-29 (rev 301) - window for creating new vehicles added
     - 2012-07-11 (rev 320) - replaced VehicleBase by Database

 ToDo list:
     - add possibility to create and edit vehicles
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/



#ifndef EDITORAPPLICATIONVEHICLE_H
#define EDITORAPPLICATIONVEHICLE_H

#include <CEGUI/CEGUIWindowManager.h>

namespace Dusk
{

//forward declaration
struct VehicleRecord;


class EditorApplicationVehicle
{
  public:
    // constructor
    EditorApplicationVehicle();

    //destructor
    virtual ~EditorApplicationVehicle();
  protected:
    /* creates the cataloge tab for vehicles

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createVehicleCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);

    /* creates the popup menu for the vehicle tab */
    void createPopupMenuVehicleTab(void);

    /* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsVehicle(void);

    /* clears vehicles in catalogue and re-lists every present vehicle */
    void refreshVehicleList(void);

    /* method to visually add a vehicle record to the catalogue

       parameters:
           ID     - ID of the vehicle
           record - vehicle data
    */
    void addVehicleRecordToCatalogue(const std::string& ID, const VehicleRecord& record);
  private:
    //callbacks for popup menu
    bool VehicleNewClicked(const CEGUI::EventArgs &e);
    bool VehicleEditClicked(const CEGUI::EventArgs &e);
    bool VehicleDeleteClicked(const CEGUI::EventArgs &e);

    /* event callback for clicking on catalogue tab */
    bool VehicleTabClicked(const CEGUI::EventArgs &e);

    //window for creating new vehicles
    void showVehicleNewWindow(void);
    //callbacks for deletion window
    bool VehicleNewFrameOKClicked(const CEGUI::EventArgs &e);
    bool VehicleNewFrameCancelClicked(const CEGUI::EventArgs &e);
    bool VehicleNewFrameMountpointsClicked(const CEGUI::EventArgs &e);

    //window for editing vehicles
    void showVehicleEditWindow(void);

    //window for deleting vehicles
    void showVehicleConfirmDeleteWindow(void);
    //callbacks for deletion window
    bool VehicleDeleteFrameNoClicked(const CEGUI::EventArgs &e);
    bool VehicleDeleteFrameYesClicked(const CEGUI::EventArgs &e);

    //string for keeping ID of the vehicle that was edited
    std::string ID_of_vehicle_to_edit;
    //string for keeping ID of the vehicle that was deleted
    std::string ID_of_vehicle_to_delete;
}; //class

} //namespace

#endif // EDITORAPPLICATIONVEHICLE_H
