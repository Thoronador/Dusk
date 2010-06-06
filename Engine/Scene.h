/*---------------------------------------------------------------------------
 Authors: ssj5000, walljumper, thoronador
 Date:    2010-03-12
 Purpose: Scene Singleton class
          The Scene class manages all objects shown in game.

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2008-01-17 (rev 23)  - Scene registers itself at API (by walljumper)
     - 2008-01-24 (rev 26)  - Scene is now Singleton
     - 2008-01-26 (rev 29)  - createScene(), destroyScene(), createGassmesh()
                              introduced; a small scene is created
     - 2008-02-08 (rev 34)  - camera adjusted to view scene from a better spot
     - 2009-02-13 (rev 80)  - landscape is loaded into scene from data file (by
                              thoronador)
     - 2009-07-13 (rev 104) - landscape is removed during destroyScene()
     - 2009-07-15 (rev 105) - DataLoader is used to load scene objects from a
                              data file and put them into the scene
     - 2009-09-10 (rev 127) - animated, walking robot added to scene
     - 2009-12-05 (rev 139) - waypoints for walking robot added
     - 2009-12-08 (rev 140) - fog (linear type) added
     - 2010-01-03 (rev 149) - snow (particle system) added to scene
     - 2010-01-16 (rev 156) - animated robot patrols between waypoints
     - 2010-03-12 (rev 181) - documentation (i.e. this block comment) added
     - 2010-05-20 (rev 205) - adjustments for new object hierarchy
     - 2010-05-21 (rev 206) - adjustments for player's movement
     - 2010-06-06 (rev 215) - adjustments for player record in NPCBase

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SCENE_H
#define SCENE_H
#include <OgreCamera.h>
#include <OgreSceneManager.h>

namespace Dusk
{
    class Scene
    {
    public:
        /* destructor */
        virtual ~Scene();

        /* singleton access */
        static Scene& get();

        /* creates a few objects for the scene */
        void createScene();

        /* destroys objects in scene */
        void destroyScene();
    private:
        /* utility function to create a grass mesh */
        void createGrassMesh();

        /* constructor */
        Scene();

        /* copy constructor - empty, because we are singleton */
        Scene(const Scene& op){}

        Ogre::SceneManager* m_SceneManager;
        Ogre::Camera* m_Camera;
    };
}

#endif // SCENE_H
