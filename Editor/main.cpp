#include "EditorApplication.h"
#include <CEGUI/CEGUI.h>
#include <OIS/OIS.h>
#include <OgreCEGUIRenderer.h>

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

class TutorialListener : public EditorFrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    TutorialListener(RenderWindow* win, Camera* cam)
        : EditorFrameListener(win, cam, true, true)
    {
       mContinue=true;
       mMouse->setEventCallback(this);
       mKeyboard->setEventCallback(this);

       CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
       CEGUI::Window *quit = wmgr->getWindow((CEGUI::utf8*)"Editor/QuitButton");

       quit->subscribeEvent(CEGUI::PushButton::EventClicked,
           CEGUI::Event::Subscriber(&TutorialListener::quit, this));

    } // CEGUIDemoListener

    bool frameStarted(const FrameEvent &evt)
    {
        mKeyboard->capture();
        mMouse->capture();

        return mContinue && !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
    }

    bool quit(const CEGUI::EventArgs &e)
    {
        mContinue = false;
        return true;
    }

    // MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg)
    {
        CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
        return true;
    }

    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
        return true;
    }

    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
    {
        CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
        return true;
    }

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &arg)
    {
        CEGUI::System *sys = CEGUI::System::getSingletonPtr();
        sys->injectKeyDown(arg.key);
        sys->injectChar(arg.text);
        return true;
    }

    bool keyReleased(const OIS::KeyEvent &arg)
    {
        CEGUI::System::getSingleton().injectKeyUp(arg.key);
        return true;
    }

private:
    bool mContinue;
};


class CEGUIDemoApplication : public EditorApplication
{
public:
    CEGUIDemoApplication()
        : mSystem(0), mRenderer(0)
    {
    }

    ~CEGUIDemoApplication()
    {
        if (mSystem)
            delete mSystem;

        if (mRenderer)
            delete mRenderer;
    }
protected:
   CEGUI::System *mSystem;
   CEGUI::OgreCEGUIRenderer *mRenderer;

    void createScene(void)
    {
       mRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
       mSystem = new CEGUI::System(mRenderer);

       CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
       mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
       mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

       CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();
       CEGUI::Window *sheet = win->createWindow("DefaultGUISheet", "Editor/Sheet");

       CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "Editor/QuitButton");
       quit->setText("Quit");
       quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

       sheet->addChildWindow(quit);


       /*quit = win->createWindow("TaharezLook/Button", "CEGUIDemo/AnotherButton");
       quit->setText("Klick mich!");
       quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.05, 0)));
       quit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.25, 0)));
       sheet->addChildWindow(quit);*/

       mSystem->setGUISheet(sheet);
    }

    void createFrameListener(void)
    {
        mFrameListener= new TutorialListener(mWindow, mCamera);
        mFrameListener->showDebugOverlay(true);
        mRoot->addFrameListener(mFrameListener);
    }

};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    CEGUIDemoApplication app;

    try {
        app.go();
    } catch(Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!",
            MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n",
            e.getFullDescription().c_str());
#endif
    }


    return 0;
}

