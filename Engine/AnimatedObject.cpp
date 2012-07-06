/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2009, 2010, 2012  ssj5000, thoronador

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

#include "AnimatedObject.h"
#include "ObjectRecord.h" //should possibly replace this one later
#include "Database.h"
#include <OgreAnimationState.h>
#include "DuskConstants.h"
#include "VertexDataFunc.h"
#include "Messages.h"

namespace Dusk
{

AnimRecord::AnimRecord()
{
  position = 0.0f;
  DoLoop = false;
}

AnimRecord::AnimRecord(const float pos, const bool loop)
{
  position = pos;
  DoLoop = loop;
}

AnimatedObject::AnimatedObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  m_Rotation = Ogre::Quaternion::IDENTITY;
  m_Scale = 1.0f;
  entity = NULL;
  m_Anims.clear();
}

AnimatedObject::AnimatedObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
{
  ID = _ID;
  position = pos;
  m_Rotation = rot;
  if (m_Scale>0.0f)
  {
    m_Scale = Scale;
  } else {
    m_Scale = 1.0f;
  }
  entity = NULL;

  m_Anims.clear();
}

AnimatedObject::~AnimatedObject()
{
  //dtor
  disable();
}

const std::string& AnimatedObject::getObjectMesh() const
{
  //TODO: adjust in future, if static and animated objects don't use the same
  // type of data source.
  return Database::getSingleton().getTypedRecord<ObjectRecord>(ID).Mesh;
}

bool AnimatedObject::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    DuskLog("AnimatedObject::Enable: ERROR: no scene manager present.\n");
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), getObjectMesh());
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  /*//not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));*/
  ent_node->setOrientation(m_Rotation);
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  //restore saved or queued animations
  if (!(m_Anims.empty()))
  {
    const Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    std::map<std::string, AnimRecord>::iterator iter;
    iter = m_Anims.begin();
    //iterate through animations
    while (iter!=m_Anims.end())
    {
      //check if given animation name identifies a valid animation
      if (anim_set->hasAnimationState(iter->first))
      {
        //if so, enable it
        Ogre::AnimationState* state = anim_set->getAnimationState(iter->first);
        state->setTimePosition(iter->second.position);
        state->setLoop(iter->second.DoLoop);
        state->setEnabled(true);
        //... and advance iterator
        ++iter;
      }
      else
      {
        //otherwise delete it from list
        const std::string currentName = iter->first;
        m_Anims.erase(iter);
        //... and set iter to next element
        iter = m_Anims.upper_bound(currentName);
      }
    }//while
  }//animations queued
  return (entity!=NULL);
}

bool AnimatedObject::disable()
{
  if (entity == NULL)
  {
    return true;
  }
  //synchro list
  synchronizeAnimationList();
  //disable it
  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm;
  scm = ent_node->getCreator();
  if (scm==NULL)
  {
    DuskLog("AnimatedObject::Disable: ERROR: got NULL for scene manager.\n");
    return false;
  }
  ent_node->detachObject(entity);
  scm->getRootSceneNode()->removeAndDestroyChild(ent_node->getName());
  scm->destroyEntity(entity);
  entity = NULL;
  return true;
}

ObjectTypes AnimatedObject::getDuskType() const
{
  return otAnimated;
}

bool AnimatedObject::canCollide() const
{
  //TODO: adjust in future, if static and animated objects don't use the same
  // data source.
  return Database::getSingleton().getTypedRecord<ObjectRecord>(ID).collide;
}

