#ifndef EditorApplication_h
#define EditorApplication_h

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "EditorApplicationNPC.h"
#include "EditorApplicationJournal.h"
#include "EditorApplicationLight.h"
#include "EditorApplicationItem.h"
#include "EditorApplicationObject.h"
#include "EditorFrameListener.h"
#include "../Engine/DuskObject.h"
#include "../Engine/DuskTypes.h"
#include "../Engine/DuskFunctions.h"
#include <OgreCEGUIRenderer.h>
#include <CEGUI/CEGUI.h>
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
                         public EditorApplicationJournal
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
	void CreateCEGUIRootWindow(void);
	void CreateCEGUIMenuBar(void);
	void CreateCEGUICatalogue(void);
	void CreatePopupMenus(void);
	void showCEGUILoadWindow(void);
	void UpdateLoadWindowFiles(const std::string Directory);

	//for editing object references
	void showObjectReferenceEditWindow(const CEGUI::Point& pt);

	//window for landscape editing
	void showLandscapeEditWindow(void);

	//closing all windows for editing and creation of objects
	void closeAllEditWindows(void);

	//shows or hides catalogue window
	void SetCatalogueVisibility(const bool visible);

	//clear items/ lights/ objects in catalogue
	//  (real data is not affected, methods only delete all shown new rows catalogue)
	void ClearCatalogue(void);

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

	//callbacks for buttons
	bool LoadFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LoadFrameOKClicked(const CEGUI::EventArgs &e);
	bool ObjectTabClicked(const CEGUI::EventArgs &e);
	bool ItemTabClicked(const CEGUI::EventArgs &e);
	bool LightTabClicked(const CEGUI::EventArgs &e);
	bool NPCTabClicked(const CEGUI::EventArgs &e);

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

	//scene query wrapper
	DuskObject* GetObjectAtMouse(const CEGUI::Point& pt);
};//class

}//namespace

#endif
