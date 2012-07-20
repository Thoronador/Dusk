/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012  thoronador

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

#include "Light.h"
#include "../database/LightRecord.h"
#include "../database/Database.h"
#include "../DuskConstants.h"
#include "../Messages.h"
#include <sstream>

namespace Dusk
{

Light::Light()
  : DuskObject()
{
  //constructor
  m_Direction = Ogre::Vector3::ZERO;
}

Light::Light(const std::string& ID, const Ogre::Vector3& pos, const Ogre::Vector3& dir)
  : DuskObject(ID, pos, Ogre::Quaternion::IDENTITY, 1.0f)
{
  //constructor
  m_Direction = dir;
}

bool Light::enable(Ogre::SceneManager* scm)
{
  if (entity != NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    DuskLog() << "Light::enable: ERROR: no scene manager present.\n";
    return false;
  }

  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();

  entity = scm->createLight(entity_name.str());
  const LightRecord& lr = Database::getSingleton().getTypedRecord<LightRecord>(ID);
  entity->setType(lr.type);
  entity->setPosition(Ogre::Vector3::ZERO);
  if (lr.type!=Ogre::Light::LT_POINT)
  { //only directional lights and spotlights need direction
    entity->setDirection(m_Direction);
  }
  entity->setDiffuseColour(lr.red, lr.green, lr.blue);
  entity->setSpecularColour(lr.red, lr.green, lr.blue);
  entity->setAttenuation(lr.radius, entity->getAttenuationConstant(),
              entity->getAttenuationLinear(), entity->getAttenuationQuadric());

  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  //rotation
  ent_node->setOrientation(m_Rotation);
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

bool Light::disable()
{
  if (entity == NULL)
  {
    return true;
  }
  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm = ent_node->getCreator();
  if (scm==NULL)
  {
    DuskLog() << "Light::disable: ERROR: got NULL for scene manager.\n";
    return false;
  }
  ent_node->detachObject(entity);
  scm->getRootSceneNode()->removeAndDestroyChild(ent_node->getName());
  scm->destroyLight(entity);
  entity = NULL;
  return true;
}

bool Light::isEnabled() const
{
  return (entity!=NULL);
}

ObjectTypes Light::getDuskType() const
{
  return otLight;
}

bool Light::canCollide() const
{
  return false;
}

void Light::setDirection(const Ogre::Vector3& dir)
{
  if (entity != NULL)
  {
    entity->setDirection(dir);
  }
  m_Direction = dir;
}

Ogre::Vector3 Light::getDirection() const
{
  return m_Direction;
}

bool Light::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Light::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  float xyz;

  //write header "RefL" (reference of Light)
  OutStream.write((const char*) &cHeaderRefL, sizeof(uint32_t)); //header
  //write ID
  uint32_t len = ID.length();
  OutStream.write((const char*) &len, sizeof(uint32_t));
  OutStream.write(ID.c_str(), len);

  //write position and rotation, and direction
  // -- position
  xyz = position.x;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = position.y;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = position.z;
  OutStream.write((const char*) &xyz, sizeof(float));
  // -- rotation
  xyz = m_Rotation.w;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = m_Rotation.x;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = m_Rotation.y;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = m_Rotation.z;
  OutStream.write((const char*) &xyz, sizeof(float));
  //(We don't need scale for lights, so don't write it to stream.)
  // -- direction
  xyz = m_Direction.x;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = m_Direction.y;
  OutStream.write((const char*) &xyz, sizeof(float));
  xyz = m_Direction.z;
  OutStream.write((const char*) &xyz, sizeof(float));
  return (OutStream.good());
}

bool Light::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Light::loadFromStream: ERROR: Cannot load from stream while "
              << "light is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Light::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  char ID_Buffer[256];
  float f_temp;
  uint32_t len;

  //read header "RefL"
  uint32_t Header = 0;
  InStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderRefL)
  {
    DuskLog() << "Light::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read ID
  InStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "Light::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    DuskLog() << "Light::loadFromStream: ERROR while reading data (ID).\n";
    return false;
  }
  ID = std::string(ID_Buffer);

  //position
  InStream.read((char*) &f_temp, sizeof(float));
  position.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.z = f_temp;
  //rotation
  InStream.read((char*) &f_temp, sizeof(float));
  m_Rotation.w = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Rotation.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Rotation.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Rotation.z = f_temp;
  //scale is not needed / always 1.0f
  m_Scale = 1.0f;
  //direction
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.z = f_temp;
  return (InStream.good());
}

}
