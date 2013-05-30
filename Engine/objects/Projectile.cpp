/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2012, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Projectile.h"
#include "../database/ProjectileRecord.h"
#include "../database/Database.h"
#include "../InjectionManager.h"
#include "../DuskConstants.h"
#include "../Landscape.h"
#include "../DiceBox.h"
#include "../Messages.h"
#include "AnyConversion.h"

namespace Dusk
{

const float Projectile::InfiniteTTL = -1.0f;

Projectile::Projectile()
  : UniformMotionObject(),
    m_TTL(InfiniteTTL),
    m_Emitter(NULL)
{
}

Projectile::Projectile(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
  : UniformMotionObject(_ID, pos, rot, Scale),
    m_Emitter(NULL)
{
  if (Database::getSingleton().hasTypedRecord<ProjectileRecord>(_ID))
  {
    const ProjectileRecord& rec = Database::getSingleton().getTypedRecord<ProjectileRecord>(_ID);
    m_TTL = rec.DefaultTTL;
    m_Speed = rec.DefaultVelocity;
  }
  else
  {
    m_TTL = InfiniteTTL;
  }
}

Projectile::~Projectile()
{
  disable();
}

ObjectTypes Projectile::getDuskType() const
{
  return otProjectile;
}

bool Projectile::canCollide() const
{
  return true;
}

void Projectile::setTTL(const float newTTL)
{
  m_TTL = newTTL;
}

float Projectile::getTTL() const
{
  return m_TTL;
}

void Projectile::setEmitter(DuskObject* emitter)
{
  m_Emitter = emitter;
}

DuskObject* Projectile::getEmitter() const
{
  return m_Emitter;
}

void Projectile::injectTime(const float SecondsPassed)
{
  if (m_Speed>0.0f and m_Direction!=Ogre::Vector3::ZERO and isEnabled())
  {
    //perform scene query to check whether projectile hit something
    Ogre::SceneManager* scm = entity->getParentSceneNode()->getCreator();
    const Ogre::Ray ray = Ogre::Ray(position, m_Direction);
    Ogre::RaySceneQuery* rsq = scm->createRayQuery(ray);
    rsq->setSortByDistance(true);
    Ogre::RaySceneQueryResult& result = rsq->execute();
    unsigned int i;

    DuskObject* hit_object = NULL;
    LandscapeRecord* hit_land = NULL;
    Ogre::Real hit_squareDistance = -1.0f;

    for (i=0; i<result.size(); ++i)
    {
      //If distance is greater then projectile would travel in this frame,
      // then quit here.
      if (result.at(i).distance>SecondsPassed*m_Speed) break;
      if (result.at(i).movable!=NULL and result.at(i).movable!=entity)
      {
        //is it a landscape record?
        if (LandscapeRecord::isLandscapeRecordName(result.at(i).movable->getName()))
        {
          Ogre::Vector3 vec_i = ray.getPoint(result.at(i).distance);
          LandscapeRecord* land_rec = Landscape::getSingleton().getRecordAtXZ(vec_i.x, vec_i.z);
          if (land_rec!=NULL)
          {
            //Is landscape really hit by this ray?
            if (land_rec->isHitByRay(ray, vec_i))
            {
              //Is it near enough to be hit in this frame?
              const Ogre::Real sq_dist = vec_i.squaredDistance(position);
              if (sq_dist<=Ogre::Math::Sqr(SecondsPassed*m_Speed))
              {
                //Is the distance the shortest so far?
                if ((sq_dist<hit_squareDistance) or (hit_squareDistance<0.0f))
                {
                  hit_squareDistance = sq_dist; //new shortest square distance
                  hit_land = land_rec; //set land to new nearest landscape record
                  hit_object = NULL; //set to NULL, because any previously set
                                     // object is not the nearest any more
                }//if shortest distance
              }
            }//if hit by ray
          }//land!=NULL
        }//if movalbe is landscape record
        else
        { //no landscape, so it must be a DuskObject (or derived type)
          DuskObject* obj = AnyToObjectPtr(result.at(i).movable->getUserAny());
          if (obj!=NULL and obj!=m_Emitter and obj->canCollide())
          {
            Ogre::Vector3 vec_i(0.0, 0.0, 0.0);
            //Is object really hit by this ray?
            if (obj->isHitByRay(ray, vec_i))
            {
              const Ogre::Real sq_dist = vec_i.squaredDistance(position);
              //Will it still be hit in this frame?
              if (sq_dist <= Ogre::Math::Sqr(SecondsPassed*m_Speed))
              {
                //Is it the shortest distance so far?
                if ((sq_dist<hit_squareDistance) or (hit_squareDistance<0.0f))
                {
                  hit_squareDistance = sq_dist; //set new shortest distance
                  hit_object = obj; //set new nearest object
                  hit_land = NULL; //Set landscape record to NULL, because it is
                                   //not the nearest object on the ray any more.
                }//if shortest
              }
            }//if object is hit by ray

          }//if object not NULL and eligible for collision
        }//else
      }//if movable not NULL
    }//for i

    //now delete the scene query
    scm->destroyQuery(rsq);
    rsq = NULL;

    if (hit_land!=NULL)
    {
      //Landscape is the nearest object, thus projectile will just vanish.
      // --> request deletetion of projectile
      InjectionManager::getSingleton().requestDeletion(this);
      return;
    }

    if (hit_object!=NULL)
    {
      const ObjectTypes ho_type = hit_object->getDuskType();
      //hit a static object (or unequipped item or weapon)?
      if ((ho_type==otStatic or ho_type==otAnimated or ho_type==otWaypoint
          or ho_type==otContainer)
          or ((ho_type==otItem or ho_type==otWeapon)
               and !(static_cast<Item*>(hit_object))->isEquipped()))
      {
        //request deletion of object
        InjectionManager::getSingleton().requestDeletion(this);
        return;
      }
      //Is object an NPC nad not the one who shot the projectile?
      else if (ho_type==otNPC and hit_object!=m_Emitter)
      {
        //projectile will hit the NPC within this frame
        // --> inflict damage
        Dusk::NPC* npc_ptr = dynamic_cast<NPC*> (hit_object);
        const ProjectileRecord& pr = Database::getSingleton().getTypedRecord<ProjectileRecord>(this->ID);
        switch (pr.dice)
        {
          case 4:
               npc_ptr->inflictDamage(DiceBox::getSingleton().d4(pr.times));
               break;
          case 6:
               npc_ptr->inflictDamage(DiceBox::getSingleton().d6(pr.times));
               break;
          case 8:
               npc_ptr->inflictDamage(DiceBox::getSingleton().d8(pr.times));
               break;
          case 10:
               npc_ptr->inflictDamage(DiceBox::getSingleton().d10(pr.times));
               break;
          case 20:
               npc_ptr->inflictDamage(DiceBox::getSingleton().d20(pr.times));
               break;
          default:
               DuskLog() << "Projectile::injectTime: ERROR: projectile \""<<ID
                         << "\" has invalid die number ("<<pr.dice<<").\n";
                         break;
        }//switch
        // --> request projectile deletetion
        InjectionManager::getSingleton().requestDeletion(this);
        return;
      }//if object is NPC
    }//if an object was hit
  }//if moving

  //perform movement
  UniformMotionObject::injectTime(SecondsPassed);
  //delete object, if time has come
  if (m_TTL>0.0f)
  {
    m_TTL = m_TTL - SecondsPassed;
    if (m_TTL<=0.0f)
    {
      //we are done here, so delete this object
      InjectionManager::getSingleton().requestDeletion(this);
      return;
    }
  }//if
}

const std::string& Projectile::getObjectMesh() const
{
  return Database::getSingleton().getTypedRecord<ProjectileRecord>(ID).Mesh;
}

void Projectile::travelToDestination(const Ogre::Vector3& dest)
{
  m_Destination = Ogre::Vector3::ZERO;
  m_Travel = false;
  setDirection(dest-getPosition());
}

bool Projectile::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Projectile::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefP" (reference of Projectile)
  OutStream.write((char*) &cHeaderRefP, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Projectile::saveToStream: ERROR while writing basic data!\n";
    return false;
  }
  //write all data inherited from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    DuskLog() << "Projectile::saveToStream: ERROR while writing motion data!\n";
    return false;
  }
  //write TTL
  OutStream.write((char*) &m_TTL, sizeof(float));
  return OutStream.good();
}

bool Projectile::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Projectile::loadFromStream: ERROR: Cannot load from stream "
              << "while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Projectile::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefP"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefP)
  {
    DuskLog() << "Projectile::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Projectile::loadFromStream: ERROR while reading basic "
              << "data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    DuskLog() << "Projectile::loadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  //TTL
  InStream.read((char*) &m_TTL, sizeof(m_TTL));
  return InStream.good();
}

} //namespace
