#ifndef APPLICATION_H
#define APPLICATION_H
#include <Ogre.h>
#include "Scene.h"
#include "FrameListener.h"
#include <string>
namespace Dusk
{
    class Application : Dusk::FrameListener
    {
    public:
        Application();
        virtual ~Application();
        virtual void go();

    protected:
        virtual bool initialise();
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
        virtual bool frameStarted(const Ogre::FrameEvent& evt);
        virtual bool frameEnded(const Ogre::FrameEvent& evt);

        Ogre::Root* m_Root;
        Ogre::Camera* m_Camera;
        Ogre::SceneManager* m_SceneManager;
        Ogre::RenderWindow* m_Window;
        Ogre::FrameListener* m_FrameListener;
        Dusk::Scene* m_Scene;
    };
}

#endif // APPLICATION_H
