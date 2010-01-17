#include "Scene.h"
#include "API.h"
#include "Camera.h"
#include "Landscape.h"
#include "DuskConstants.h"
#include "DataLoader.h"
#include "ObjectData.h"
#include "AnimatedObject.h"
#include "AnimationData.h"
#include "Weather.h"

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

        /* robot and landscape are now part of DataLoader, or new file
           DuskData.dusk respectively, so we don't need this here.

        Ogre::Entity *robot = m_SceneManager->createEntity("robot", "robot.mesh");
        m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(robot);

        //Landscape: load
        if (Landscape::GetSingleton().LoadFromFile("data"+path_sep+"Landscape.dusk"))
        {
          if (Landscape::GetSingleton().SendToEngine(getAPI().getOgreSceneManager()))
          {
            std::cout << "Landscape loaded successfully.\n";
          }
        }*/

        if (DataLoader::GetSingleton().LoadFromFile("data"+path_sep+"DuskData.dusk"))
        {
          std::cout << "Data loaded successfully.\n";
          if (Landscape::GetSingleton().SendToEngine(getAPI().getOgreSceneManager()))
          {
            std::cout << "Landscape successfully added.\n";
          }
          ObjectData::GetSingleton().EnableAllObjects(getAPI().getOgreSceneManager());
        }

        //create animated object (for test purposes)
        AnimatedObject* aniObj = NULL;
        aniObj = AnimationData::GetSingleton().addAnimatedReference("robot",
                                    Ogre::Vector3(0.0f, 0.0f, 30.0f),
                                    Ogre::Vector3(0.0f, 0.0f, 0.0f), 0.55f);
        aniObj->Enable(m_SceneManager);
        aniObj->SetSpeed(12.5f);
        //aniObj->TravelToDestination(Ogre::Vector3(200.0f, 30.0f, 0.0f));
        aniObj->PlayAnimation("Walk", true);
        aniObj->AddWaypoint(Ogre::Vector3(200.0f, 30.0f, 0.0f));
        aniObj->AddWaypoint(Ogre::Vector3(200.0f, 50.0f, -200.0f));
        aniObj->AddWaypoint(Ogre::Vector3(0.0f, 0.0f, 30.0f));
        aniObj->setUseWaypoints(true);
        aniObj->setPatrolMode(true);
        //just out of curiosity
        std::cout << "Animated robot type enum (int): "<<(int)(aniObj->GetType())<<"\n"
                  << "  sizeof(DuskObject): "<<sizeof(DuskObject)<<" bytes\n"
                  << "  sizeof(Light): "<<sizeof(Light)<<" bytes\n"
                  << "  sizeof(Container): "<<sizeof(Container)<<" bytes\n"
                  << "  sizeof(AnimatedObject): "<<sizeof(AnimatedObject)<<" bytes\n"
                  << "  sizeof(NPC): "<<sizeof(NPC)<<" bytes\n";
        //end of animated object

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

        //fog effect
        Weather& w_singleton = Weather::getSingelton();
        w_singleton.setFogColour(0.9, 0.9, 0.9);
        w_singleton.startLinearFog(50.0, 500.0);
        w_singleton.startSnow();
    }

    void Scene::destroyScene()
    {
      Landscape::GetSingleton().RemoveFromEngine(getAPI().getOgreSceneManager());
      ObjectData::GetSingleton().ClearData();
    }
}
