#ifndef EditorApplication_h
#define EditorApplication_h

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "EditorFrameListener.h"
#include "../Engine/ItemBase.h"
#include <OgreCEGUIRenderer.h>
#include <CEGUI/CEGUI.h>
#include <string>
#include <vector>

namespace Dusk
{

typedef struct FileEntry {
                 std::string FileName;
                 bool IsDirectory;
};//struct

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
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    EditorFrameListener* mFrameListener;
    Ogre::RenderWindow* mWindow;

	CEGUI::System *mSystem;
    CEGUI::OgreCEGUIRenderer *mRenderer;

    //application specific
    std::string LoadedDataFile;
    std::string LoadFrameDirectory;
    std::vector<FileEntry> LoadFrameFiles;

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
	//void CreateCEGUIMenu(void);
	void CreateCEGUIMenuBar(void);
	void CreateCEGUICatalogue(void);
	void CreatePopupMenus(void);
	void showCEGUILoadWindow(void);
	void UpdateLoadWindowFiles(const std::string Directory);
	void showWarning(const std::string Text_of_warning);
	void showHint(const std::string hint_text);

	void addItemRecordToCatalogue(const std::string& ID, const ItemRecord& ItemData);
	void addObjectRecordToCatalogue(const std::string& ID, const std::string& Mesh);

	//callbacks for buttons
	bool LoadButtonClicked(const CEGUI::EventArgs &e);
	bool LoadFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LoadFrameOKClicked(const CEGUI::EventArgs &e);
	bool WarningFrameOKClicked(const CEGUI::EventArgs &e);
	bool HintFrameOKClicked(const CEGUI::EventArgs &e);
	bool ObjectTabClicked(const CEGUI::EventArgs &e);

	//callbacks for popup menus
	bool ObjectNewClicked(const CEGUI::EventArgs &e);
	bool ObjectEditClicked(const CEGUI::EventArgs &e);
	bool ObjectDeleteClicked(const CEGUI::EventArgs &e);

};//class

}//namespace

#endif
