/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "Projectile.h"
#include "ProjectileBase.h"
#include "InjectionManager.h"
#include "DuskConstants.h"
#include "Landscape.h"
#include "DiceBox.h"

namespace Dusk
{

const float Projectile::InfiniteTTL = -1.0f;

Projectile::Projectile()
  : UniformMotionObject()
{
  m_TTL = InfiniteTTL;
  m_Emitter = NULL;
}

Projectile::Projectile(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : UniformMotionObject(_ID, pos, rot, Scale)
{
  if (ProjectileBase::getSingleton().hasProjectile(_ID))
  {
    const ProjectileRecord rec = ProjectileBase::getSingleton().getProjectileData(_ID);
    m_TTL = rec.DefaultTTL;
    m_Speed = rec.DefaultVelocity;
  }
  else
  {
    m_TTL = InfiniteTTL;
  }
  m_Emitter = NULL;
}

Projectile::~Projectile()
{
  disable();
}

ObjectTypes Projectile::getDuskType() const
{
  return otProjectile;
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

    for (i=0; i<result.size(); ++i)
    {
      if (result.at(i).distance>SecondsPassed*m_Speed) break;
      if (result.at(i).movable!=NULL and result.at(i).movable!=entity)
      {
        //is it a landscape record?
        if (LandscapeRecord::isLandscapeRecordName(result.at(i).movable->getName()))
        {
          Ogre::Vector3 vec_i = ray.getPoint(result.at(i).distance);
          const LandscapeRecord* land_rec = Landscape::getSingleton().getRecordAtXZ(vec_i.x, vec_i.z);
          if (land_rec!=NULL)
          {
            if (land_rec->isHitByRay(ray, vec_i))
            {
              if (vec_i.squaredDistance(position)<=Ogre::Math::Sqr(SecondsPassed*m_Speed))
              {
                //projectile will hit the landscape within this frame
                // --> request deletetion of projectile
                InjectionManager::getSingleton().requestDeletion(this);
                return;
              }
            }//If hit by ray
          }//if
        }//landscape record
        else
        { //no landscape, so it must be a DuskObject (or derived type)
          DuskObject* obj = static_cast<DuskObject*>(result.at(i).movable->getUserObject());
          if (obj!=NULL and obj!=m_Emitter)
          {
            //hit a static object (or item or weapon)?
            if ((obj->getDuskType()==otStatic or obj->getDuskType()==otAnimated
                 or obj->getDuskType()==otWaypoint or obj->getDuskType()==otContainer)
               or ((obj->getDuskType()==otItem or obj->getDuskType()==otWeapon)
                   and !(static_cast<Item*>(obj))->isEquipped()))
            {
              Ogre::Vector3 vec_i(0.0, 0.0, 0.0);
              if (obj->isHitByRay(ray, vec_i))
              {
                if (vec_i.squaredDistance(position)<=Ogre::Math::Sqr(SecondsPassed*m_Speed))
                {
                  //projectile will hit the static object within this frame
                  // --> request projectile deletetion
                  InjectionManager::getSingleton().requestDeletion(this);
                  return;
                }//if distance shot enough
              }//hit?
            }//static?
            else if (obj->getDuskType()==otNPC and obj!=m_Emitter)
            {
              Ogre::Vector3 vec_i(0.0, 0.0, 0.0);
              if (obj->isHitByRay(ray, vec_i))
              {
                if (vec_i.squaredDistance(position)<=Ogre::Math::Sqr(SecondsPassed*m_Speed))
                {
                  //projectile will hit the NPC within this frame
                  // --> inflict damage
                  Dusk::NPC* npc_ptr = dynamic_cast<NPC*> (obj);
                  const ProjectileRecord pr = ProjectileBase::getSingleton().getProjectileData(this->ID);
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
                         std::cout << "Projectile::injectTime: ERROR: projectile \""
                                   << ID << "\" has invalid die number ("<<pr.dice
                                   <<").\n";
                         break;
                  }//switch
                  // --> request projectile deletetion
                  InjectionManager::getSingleton().requestDeletion(this);
                  return;
                }//if distance shot enough
              }//hit?
            }//NPC?
          }//not NULL and not equal to emitter
        }//else branch
      }//if movable != NULL
    }//for i
    scm->destroyQuery(rsq);
    rsq = NULL;
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

std::string Projectile::getObjectMesh() const
{
  return ProjectileBase::getSingleton().getProjectileMesh(ID);
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
    std::cout << "Projectile::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefP" (reference of Projectile)
  OutStream.write((char*) &cHeaderRefP, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    std::cout << "Projectile::SaveToStream: ERROR while writing basic data!\n";
    return false;
  }
  //write all data inherited from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    std::cout << "Projectile::SaveToStream: ERROR while writing motion data!\n";
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
    std::cout << "Projectile::LoadFromStream: ERROR: Cannot load from stream "
              << "while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Projectile::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefP"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefP)
  {
    std::cout << "Projectile::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    std::cout << "Projectile::LoadFromStream: ERROR while reading basic "
              << "data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    std::cout << "Projectile::LoadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  //TTL
  InStream.read((char*) &m_TTL, sizeof(m_TTL));
  return InStream.good();
}

} //namespace
