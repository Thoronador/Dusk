/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 ssj5000, walljumper, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
     - 2010-08-07 (rev 222) - adjustments for weapon records in WeaponBase and
                              player's equipment
     - 2010-08-15 (rev 228) - destroyScene() does now destroy a bit more :P
     - 2010-08-22 (rev 233) - adjustments for Sun
     - 2010-08-27 (rev 236) - error in player's animation string fixed
     - 2010-11-13 (rev 254) - update due to renaming of some functions in
                              Landscape class
     - 2010-11-20 (rev 255) - rotation is now handled as Quaternion

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