bool AnimatedObject::isHitByRay(const Ogre::Ray& ray, Ogre::Vector3& impact) const
{
  /* Basically, this function has the same code that DuskObject::isHitByRay()
     uses. The main difference is the use of GetMeshInformationAnimated() to
     retrieve the vertex data for animated entities instead of the function
     GetMeshInformation() for static entities.
  */
  //if object is not enabled, it can not be hit by a ray
  if (!isEnabled()) return false;
  //perform bounding box check first, because it's less expensive and faster
  // than a full ray-to-polygon check
  if (!(ray.intersects(entity->getWorldBoundingBox()).first)) return false;

  /* The rest of this function's code is taken from
       http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Raycasting%20to%20the%20polygon%20level
     with some minor adjustments.
  */
  // mesh data to retrieve
  size_t vertex_count;
  size_t index_count;
  Ogre::Vector3 *vertices;
  unsigned long *indices;
  // get the mesh information
  GetMeshInformationAnimated(entity, vertex_count, vertices, index_count,
                             indices,
  #if defined(OGRE_VERSION_MAJOR) && defined(OGRE_VERSION_MINOR)
    /* With Ogre "Shoggoth" 1.6 the functions getWorldPosition() and
       getWorldOrientation() were removed from Ogre::Node, so we have to use
       _getDerivedPosition() and _getDerivedOrientation() instead.
    */
    #if ((OGRE_VERSION_MAJOR>1) || (OGRE_VERSION_MAJOR==1&& OGRE_VERSION_MINOR>=6))
       //Code for Ogre "Shoggoth" 1.6 and later
                             entity->getParentNode()->_getDerivedPosition(),
                             entity->getParentNode()->_getDerivedOrientation(),
    #else
       //Code for earlier Ogre Versions, e.g. Ogre "Eihort" 1.4
                             entity->getParentNode()->getWorldPosition(),
                             entity->getParentNode()->getWorldOrientation(),
    #endif
  #else
    #error OGRE_VERSION_MAJOR and OGRE_VERSION_MINOR are not defined!
    #error Are you sure you the Ogre headers are included?
  #endif
                             entity->getParentNode()->_getDerivedScale());
  // test for hitting individual triangles on the mesh
  Ogre::Real closest_distance = -1.0f;
  std::pair<bool, Ogre::Real> hit;
  unsigned int i;
  for (i=0; i<index_count; i=i+3)
  {
    // check for a hit against this triangle
    hit = Ogre::Math::intersects(ray, vertices[indices[i]], vertices[indices[i+1]],
                                 vertices[indices[i+2]], true, false);
    // Was it hit?
    if (hit.first)
    {
      //Is distance closer than previous closest distance?
      if ((hit.second<closest_distance) || (closest_distance<0.0f))
      {
        // closest intersect so far, save it
        closest_distance = hit.second;
      }//if new closest distance
    }//if hit
  }//for
  // free the verticies and indicies memory
  delete[] vertices;
  delete[] indices;
  //return values
  if (closest_distance>-0.5f)
  {
    impact = ray.getPoint(closest_distance);
    return true;
  }
  return false;
}

bool AnimatedObject::startAnimation(const std::string& AnimName, const bool DoLoop)
{
  if (AnimName=="")
    return false;
  if (entity!=NULL)
  {
    const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
    if (anim_set!=NULL)
    {
      if (!anim_set->hasAnimationState(AnimName))
      {
        DuskLog() << "AnimatedObject::EnableAnimation: Error: mesh has no "
                  << "animation named \"" <<AnimName<<"\"!\n";
        return false;
      }
      Ogre::AnimationState* state = entity->getAnimationState(AnimName);
      state->setTimePosition(0.0f);
      state->setLoop(DoLoop);
      state->setEnabled(true);
      m_Anims[AnimName] = AnimRecord(0.0f, DoLoop);
      return true;
    }//animation set given
  }
  return false;
}

bool AnimatedObject::stopAnimation(const std::string& AnimName)
{
  if (entity==NULL)  // no entitiy -> no animation, so basically
    return true;     //   the animation is already "stopped" :P
  if (AnimName=="")
    return false;

  const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
  if (anim_set!=NULL)
  {
    if (!anim_set->hasAnimationState(AnimName))
    {
      DuskLog() <<"AnimatedObject::StopAnimation: Error: mesh has no "
                << "animation named \"" <<AnimName<<"\"!\n";
      return false;
    }
    Ogre::AnimationState* state = entity->getAnimationState(AnimName);
    state->setTimePosition(0.0f);
    state->setEnabled(false);
    m_Anims.erase(AnimName);
    return true;
  }//animation set given
  return false;
}

