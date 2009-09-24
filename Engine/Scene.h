#ifndef SCENE_H
#define SCENE_H
#include <Ogre.h>

namespace Dusk
{
    class Scene
    {
    public:
        virtual ~Scene();
        static Scene& get();

        void createScene();
        void destroyScene();
    private:
        void createGrassMesh();
        Scene();
        Scene(const Scene& op){}

        Ogre::SceneManager* m_SceneManager;
        Ogre::Camera* m_Camera;
    };
}

#endif // SCENE_H
