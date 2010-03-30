#ifndef EditorApplication_h
#define EditorApplication_h

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "EditorFrameListener.h"
#include "../Engine/DuskObject.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/DuskTypes.h"
#include "../Engine/ItemBase.h"
#include "../Engine/LightBase.h"
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
    std::string ID_of_light_to_delete;
    //id for editing
    std::string ID_of_object_to_edit;
    std::string ID_of_item_to_edit;
    std::string ID_of_light_to_edit;

    //journal
    std::string ID_of_quest_to_delete;
    std::string ID_of_quest_to_rename;

    std::string ID_of_quest_to_add_entry;
    //std::string ID_of_quest_to_edit_entry;
    unsigned int Index_of_entry_to_edit;
    std::string QuestID_of_entry_to_edit;

    unsigned int Index_of_entry_to_delete;
    std::string QuestID_of_entry_to_delete;

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

    void RefreshObjectList(void);
    void RefreshItemList(void);
    void RefreshLightList(void);
	//general message windows
	void showWarning(const std::string& Text_of_warning);
	void showHint(const std::string& hint_text, const bool big=false);

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
	//windows for creating/ editing lights
	void showLightNewWindow(void);
    void showLightEditWindow(void);
	void showLightConfirmDeleteWindow(void);
	void showLightEditConfirmIDChangeWindow(void);

	//for editing object references
	void showObjectReferenceEditWindow(const CEGUI::Point& pt);

	//window for landscape editing
	void showLandscapeEditWindow(void);

	//window for journal entries
	void showJournalWindow(void);
	void showJournalConfirmDeleteQuestWindow(void);
	void showJournalRenameQuestWindow(void);

	void showQuestEntryNewWindow(void);
	void showQuestEntryConfirmDeleteWindow(void);

	void showQuestEntryEditWindow(void);

	//closing all windows for editing and creation of objects
	void closeAllEditWindows(void);

	//shows or hides catalogue window
	void SetCatalogueVisibility(const bool visible);

    //methods to visually add Items or Objects to catalogue
    //  (real data is not effected, methods only show new row in catalogue)
	void addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData);
	void addLightRecordToCatalogue(const std::string& ID, const LightRecord& Record);
	void addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh);
	//clear items/ lights/ objects in catalogue
	//  (real data is not affected, methods only delete all shown new rows catalogue)
	void ClearCatalogue(void);

	//loads all quest IDs into list/ combobox in JournalFrame
	void UpdateQuestList(void);
	void UpdateQuestEntryList(const std::string& questID);

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
	bool WarningFrameOKClicked(const CEGUI::EventArgs &e);
	bool HintFrameOKClicked(const CEGUI::EventArgs &e);
	bool ObjectTabClicked(const CEGUI::EventArgs &e);
	bool ItemTabClicked(const CEGUI::EventArgs &e);
	bool LightTabClicked(const CEGUI::EventArgs &e);
	bool JournalEntryListClicked(const CEGUI::EventArgs &e);

	//callbacks for popup menus
	bool ObjectNewClicked(const CEGUI::EventArgs &e);
	bool ObjectEditClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteClicked(const CEGUI::EventArgs &e);

	bool ItemNewClicked(const CEGUI::EventArgs &e);
	bool ItemEditClicked(const CEGUI::EventArgs &e);
	bool ItemDeleteClicked(const CEGUI::EventArgs &e);

	bool LightNewClicked(const CEGUI::EventArgs &e);
	bool LightEditClicked(const CEGUI::EventArgs &e);
	bool LightDeleteClicked(const CEGUI::EventArgs &e);

	bool JournalEntryNewClicked(const CEGUI::EventArgs &e);
	bool JournalEntryEditClicked(const CEGUI::EventArgs &e);
	bool JournalEntryDeleteClicked(const CEGUI::EventArgs &e);

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

	//callbacks for Journal window
	bool JournalFrameNewQuestClicked(const CEGUI::EventArgs &e);
	bool JournalFrameDeleteQuestClicked(const CEGUI::EventArgs &e);
	bool JournalFrameRenameQuestClicked(const CEGUI::EventArgs &e);
	bool JournalFrameCloseClicked(const CEGUI::EventArgs &e);
	bool JournalFrameComboboxChanged(const CEGUI::EventArgs &e);

	//callbacks for quest deletion window
	bool JournalDeleteQuestFrameYesClicked(const CEGUI::EventArgs &e);
	bool JournalDeleteQuestFrameNoClicked(const CEGUI::EventArgs &e);

	//callbacks for new quest entry window
	bool NewQuestEntryFrameOKClicked(const CEGUI::EventArgs &e);
	bool NewQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e);

	//callbacks of window to delete quest entries
	bool QuestEntryConfirmDeleteNoClicked(const CEGUI::EventArgs &e);
	bool QuestEntryConfirmDeleteYesClicked(const CEGUI::EventArgs &e);

    //callbacks for edit quest entry window
    bool EditQuestEntryFrameOKClicked(const CEGUI::EventArgs &e);
    bool EditQuestEntryFrameCancelClicked(const CEGUI::EventArgs &e);

    //callbacks for rename quest window
    bool QuestRenameFrameCancelClicked(const CEGUI::EventArgs &e);
    bool QuestRenameFrameOKClicked(const CEGUI::EventArgs &e);

	//scene query wrapper
	DuskObject* GetObjectAtMouse(const CEGUI::Point& pt);
};//class

}//namespace

#endif
