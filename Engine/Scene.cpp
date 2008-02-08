#include "Scene.h"
#include "API.h"
#include "Camera.h"
namespace Dusk
{

Scene::Scene()
{    //ctor
    m_SceneManager = getAPI().getOgreSceneManager();
    m_Camera = getAPI().getOgreCamera();
}

Scene::~Scene()
{
    //dtor
}
Scene& Scene::get()
{
    static Scene Instance;
    return Instance;
}
void Scene::createGrassMesh()
    {
        const float width = 25;
        const float height = 30;
        Ogre::ManualObject mo("GrassObject");

        Ogre::Vector3 vec(width/2, 0, 0);
        Ogre::Quaternion rot;
        rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

        mo.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        for (int i = 0; i < 3; ++i)
        {
            mo.position(-vec.x, height, -vec.z);
            mo.textureCoord(0, 0);

            mo.position(vec.x, height, vec.z);
            mo.textureCoord(1, 0);

            mo.position(-vec.x, 0, -vec.z);
            mo.textureCoord(0, 1);

            mo.position(vec.x, 0, vec.z);
            mo.textureCoord(1, 1);

            int offset = i * 4;
            mo.triangle(offset, offset+3, offset+1);
            mo.triangle(offset, offset+2, offset+3);

            vec = rot * vec;
        }
        mo.end();

        mo.convertToMesh("GrassBladesMesh");
    }

    void Scene::createScene()
    {
        createGrassMesh();
        m_SceneManager->setAmbientLight(Ogre::ColourValue(1, 1, 1));
        Dusk::Camera* cam = getAPI().getDuskCamera();
        cam->setPosition(Ogre::Vector3(150, 50, 150));
        cam->lookAt(Ogre::Vector3(0, 0, 0));

        Ogre::Entity *robot = m_SceneManager->createEntity("robot", "robot.mesh");
        m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(robot);

        Ogre::Plane plane;
        plane.normal = Ogre::Vector3::UNIT_Y;
        plane.d = 0;
        Ogre::MeshManager::getSingleton().createPlane("floor",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            450,450,10,10,true,1,50,50,Ogre::Vector3::UNIT_Z);
        Ogre::Entity* pPlaneEnt = m_SceneManager->createEntity("plane", "floor");
        pPlaneEnt->setMaterialName("Examples/GrassFloor");
        pPlaneEnt->setCastShadows(false);
        m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

        Ogre::Entity *grass = m_SceneManager->createEntity("grass", "GrassBladesMesh");
        Ogre::StaticGeometry *sg = m_SceneManager->createStaticGeometry("GrassArea");

        const int size = 375;
        const int amount = 20;

        sg->setRegionDimensions(Ogre::Vector3(size, size, size));
        sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

        for (int x = -size/2; x < size/2; x += (size/amount))
            for (int z = -size/2; z < size/2; z += (size/amount))
            {
                Ogre::Real r = size / (float)amount / 2;
                Ogre::Vector3 pos(x + Ogre::Math::RangeRandom(-r, r), 0, z + Ogre::Math::RangeRandom(-r, r));
                Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);
                Ogre::Quaternion orientation;
                orientation.FromAngleAxis(Ogre::Degree(Ogre::Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);

                sg->addEntity(grass, pos, orientation, scale);
            }

        sg->build();
    }
    void Scene::destroyScene()
    {

    }
}
