/*---------------------------------------------------------------------------
 Author:  walljumper
 Date:    2008-02-08
 Purpose: API Singleton class
          API holds pointers to some important Objects of Ogre and Dusk, and
          allows to access them.

 History:
     - 2007-12-30 (rev 17)  - initial version (by walljumper),
                              setOgreObjects() function
     - 2008-01-17 (rev 23)  - API is now singleton
                            - seperate set- and get-functions for each object
                              of Dusk
     - 2008-01-26 (rev 29)  - getConsole(), getDispatcher(), getScene() and
                              pointers to Console, Dispatcher and Scene removed
     - 2008-01-26 (rev 31)  - setConsole(), setDispatcher(), setScene() and
                              set- and get-functions for InputSystemBinding and
                              InputSystemEditor removed
     - 2008-01-30 (rev 33)  - Camera added (not finished)
     - 2008-02-08 (rev 34)  - setDuskCamera() and getDuskCamera() added
     - 2009-12-31 (rev 147) - documentation updated and some unneccessary lines
                              removed (by thoronador)

 ToDo list:
     - ???
 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef API_H
#define API_H
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
namespace Dusk
{
    class Application;
    class FrameListener;
    class Camera;

    // This class contains pointers to every single object
    class API
    {
        public:
            ~API(){}
            static API& getSingelton();
            //set functions
            /* sets pointers to all important Ogre objects */
            void setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr);
            void setApplication(Dusk::Application* app);
            void setDuskCamera(Dusk::Camera* cam);

            void setFrameListener(Dusk::FrameListener* op);
            //basic get functions
            /* returns pointer to Ogre's Root object */
            Ogre::Root* getOgreRoot();

            /* returns pointer to Ogre's Camera object */
            Ogre::Camera* getOgreCamera();

            /* returns pointer to RenderWindow */
            Ogre::RenderWindow* getOgreRenderWindow();

            /* returns pointer to Ogre's SceneManager */
            Ogre::SceneManager* getOgreSceneManager();

            Dusk::Application* getApplication();
            Dusk::FrameListener* getFrameListener();
            Dusk::Camera* getDuskCamera();
        protected:
        private:
        API();
        API(const API& op){}
        //Ogre Objects
        Ogre::Root* m_Root;
        Ogre::Camera* m_Camera;
        Ogre::RenderWindow* m_Window;
        Ogre::SceneManager* m_SceneManager;

        //Dusk Objects
        Dusk::Application* m_Application;
        Dusk::FrameListener* m_FrameListener;
        Dusk::Camera* m_DuskCamera;
    };
    inline API& getAPI() { return API::getSingelton(); }
}

#endif // API_H
