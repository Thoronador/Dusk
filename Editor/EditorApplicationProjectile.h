/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2011, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2011-05-10
 Purpose: EditorApplicationProjectile class
          EditorApplicationProjectile is an ancestor of EditorApplication and
          holds the methods which are needed to show and manage the (CE)GUI
          elements associated with creating/ editing projectiles.

 History:
     - 2011-05-10 (rev 284) - initial version (by thoronador)
     - 2011-05-11 (rev 286) - windows for creating and editing projectiles added
     - 2011-05-13 (rev 288) - added missing frame windows to function
                              closeEditWindowsProjectile()
     - 2011-05-17 (rev 290) - minor fixes
     - 2013-06-02           - minor adjustment for CEGUI 0.7

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONPROJECTILE_H
#define EDITORAPPLICATIONPROJECTILE_H

#include <CEGUI/CEGUIWindowManager.h>

namespace Dusk
{

//forward declaration
struct ProjectileRecord;

class EditorApplicationProjectile
{
  public:
    // constructor
    EditorApplicationProjectile();

    //destructor
    virtual ~EditorApplicationProjectile();
  protected:
    /* creates the cataloge tab for projectiles

       parameters:
           winmgr - the window manager (for convenience)
           tab    - the basic catalogue tab control
    */
    void createProjectileCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab);

    /* creates the popup menu for the projectile tab */
    void createPopupMenuProjectileTab(void);
    //callbacks for popup menu
    bool ProjectileNewClicked(const CEGUI::EventArgs &e);
    bool ProjectileEditClicked(const CEGUI::EventArgs &e);
    bool ProjectileDeleteClicked(const CEGUI::EventArgs &e);

    /* event callback for clicking on catalogue tab */
    bool ProjectileTabClicked(const CEGUI::EventArgs &e);

    /* destroys all windows that might currently be open, because they were
       created by this class
    */
    void closeEditWindowsProjectile(void);

    //window for deletion
    void showProjectileConfirmDeleteWindow(void);
    //callbacks for deletion window
    bool ProjectileDeleteFrameNoClicked(const CEGUI::EventArgs &e);
    bool ProjectileDeleteFrameYesClicked(const CEGUI::EventArgs &e);

    //window for creating new projectiles
    void showProjectileNewWindow(void);
    //callbacks of window for creating new projectiles
    bool ProjectileNewFrameCancelClicked(const CEGUI::EventArgs &e);
    bool ProjectileNewFrameOKClicked(const CEGUI::EventArgs &e);

    //window for editing projectiles
    void showProjectileEditWindow(void);
    //callbacks of window for editing new projectiles
    bool ProjectileEditFrameCancelClicked(const CEGUI::EventArgs &e);
    bool ProjectileEditFrameOKClicked(const CEGUI::EventArgs &e);

    /* clears projectiles in catalogue and re-lists every present projectile */
    void refreshProjectileList(void);

    //method to visually add a projectile record to the catalogue
    void addProjectileRecordToCatalogue(const std::string& ID, const ProjectileRecord& record);

    //ID of the projectile that shall be deleted
    std::string ID_of_Projectile_to_delete;
    //ID of the projectile that shall be edited
    std::string ID_of_Projectile_to_edit;
};//class

}//namespace

#endif // EDITORAPPLICATIONPROJECTILE_H
