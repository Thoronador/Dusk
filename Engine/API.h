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
    class Camera;

    // This class contains pointers to every single object
    class API
    {
        public:
            ~API(){}
            static API& getSingelton();
            //set functions
            void setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr);
            void setApplication(Dusk::Application* app);
            void setDuskCamera(Dusk::Camera* cam);

            void setFrameListener(Dusk::FrameListener* op);
                        //basic get functions
            Ogre::Root* getOgreRoot();
            Ogre::Camera* getOgreCamera();
            Ogre::RenderWindow* getOgreRenderWindow();
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
