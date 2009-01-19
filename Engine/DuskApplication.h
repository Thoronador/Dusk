#ifndef DUSKAPPLICATION_H
#define DUSKAPPLICATION_H

#include <Ogre.h>
#include "FrameListener.h"
using namespace Ogre;

class DuskApplication : public Dusk::FrameListener
{
public:
   DuskApplication();
   virtual ~DuskApplication();

   virtual void go();

protected:
   virtual bool initialise();
   virtual bool initOgreCore();

   virtual void createSceneManager();
   virtual void createCamera();
   virtual void createViewports();
   virtual void createResourceListener();

   virtual void addResourceLocations();
   virtual void initResources();

   virtual void createScene() = 0; // I am pure virtual, override me!
   virtual void destroyScene();

   Root *m_Root;
   Camera* m_Camera;
   SceneManager* m_SceneMgr;
   RenderWindow* m_Window;
};



#endif // DUSKAPPLICATION_H
