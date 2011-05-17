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
 Date:    2011-05-13
 Purpose: EditorApplicationWeapon class
          EditorApplicationWeapon is an ancestor of EditorApplication and
          holds the methods which are needed to show and manage the (CE)GUI
          elements associated with creating/ editing weapons.

 History:
     - 2010-05-13 (rev 288) - initial version (by thoronador)
     - 2010-05-14 (rev 289) - window for creating new weapons added
     - 2010-05-17 (rev 290) - window for editing weapons added

 ToDo list:
     - add possibility to create and edit weapons
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONWEAPON_H
#define EDITORAPPLICATIONWEAPON_H

#include <CEGUI/CEGUIWindowManager.h>

namespace Dusk
{

//forward declaration
struct WeaponRecord;


class EditorApplicationWeapon
{
  public:
    // constructor
    EditorApplicationWeapon();

    //destructor
    virtual ~EditorApplicationWeapon();
  protected:
    /* creates the cataloge tab for weapons

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createWeaponCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);

    /* creates the popup menu for the weapon tab */
    void createPopupMenuWeaponTab(void);
    //callbacks for popup menu
    bool WeaponNewClicked(const CEGUI::EventArgs &e);
    bool WeaponEditClicked(const CEGUI::EventArgs &e);
    bool WeaponDeleteClicked(const CEGUI::EventArgs &e);

    /* event callback for clicking on catalogue tab */
    bool WeaponTabClicked(const CEGUI::EventArgs &e);

    //window for deletion
    void showWeaponConfirmDeleteWindow(void);
    //callbacks for deletion window
    bool WeaponDeleteFrameNoClicked(const CEGUI::EventArgs &e);
    bool WeaponDeleteFrameYesClicked(const CEGUI::EventArgs &e);

    //window for creating new weapons
    void showWeaponNewWindow(void);
    //callbacks of window for creating new weapons
    bool WeaponNewFrameCancelClicked(const CEGUI::EventArgs &e);
    bool WeaponNewFrameOKClicked(const CEGUI::EventArgs &e);

    //window for editing projectiles
    void showWeaponEditWindow(void);
    //callbacks of window for editing weapons
    bool WeaponEditFrameCancelClicked(const CEGUI::EventArgs &e);
    bool WeaponEditFrameOKClicked(const CEGUI::EventArgs &e);

    /* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsWeapon(void);

    /* clears weapons in catalogue and re-lists every present weapon */
    void refreshWeaponList(void);

    //method to visually add a weapon record to the catalogue
    void addWeaponRecordToCatalogue(const std::string& ID, const WeaponRecord& record);

    //string for keeping ID of the weapon that has to be deleted
    std::string ID_of_Weapon_to_delete;
    //string for keeping ID of the weapon that was edited
    std::string ID_of_Weapon_to_edit;
};//class

} //namespace

#endif // EDITORAPPLICATIONWEAPON_H
