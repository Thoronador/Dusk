#ifndef APPLICATION_H
#define APPLICATION_H
#include <Ogre.h>
#include "Scene.h"
#include "FrameListener.h"
#include <string>
namespace Dusk
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        virtual void go(std::string pluginFileName);

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

        virtual void createScene() = 0; // I am pure virtual, override me!
        virtual void destroyScene();
        // FrameListener overrides


        Ogre::Root* m_Root;
        Ogre::Camera* m_Camera;
        Ogre::SceneManager* m_SceneManager;
        Ogre::RenderWindow* m_Window;
        Ogre::FrameListener* m_FrameListener;
        Dusk::Scene* m_Scene;
    };
}

#endif // APPLICATION_H