unsigned int AnimatedObject::stopAllAnimations()
{
  if (entity==NULL)  // no entitiy -> no animation, so basically
    return 0;        // all animations are already "stopped" :P
  const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
  if (NULL==anim_set)
  {
    return 0;
  }
  unsigned int result = 0;
  Ogre::ConstEnabledAnimationStateIterator easIter = anim_set->getEnabledAnimationStateIterator();
  Ogre::AnimationState* as = NULL;
  while (easIter.hasMoreElements())
  {
    as = easIter.peekNext();
    as->setEnabled(false);
    ++result;
    /* ****
       ** We have to get the iterator again at the end of this loop, because
       ** setting enabled to false (as in the line above) will change the list
       ** of enabled animation states and invalidate(!) the iterator. Thus we
       ** cannot just advance the iterator.
       ****
    */
    easIter = anim_set->getEnabledAnimationStateIterator();
  }//while
  m_Anims.clear();
  return result;
}

bool AnimatedObject::isAnimationActive(const std::string& AnimName) const
{
  if (entity==NULL)  // no entitiy -> no animation, so basically
    return false;     //   the animation is already "stopped" :P
  if (AnimName=="")
    return false;
  const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
  if (anim_set!=NULL)
  {
    if (!anim_set->hasAnimationState(AnimName))
    {
      DuskLog() << "AnimatedObject::IsAnimationActive: Error: mesh has no "
                << "animation named \"" <<AnimName<<"\"!\n";
      return false;
    }
    return entity->getAnimationState(AnimName)->getEnabled();
  }//animation set given
  return false;
}

std::vector<std::string> AnimatedObject::getCurrentAnimations() const
{
  std::vector<std::string> tempList;
  tempList.clear();
  //if object is not enabled, get list from m_Anims
  if (!isEnabled())
  {
    std::map<std::string, AnimRecord>::const_iterator cIter = m_Anims.begin();
    while (cIter!=m_Anims.end())
    {
      tempList.push_back(cIter->first);
      ++cIter;
    }//while
    return tempList;
  }//not enabled
  //if object is enabled, get list from entity's animation states
  const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
  if (anim_set==NULL) return tempList;
  Ogre::AnimationState* as = NULL;
  Ogre::ConstEnabledAnimationStateIterator asIter = anim_set->getEnabledAnimationStateIterator();
  while (asIter.hasMoreElements())
  {
    as = asIter.peekNext();
    tempList.push_back(as->getAnimationName());
    asIter.moveNext();
  }//while
  return tempList;
}

std::vector<std::string> AnimatedObject::getPossibleAnimationStates() const
{
  if (entity==NULL)
  {
    return std::vector<std::string>();
  }
  const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
  if (NULL==anim_set)
  {
    return std::vector<std::string>();
  }
  Ogre::ConstAnimationStateIterator cIter = anim_set->getAnimationStateIterator();
  std::vector<std::string> result;
  while (cIter.hasMoreElements())
  {
    result.push_back(cIter.peekNextKey());
    cIter.moveNext();
  } //while
  return result;
}

bool AnimatedObject::getLoopState(const std::string& AnimName) const
{
  if (isEnabled())
  {
    const Ogre::AnimationStateSet * anim_set = entity->getAllAnimationStates();
    if (anim_set==NULL) return false;
    const Ogre::AnimationState* as = anim_set->getAnimationState(AnimName);
    if (as!=NULL) return as->getLoop();
    return false;
  }//if is enabled;
  //if not enabled, then search in map
  std::map<std::string, AnimRecord>::const_iterator cIter = m_Anims.find(AnimName);
  if (cIter!=m_Anims.end()) return cIter->second.DoLoop;
  return false;
}

void AnimatedObject::injectTime(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  //adjust animation states
  if (entity!=NULL)
  {
    const Ogre::AnimationStateSet* animSet = entity->getAllAnimationStates();
    if (animSet!=NULL)
    {
      Ogre::ConstEnabledAnimationStateIterator easIter = animSet->getEnabledAnimationStateIterator();
      while (easIter.hasMoreElements())
      {
        Ogre::AnimationState* as = easIter.getNext();
        as->addTime(SecondsPassed);
        m_Anims[as->getAnimationName()].position = as->getTimePosition();
      }//while
    }// anim set present
  }//if
}

