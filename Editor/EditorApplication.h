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

#ifndef EditorApplication_h
#define EditorApplication_h

#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include "EditorApplicationNPC.h"
#include "EditorApplicationJournal.h"
#include "EditorApplicationLight.h"
#include "EditorApplicationItem.h"
#include "EditorApplicationObject.h"
#include "EditorApplicationProjectile.h"
#include "EditorApplicationWeapon.h"
#include "EditorFrameListener.h"
#include "../Engine/DuskObject.h"
#include "../Engine/DuskTypes.h"
#include "../Engine/DuskFunctions.h"
#include <OgreCEGUIRenderer.h>
#include <CEGUI/CEGUIEventArgs.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUIVector.h>
#include <string>
#include <vector>

namespace Dusk
{

struct mouse_button_data
{
  CEGUI::Point down, up;
};//struct

struct mouse_record
{
  mouse_button_data LeftButton;
  mouse_button_data RightButton;
};//struct

//helps us to locate a item within column lists
CEGUI::ListboxItem * getLbItemAtPoint(const CEGUI::Point& pt, CEGUI::MultiColumnList* mcl);

class EditorApplication: public EditorApplicationNPC, public EditorApplicationLight,
                         public EditorApplicationItem, public EditorApplicationObject,
                         public EditorApplicationJournal, public EditorApplicationProjectile,
                         public EditorApplicationWeapon
{
public:
    /// Standard constructor
    EditorApplication();
    /// Standard destructor
    virtual ~EditorApplication();

    /// Start the editor
    virtual void go(void);

protected:
    Ogre::Root *mRoot;
    Ogre::SceneManager* mSceneMgr;
    EditorFrameListener* mFrameListener;
    Ogre::RenderWindow* mWindow;

	CEGUI::System *mSystem;
    CEGUI::OgreCEGUIRenderer *mRenderer;

    //application specific
    std::string LoadedDataFile;
    std::string LoadFrameDirectory;
    std::vector<FileEntry> LoadFrameFiles;

    //mouse handling data
    mouse_record mouse;

    DuskObject * mouse_object;
    DuskObject * edit_object;

    //landscape
    ColourData LandscapeColour;
    float m_LandscapeEditRadius;

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void);
    /** Configures the application - returns false if the user chooses to abandon configuration. */
    virtual bool configure(void);

    virtual void createSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);

    virtual void createScene(void);

    virtual void destroyScene(void);    // Optional to override this

    virtual void createViewports(void);

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources(void);

	/// Optional override method where you can create resource listeners (e.g. for loading screens)
	virtual void createResourceListener(void);

	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources(void);

	//CEGUI related methods to create interface
	void createCEGUIRootWindow(void);
	void createCEGUIMenuBar(void);
	void createCEGUICatalogue(void);
	void createPopupMenus(void);
	void showCEGUILoadWindow(void);
	void UpdateLoadWindowFiles(const std::string& Directory);

	//for editing object references
	void showObjectReferenceEditWindow(const CEGUI::Point& pt);

	//window for landscape editing
	void showLandscapeEditWindow(void);

	//closing all windows for editing and creation of objects
	void closeAllEditWindows(void);

	//shows or hides catalogue window
	void setCatalogueVisibility(const bool visible);

	//clear items/ lights/ objects in catalogue
	//  (real data is not affected, methods only delete all shown new rows catalogue)
	void clearCatalogue(void);

	//callbacks for menu items
	bool LoadButtonClicked(const CEGUI::EventArgs &e);
	bool SaveButtonClicked(const CEGUI::EventArgs &e);
	bool StatsButtonClicked(const CEGUI::EventArgs &e);

	bool ModeMoveClicked(const CEGUI::EventArgs &e);
	bool ModeLandUpClicked(const CEGUI::EventArgs &e);
	bool ModeLandDownClicked(const CEGUI::EventArgs &e);
	bool ModeLandColourClicked(const CEGUI::EventArgs &e);
	bool ModeListClicked(const CEGUI::EventArgs &e);

	bool JournalClicked(const CEGUI::EventArgs &e);

	bool WeatherToggleClicked(const CEGUI::EventArgs &e);

	bool WireframeToggleClicked(const CEGUI::EventArgs &e);

	//callbacks for buttons
	bool LoadFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LoadFrameOKClicked(const CEGUI::EventArgs &e);

	//callbacks for window clicks / to implement object dragging
	bool RootMouseDown(const CEGUI::EventArgs &e);
	bool RootMouseUp(const CEGUI::EventArgs &e);
	bool RootMouseMove(const CEGUI::EventArgs &e);
	bool RootDoubleClicked(const CEGUI::EventArgs &e);
	bool ObjectListMouseDown(const CEGUI::EventArgs &e);

	//callbacks for ref edit window
	bool ObjectReferenceEditCancelClicked(const CEGUI::EventArgs &e);
	bool ObjectReferenceEditSaveClicked(const CEGUI::EventArgs &e);

	//callbacks for landscape window
	bool LandscapeFrameFinishClicked(const CEGUI::EventArgs &e);
	bool LandscapeFrameRadioButtonClicked(const CEGUI::EventArgs &e);
	bool LandscapeFrameColourChanged(const CEGUI::EventArgs &e);
	bool LandscapeFrameRadiusChanged(const CEGUI::EventArgs &e);

	//scene query wrapper
	DuskObject* getObjectAtMouse(const CEGUI::Point& pt);
};//class

}//namespace

#endif
