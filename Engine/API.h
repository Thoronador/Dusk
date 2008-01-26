#ifndef API_H
#define API_H
#include <Ogre.h>
namespace Dusk
{
    class Application;
    class FrameListener;
    class Console;
    class Dispatcher;
    class InputSystemBinding;
    class InputSystemEditor;
    class Scene;

    // This class contains pointers to every single object
    class API
    {
        public:
            ~API(){}
            static API& getSingelton();
            //set functions
            void setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr);
            void setApplication(Dusk::Application* app);

            void setFrameListener(Dusk::FrameListener* op);
                        //basic get functions
            Ogre::Root* getOgreRoot();
            Ogre::Camera* getOgreCamera();
            Ogre::RenderWindow* getOgreRenderWindow();
            Ogre::SceneManager* getOgreSceneManager();

            Dusk::Application* getApplication();
            Dusk::FrameListener* getFrameListener();
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

    };
    inline API& getAPI() { return API::getSingelton(); }
}

#endif // API_H
