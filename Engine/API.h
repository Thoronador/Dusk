/*---------------------------------------------------------------------------
 Author:  walljumper, thoronador
 Date:    2010-03-24
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
     - 2010-03-12 (rev 180) - changes to make API usable in Editor, too
     - 2010-03-12 (rev 181) - Ogre::Camera pointer and associated functions
                              removed, because the classes Dusk::Camera and
                              Dusk::EditorCamera can be used to access Ogre::Camera
     - 2010-03-24 (rev 186) - Pointer to Dusk::Camera removed, because it is now
                              singleton and can be accessed via static method

 ToDo list:
     - We should possibly try to reduce the number of object pointers which API
       holds in the long run to a minimum.
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef API_H
#define API_H
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
namespace Dusk
{
  #ifndef DUSK_EDITOR
    class Application;
    class FrameListener;
  #else
    class EditorApplication;
    class EditorFrameListener;
  #endif

    // This class contains pointers to every single object
  class API
  {
    public:
      ~API(){}
      static API& getSingelton();
      //set functions
      /* sets pointers to all important Ogre objects */
      void setOgreObjects(Ogre::Root* root,Ogre::RenderWindow* window,Ogre::SceneManager* mgr);

      #ifndef DUSK_EDITOR
      void setApplication(Dusk::Application* app);
      void setFrameListener(Dusk::FrameListener* op);
      #else
      void setApplication(EditorApplication* app);
      void setFrameListener(EditorFrameListener* op);
      #endif
      //basic get functions
      /* returns pointer to Ogre's Root object */
      Ogre::Root* getOgreRoot();

      /* returns pointer to RenderWindow */
      Ogre::RenderWindow* getOgreRenderWindow();

      /* returns pointer to Ogre's SceneManager */
      Ogre::SceneManager* getOgreSceneManager();

      #ifndef DUSK_EDITOR
      Dusk::Application* getApplication();
      Dusk::FrameListener* getFrameListener();
      #else
      EditorApplication* getApplication();
      EditorFrameListener* getFrameListener();
      #endif
    protected:
    private:
      API();
      API(const API& op){}
      //Ogre Objects
      Ogre::Root* m_Root;
      Ogre::RenderWindow* m_Window;
      Ogre::SceneManager* m_SceneManager;

      #ifndef DUSK_EDITOR
      //Dusk Objects
      Dusk::Application* m_Application;
      Dusk::FrameListener* m_FrameListener;
      #else
      //Editor Objects
      EditorApplication* m_Application;
      EditorFrameListener* m_FrameListener;
      #endif
  };
  inline API& getAPI() { return API::getSingelton(); }
}

#endif // API_H
