// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------
#include <Ogre.h>

#include "Application.h"
using namespace Ogre;
// ----------------------------------------------------------------------------
// Test Class
// ----------------------------------------------------------------------------
class TutorialApplication : public Dusk::Application
{
protected:
public:
    TutorialApplication()
    {
    }

    ~TutorialApplication()
    {
    }
protected:
    MeshPtr mGrassMesh;

    void createGrassMesh()
    {
        const float width = 25;
        const float height = 30;
        ManualObject mo("GrassObject");

        Vector3 vec(width/2, 0, 0);
        Quaternion rot;
        rot.FromAngleAxis(Degree(60), Vector3::UNIT_Y);

        mo.begin("Examples/GrassBlades", RenderOperation::OT_TRIANGLE_LIST);
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

    void createScene(void)
    {
        createGrassMesh();
        m_SceneManager->setAmbientLight(ColourValue(1, 1, 1));

        m_Camera->setPosition(150, 50, 150);
        m_Camera->lookAt(0, 0, 0);

        Entity *robot = m_SceneManager->createEntity("robot", "robot.mesh");
        m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(robot);

        Plane plane;
        plane.normal = Vector3::UNIT_Y;
        plane.d = 0;
        MeshManager::getSingleton().createPlane("floor",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
            450,450,10,10,true,1,50,50,Vector3::UNIT_Z);
        Entity* pPlaneEnt = m_SceneManager->createEntity("plane", "floor");
        pPlaneEnt->setMaterialName("Examples/GrassFloor");
        pPlaneEnt->setCastShadows(false);
        m_SceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(pPlaneEnt);

        Entity *grass = m_SceneManager->createEntity("grass", "GrassBladesMesh");
        StaticGeometry *sg = m_SceneManager->createStaticGeometry("GrassArea");

        const int size = 375;
        const int amount = 20;

        sg->setRegionDimensions(Vector3(size, size, size));
        sg->setOrigin(Vector3(-size/2, 0, -size/2));

        for (int x = -size/2; x < size/2; x += (size/amount))
            for (int z = -size/2; z < size/2; z += (size/amount))
            {
                Real r = size / (float)amount / 2;
                Vector3 pos(x + Math::RangeRandom(-r, r), 0, z + Math::RangeRandom(-r, r));
                Vector3 scale(1, Math::RangeRandom(0.9, 1.1), 1);
                Quaternion orientation;
                orientation.FromAngleAxis(Degree(Math::RangeRandom(0, 359)), Vector3::UNIT_Y);

                sg->addEntity(grass, pos, orientation, scale);
            }

        sg->build();
    }
};
// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    TutorialApplication app;

    try
    {
        app.go();
    }
    catch( Ogre::Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else

        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }

    return 0;
}
