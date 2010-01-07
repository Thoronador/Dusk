#ifndef APPLICATION_H
#define APPLICATION_H
#include <Ogre.h>
#include "Scene.h"
#include "FrameListener.h"
#include <string>
namespace Dusk
{
    class API;
    class Camera;
    class Application
    {
    public:
        Application();
        virtual ~Application();
        virtual void go(std::string pluginFileName);
        Dusk::FrameListener* getFrameListener();

    protected:
        virtual bool initialise(std::string pluginFileName);
        virtual bool initOgreCore();

        virtual void createSceneManager();
        virtual void createCamera();
        virtual void createViewports();
        virtual void createResourceListener();
        virtual void createFrameListener();

        virtual void addResourceLocations();
        virtual void initResources();

        virtual void destroyScene();
        // FrameListener overrides

        Ogre::Root* m_Root;
        Ogre::Camera* m_Camera;
        Ogre::SceneManager* m_SceneManager;
        Ogre::RenderWindow* m_Window;
        Ogre::FrameListener* m_FrameListener;

        Dusk::Camera* m_DuskCamera;
    };
}

#endif // APPLICATION_H
