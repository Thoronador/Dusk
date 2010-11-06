/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2009, 2010 thoronador

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

#ifndef EDITORFRAMELISTENER_H
#define EDITORFRAMELISTENER_H

#include <Ogre.h>
#include <OgreStringConverter.h>

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include <CEGUI/CEGUI.h>

#include "../Engine/API.h"

namespace Dusk
{

enum EditorMode
{
  EM_Movement, EM_Lists, EM_LandscapeUp, EM_LandscapeDown, EM_LandscapeColour
};

class EditorFrameListener: public Ogre::FrameListener, public Ogre::WindowEventListener,
                           public OIS::MouseListener, public OIS::KeyListener
{
protected:
	virtual void updateStats(void)
	{
		static Ogre::String currFps = "Current FPS: ";
		static Ogre::String avgFps = "Average FPS: ";
		static Ogre::String bestFps = "Best FPS: ";
		static Ogre::String worstFps = "Worst FPS: ";
		static Ogre::String tris = "Triangle Count: ";
		static Ogre::String batches = "Batch Count: ";

		// update stats when necessary
		try {
			Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
			Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
			Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
			Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

			const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
			guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
				+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
				+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

			Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
			guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

			Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
			guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));

			Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
			guiDbg->setCaption(mDebugText);
		}
		catch(...) { /* ignore */ }
	}

public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	EditorFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
			     bool bufferedJoy = false ) :
		mCamera(cam), mTranslateVector(Ogre::Vector3::ZERO), mCurrentSpeed(0), mWindow(win), mStatsOn(true), mNumScreenShots(0),
		mMoveScale(0.0f), mRotScale(0.0f), mTimeUntilNextToggle(0), mFiltering(Ogre::TFO_BILINEAR),
		mAniso(1), mSceneDetailIndex(0), mMoveSpeed(100), mRotateSpeed(36), mDebugOverlay(0),
		mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
	{
		mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		win->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem( pl );

		//Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, bufferedKeys ));
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, bufferedMouse ));
		try {
			mJoy = static_cast<OIS::JoyStick*>(mInputManager->createInputObject( OIS::OISJoyStick, bufferedJoy ));
		}
		catch(...) {
			mJoy = 0;
		}

		//Set initial mouse clipping size
		windowResized(mWindow);

		showDebugOverlay(true);

		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

        mContinue=true;
        m_WireFrameLandscape = false;
		mMouse->setEventCallback(this);
        mKeyboard->setEventCallback(this);

        CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
        CEGUI::MenuItem *quit = static_cast<CEGUI::MenuItem*> (wmgr->getWindow((CEGUI::utf8*)"Editor/MenuBar/File/PopUp/Quit"));
        quit->subscribeEvent(CEGUI::MenuItem::EventClicked,
            CEGUI::Event::Subscriber(&EditorFrameListener::quit, this));
        m_Mode = EM_Lists;

        getAPI().setFrameListener(this);
	}

	bool frameStarted(const Ogre::FrameEvent &evt);

	// MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg);

    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &arg);

    bool keyReleased(const OIS::KeyEvent &arg);

    bool quit(const CEGUI::EventArgs &e);

	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);

	virtual ~EditorFrameListener();

	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent& evt);

	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent& evt);

	virtual void moveCamera();

	virtual void showDebugOverlay(bool show);

	// Override frameRenderingQueued event to process that (don't care about frameEnded)
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	bool frameEnded(const Ogre::FrameEvent& evt);

	//mode for application
	void setEditorMode(const EditorMode em);
	EditorMode getEditorMode(void);

	void setWireFrame(const bool wf);

    //wrapper functions to query keyboard and mouse state
	bool IsKeyDown(const OIS::KeyCode kc);
	bool IsMouseDown(const OIS::MouseButtonID mb);

protected:
	Ogre::Camera* mCamera;

	Ogre::Vector3 mTranslateVector;
	Ogre::Real mCurrentSpeed;
	Ogre::RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	float mSpeedLimit;
	Ogre::Degree mRotScale;
	// just to stop toggles flipping too fast
	Ogre::Real mTimeUntilNextToggle ;
	Ogre::Radian mRotX, mRotY;
	Ogre::TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Ogre::Real mMoveSpeed;
	Ogre::Degree mRotateSpeed;
	Ogre::Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;

    EditorMode m_Mode;

	bool mContinue;
	bool m_WireFrameLandscape;
};//class

}//namespace

#endif // EDITORFRAMELISTENER_H
