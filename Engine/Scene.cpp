/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010, 2011 ssj5000, walljumper, thoronador

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

#include "Scene.h"
#include "API.h"
#include "Camera.h"
#include "Database.h"
#include "Landscape.h"
#include "DuskConstants.h"
#include "DataLoader.h"
#include "ObjectManager.h"
#include "AnimatedObject.h"
#include "InjectionManager.h"
#include "Weather.h"
#include "Player.h"
#include "NPCRecord.h"
#include "WeaponBase.h"
#include "ProjectileBase.h"
#include "QuestLog.h"
#include "Messages.h"

namespace Dusk
{

Scene::Scene()
{    //ctor
    m_SceneManager = getAPI().getOgreSceneManager();
    m_Camera = Camera::getSingleton().getOgreCamera();
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

/*void createArcMesh()
{
  Ogre::ManualObject man_ob("ArcObject");

  Ogre::Quaternion quat;
  man_ob.begin("", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  const unsigned int arc_steps = 16;


  unsigned int i;
  for (i=0; i<=arc_steps; ++i)
  {
    quat.FromAngleAxis(Ogre::Degree(i*180.0f/arc_steps), Ogre::Vector3::UNIT_Y);
    man_ob.position(quat*Ogre::Vector3(100, 20, 0));
    man_ob.position(quat*Ogre::Vector3(100, -20, 0));
  }
  //now to indices
  for (i=0; i<arc_steps; ++i)
  {
    man_ob.quad(i*2, i*2+1, i*2+3, i*2+2);
    //man_ob.triangle(i*2, i*2+1, i*2+2);
    //man_ob.triangle(i*2+2,i*2+1, i*2+3);
  }

  man_ob.end();
  man_ob.convertToMesh("Arc.mesh");
}*/

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
        Dusk::Camera& cam = Camera::getSingleton();
        cam.setPosition(Ogre::Vector3(150, 50, 150));
        cam.lookAt(Ogre::Vector3(0, 0, 0));

        if (DataLoader::getSingleton().loadFromFile("data"+path_sep+"DuskData.dusk"))
        {
          DuskLog() << "Data loaded successfully.\n";
          if (Landscape::getSingleton().sendToEngine(getAPI().getOgreSceneManager()))
          {
            DuskLog() << "Landscape successfully added.\n";
          }
          ObjectManager::getSingleton().enableAllObjects(getAPI().getOgreSceneManager());
        }

        //create waypoint object (for test purposes)
        WaypointObject* wpObj = NULL;
        wpObj = InjectionManager::getSingleton().addWaypointReference("robot",
                                    Ogre::Vector3(0.0f, 0.0f, 30.0f),
                                    Ogre::Quaternion::IDENTITY, 0.55f);
        wpObj->enable(m_SceneManager);
        wpObj->setSpeed(12.5f);
        //aniObj->TravelToDestination(Ogre::Vector3(200.0f, 30.0f, 0.0f));
        //aniObj->PlayAnimation("Walk", true);
        wpObj->addWaypoint(Ogre::Vector3(200.0f, 30.0f, 0.0f));
        wpObj->addWaypoint(Ogre::Vector3(200.0f, 50.0f, -200.0f));
        wpObj->addWaypoint(Ogre::Vector3(0.0f, 0.0f, 30.0f));
        wpObj->setUseWaypoints(true);
        wpObj->setPatrolMode(true);
        //just out of curiosity
        DuskLog() << "Animated robot type enum (int): "<<(int)(wpObj->getDuskType())<<"\n"
                  << "  sizeof(DuskObject): "<<sizeof(DuskObject)<<" bytes\n"
                  << "  sizeof(Light): "<<sizeof(Light)<<" bytes\n"
                  << "  sizeof(Container): "<<sizeof(Container)<<" bytes\n"
                  << "  sizeof(AnimatedObject): "<<sizeof(AnimatedObject)<<" bytes\n"
                  << "  sizeof(WaypointObject): "<<sizeof(WaypointObject)<<" bytes\n"
                  << "  sizeof(NPC): "<<sizeof(NPC)<<" bytes\n"
                  << "  sizeof(Player): "<<sizeof(Player)<<" bytes\n";
        //end of waypoint object

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

        //weather effects
        Weather& w_singleton = Weather::getSingelton();
        /* //fog does not look good with sun yet, so don't enable it here
        w_singleton.setFogColour(0.9, 0.9, 0.9);
        w_singleton.startLinearFog(50.0, 500.0);*/
        w_singleton.startSnow();
        //add sword item to weapon base
        WeaponRecord wrec;
        wrec.ID = "sword";
        wrec.DamageTimes = 2; // 2d8 damage
        wrec.DamageDice = 8;
        wrec.Mesh = "Sword.mesh";
        wrec.Name = "Sword";
        wrec.ProjectileID = "";
        wrec.Range = 75.0f;
        wrec.TimeBetweenAttacks = 2.0f; //2 secs cooldown
        wrec.Type = wtMelee;
        wrec.value = 100;
        wrec.weight = 2.5f;
        Database::getSingleton().addRecord(wrec);
        //add projectile-shooting sword to weapon base
        wrec.Name = "Sword shooting swords";
        wrec.ProjectileID = "sword_projectile";
        wrec.ID = "sword_gun";
        wrec.Type = wtGun;
        Database::getSingleton().addRecord(wrec);
        //add projectile information
        ProjectileRecord proj_rec;
        proj_rec.ID = "sword_projectile";
        proj_rec.Mesh = "Sword.mesh";
        proj_rec.DefaultTTL = 25.0;
        proj_rec.DefaultVelocity = 40.0;
        proj_rec.times = 2;
        proj_rec.dice = 8;
        Database::getSingleton().addRecord(proj_rec);
        /*ProjectileBase::getSingleton().addProjectile("sword_projectile",
                                                "Sword.mesh", 25.0, 40.0, 2, 8);*/
        //add player information to NPCBase
        NPCAnimations anims;
        anims.Death = "Dance";
        anims.Idle = "IdleBase,IdleTop";
        anims.Jump = "JumpLoop";
        anims.MeleeAttack = "SliceHorizontal";
        anims.ProjectileAttack = "SliceVertical";
        anims.Walk = "RunBase,RunTop";
        NPCTagPoints tps;
        tps.HandLeft = "Handle.L";
        tps.HandRight = "Handle.R";
        tps.SheathLeft = "Sheath.L";
        tps.SheathRight = "Sheath.R";
        NPCRecord npc_rec;
        npc_rec.Animations = anims;
        npc_rec.TagPoints = tps;
        npc_rec.ID = "player";
        npc_rec.Name = "The Player him-/herself";
        npc_rec.Mesh = "Sinbad.mesh";
        npc_rec.Level = 1;
        npc_rec.Attributes = NPCAttributes::getNullAttributes();
        npc_rec.Female = false;
        npc_rec.InventoryAtStart = Inventory::getEmptyInventory();
        Database::getSingleton().addRecord(npc_rec);
        /*NPCBase::getSingleton().addNPC("player", "The Player him-/herself",
                        "Sinbad.mesh", 1, NPCAttributes::getNullAttributes(),
                        false, Inventory::getEmptyInventory(), anims, tps);*/
        if (Database::getSingleton().hasRecord("player"))
        {
          DuskLog() << "Debug: Database got player entry.\n";
          DuskLog() << "Debug: Tag points of \"player\":\n";
          const NPCTagPoints& tttp = Database::getSingleton().getTypedRecord<NPCRecord>("player").TagPoints;
          DuskLog() << "  Left  hand:   \""<<tttp.HandLeft<<"\"\n"
                    << "  Right hand:   \""<<tttp.HandRight<<"\"\n"
                    << "  Left  sheath: \""<<tttp.SheathLeft<<"\"\n"
                    << "  Right sheath: \""<<tttp.SheathRight<<"\"\n";
        }
        else
        {
          DuskLog() << "Debug: NPCBase does NOT have a player entry.\n";
        }
        //add player mesh
        Player::getSingleton().getInventory().addItem("sword", 5);
        DuskLog() << "Player now has "<<Player::getSingleton().getConstInventory().getItemCount("sword")
                  << " swords in inventory.\n";
        Player::getSingleton().getInventory().addItem("sword_gun", 5);
        DuskLog() << "Player now has "<<Player::getSingleton().getConstInventory().getItemCount("sword_gun")
                  << " sword guns in inventory.\n";
        Player::getSingleton().setScale(5.0f);
        Player::getSingleton().enable(m_SceneManager);
        Player::getSingleton().setRotation(Ogre::Quaternion(Ogre::Degree(180.0), Ogre::Vector3::UNIT_Y));
        if (Player::getSingleton().equip("sword"))
        {
          DuskLog() << "First equip() successful.\n";
          if (Player::getSingleton().equip("sword_gun"))
          {
            DuskLog() << "Second equip() successful.\n";
          }
        }
        else
        {
          DuskLog() << "Player::equip() failed.\n";
        }
        Camera::getSingleton().setZoom(125.0f);
        // ---- begin questlog test
        QuestLog::getSingleton().addQuestEntry("test_quest", 1);
        QuestLog::getSingleton().addQuestEntry("test_quest", 2);
        QuestLog::getSingleton().addQuestEntry("test_quest", 3);
        // ---- end of questlog test


        // ---- diamond square algorithm test
        LandscapeRecord* lrec = Landscape::getSingleton().createRecord();
        if (lrec->generateByDiamondSquare(-10.0, LandscapeRecord::cDefaultStride*64*0.5, 1.0))
        {
          std::cout << "Diamond square generation successful!\n";
          lrec->moveTo(LandscapeRecord::cDefaultStride*64*-1.5, 0.0);
          if (lrec->enable(m_SceneManager, true))
          {
            std::cout << "Enabling wire frame successful!\n";
          }//if
          else
          {
            std::cout << "Enabling wire framed terrain failed!\n";
          }

        }//if
        // ------ next
        lrec = Landscape::getSingleton().createRecord();
        if (lrec->generateByDiamondSquare(-10.0, LandscapeRecord::cDefaultStride*64*0.5, 0.75))
        {
          std::cout << "Diamond square generation successful!\n";
          lrec->moveTo(LandscapeRecord::cDefaultStride*64*-1.5, LandscapeRecord::cDefaultStride*64*1.5);
          if (lrec->enable(m_SceneManager, true))
          {
            std::cout << "Enabling wire frame successful!\n";
          }//if
          else
          {
            std::cout << "Enabling wire framed terrain failed!\n";
          }

        }//if
        // ------ next
        lrec = Landscape::getSingleton().createRecord();
        if (lrec->generateByDiamondSquare(-10.0, LandscapeRecord::cDefaultStride*64*0.5, 0.25))
        {
          std::cout << "Diamond square generation successful!\n";
          lrec->moveTo(LandscapeRecord::cDefaultStride*64*-1.5, LandscapeRecord::cDefaultStride*64*3.0);
          if (lrec->enable(m_SceneManager, true))
          {
            std::cout << "Enabling wire frame successful!\n";
          }//if
          else
          {
            std::cout << "Enabling wire framed terrain failed!\n";
          }

        }//if

        // ---- end of diamond square algorithm test


        /*
        //object test
        createArcMesh();
        ObjectBase::getSingleton().addObject("Arc", "Arc.mesh", true);
        DuskObject* obj=
        ObjectManager::getSingleton().addObjectReference("Arc", Ogre::Vector3(50, 0, -200),
                              Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X), 1.0f);
        obj->enable(m_SceneManager);*/
    }

    void Scene::destroyScene()
    {
      DuskLog() << "Scene destruction in progress...\n";
      DuskLog() << "  Delete objects...\n";
      DataLoader::getSingleton().clearData(ALL_BITS);
      DuskLog()<<"  Delete weather effects...\n";
      Weather& w_singleton = Weather::getSingelton();
      if (w_singleton.isFoggy()) w_singleton.stopFog();
      if (w_singleton.isRaining()) w_singleton.stopRain();
      if (w_singleton.isSnowing()) w_singleton.stopSnow();
      w_singleton.deleteAllCelestials();
      DuskLog() << "  Delete Landscape...\n";
      Landscape::getSingleton().removeFromEngine(getAPI().getOgreSceneManager());
      Landscape::getSingleton().clearAllRecords();
      DuskLog() << "Scene destroyed.\n";
    }
} //namespace
