#ifndef EditorApplication_h
#define EditorApplication_h

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "EditorFrameListener.h"
#include <OgreCEGUIRenderer.h>
#include <CEGUI/CEGUI.h>
#include <string>


namespace Dusk
{

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
	//Ogre::String mResourcePath;

	CEGUI::System *mSystem;
    CEGUI::OgreCEGUIRenderer *mRenderer;

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
	void CreateCEGUIMenu(void);
	void showCEGUILoadWindow(void);

	void showWarning(const std::string Text_of_warning);

	//callbacks for buttons
	bool LoadButtonClicked(const CEGUI::EventArgs &e);
	bool LoadFrameCancelClicked(const CEGUI::EventArgs &e);
	bool LoadFrameOKClicked(const CEGUI::EventArgs &e);
	bool WarningFrameOKClicked(const CEGUI::EventArgs &e);

};//class

}//namespace

#endif