void AnimatedObject::synchronizeAnimationList()
{
  if (!isEnabled()) return;
  //clear list
  m_Anims.clear();
  //rebuilt list with current data
  const Ogre::AnimationStateSet* animSet = entity->getAllAnimationStates();
  if (animSet!=NULL)
  {
    Ogre::ConstEnabledAnimationStateIterator easIter = animSet->getEnabledAnimationStateIterator();
    AnimRecord a_rec;
    while (easIter.hasMoreElements())
    {
      Ogre::AnimationState* as = easIter.getNext();
      a_rec.position = as->getTimePosition();
      a_rec.DoLoop = as->getLoop();
      m_Anims[as->getAnimationName()] = a_rec;
    }//while
  }// anim set present
}

bool AnimatedObject::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "AnimatedObject::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefA" (reference of AnimatedObject)
  OutStream.write((char*) &cHeaderRefA, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "AnimatedObject::saveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  return saveAnimatedObjectPart(OutStream);
}

bool AnimatedObject::saveAnimatedObjectPart(std::ofstream& OutStream) const
{
  // save new data members from AnimatedObject
  // -- save length
  unsigned int len = m_Anims.size();
  OutStream.write((char*) &len, sizeof(len));
  // -- save all animations
  std::map<std::string, AnimRecord>::const_iterator cIter = m_Anims.begin();
  while (cIter != m_Anims.end())
  {
    // -- anim name
    len = cIter->first.length();
    OutStream.write((char*) &len, sizeof(len));
    OutStream.write(cIter->first.c_str(), len);
    // -- position
    OutStream.write((char*) &(cIter->second.position), sizeof(float));
    // -- loop mode
    OutStream.write((char*) &(cIter->second.DoLoop), sizeof(bool));
    ++cIter;
  }//while
  return OutStream.good();
}

bool AnimatedObject::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "AnimatedObject::loadFromStream: ERROR: Cannot load from "
              << "stream while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "AnimatedObject::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefA"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefA)
  {
    DuskLog() << "AnimatedObject::loadFromStream: ERROR: Stream contains "
              << "invalid reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "AnimatedObject::loadFromStream: ERROR while reading basic "
              << "object data.\n";
    return false;
  }//if
  // go on with new data members from AnimatedObject
  return loadAnimatedObjectPart(InStream);
}

bool AnimatedObject::loadAnimatedObjectPart(std::ifstream& InStream)
{
  //load data members from AnimatedObject
  //animation data
  unsigned int count = 0;
  //number of animations
  InStream.read((char*) &count, sizeof(unsigned int));
  if (count>100)
  {
    DuskLog() << "AnimatedObject::loadAnimatedObjectPart: ERROR: object seems "
              << "to have more than 100 active animations. Aborted.\n";
    return false;
  }
  //clear animation list
  m_Anims.clear();
  char ID_Buffer[256];
  AnimRecord tempRec;
  unsigned int i, len;
  for (i=0; i<count; ++i)
  {
    // -- length of animation name
    InStream.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      DuskLog() << "AnimatedObject::loadAnimatedObjectPart: ERROR: animation "
                << "name cannot be longer than 255 characters.\n";
      return false;
    }
    InStream.read(ID_Buffer, len);
    ID_Buffer[len] = '\0';
    if (!InStream.good())
    {
      DuskLog() << "AnimatedObject::loadAnimatedObjectPart: ERROR while reading "
                << "animation name.\n";
      return false;
    }
    // -- position
    InStream.read((char*) &(tempRec.position), sizeof(float));
    // -- loop mode
    InStream.read((char*) &(tempRec.DoLoop), sizeof(bool));
    if (!InStream.good())
    {
      DuskLog() << "AnimatedObject::loadAnimatedObjectPart: ERROR while reading"
                << " animation data.\n";
      return false;
    }
    m_Anims[std::string(ID_Buffer)] = tempRec;
  }//for
  return (InStream.good());
}

} //namespace
