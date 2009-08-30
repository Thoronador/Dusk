#ifndef EditorApplication_h
#define EditorApplication_h

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "EditorFrameListener.h"
#include "../Engine/ItemBase.h"
#include "../Engine/DuskObject.h"
#include <OgreCEGUIRenderer.h>
#include <CEGUI/CEGUI.h>
#include <string>
#include <vector>

namespace Dusk
{

struct FileEntry {
           std::string FileName;
           bool IsDirectory;
};//struct

struct mouse_button_data
{
  CEGUI::Point down, up;
};//struct

struct mouse_record
{
  mouse_button_data LeftButton;
  mouse_button_data RightButton;
  static const int drag_threshold = 4;
};//struct

//helps us to locate a item within column lists
CEGUI::ListboxItem * getLbItemAtPoint(const CEGUI::Point& pt, CEGUI::MultiColumnList* mcl);

class EditorApplication
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
    //id of item for deletion
    std::string ID_of_object_to_delete;
    std::string ID_of_item_to_delete;
    //id for editing
    std::string ID_of_object_to_edit;
    std::string ID_of_item_to_edit;

    //mouse handling data
    mouse_record mouse;

    DuskObject * mouse_object;

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

    void RefreshObjectList(void);
    void RefreshItemList(void);
	//general message windows
	void showWarning(const std::string Text_of_warning);
	void showHint(const std::string hint_text);

    //windows for creating/ editing objects
	void showObjectNewWindow(void);
	void showObjectEditWindow(void);
	void showObjectConfirmDeleteWindow(void);
	void showObjectEditConfirmIDChangeWindow(void);
    //windows for creating/ editing items
    void showItemNewWindow(void);
    void showItemEditWindow(void);
	void showItemConfirmDeleteWindow(void);
	void showItemEditConfirmIDChangeWindow(void);

	//closing all windows for editing and creation of objects
	void closeAllEditWindows(void);

    //methods to visually add Items or Objects to catalogue
    //  (real data is not effected, methods only show new row in catalogue)
	void addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData);
	void addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh);

	//callbacks for menu items
	bool LoadButtonClicked(const CEGUI::EventArgs &e);
	bool SaveButtonClicked(const CEGUI::EventArgs &e);
	bool StatsButtonClicked(const CEGUI::EventArgs &e);

	bool ModeMoveClicked(const CEGUI::EventArgs &e);
	bool ModeLandClicked(const CEGUI::EventArgs &e);
	bool ModeListClicked(const CEGUI::EventArgs &e);

	//callbacks for buttons
	bool LoadFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LoadFrameOKClicked(const CEGUI::EventArgs &e);
	bool WarningFrameOKClicked(const CEGUI::EventArgs &e);
	bool HintFrameOKClicked(const CEGUI::EventArgs &e);
	bool ObjectTabClicked(const CEGUI::EventArgs &e);
	bool ItemTabClicked(const CEGUI::EventArgs &e);

	//callbacks for popup menus
	bool ObjectNewClicked(const CEGUI::EventArgs &e);
	bool ObjectEditClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteClicked(const CEGUI::EventArgs &e);

	bool ItemNewClicked(const CEGUI::EventArgs &e);
	bool ItemEditClicked(const CEGUI::EventArgs &e);
	bool ItemDeleteClicked(const CEGUI::EventArgs &e);

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

	//callbacks for window clicks / to implement object dragging
	bool RootMouseDown(const CEGUI::EventArgs &e);
	bool RootMouseUp(const CEGUI::EventArgs &e);
	bool RootMouseMove(const CEGUI::EventArgs &e);
	bool ObjectListMouseDown(const CEGUI::EventArgs &e);

};//class

}//namespace

#endif
