/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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

#include "Weather.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreParticleSystem.h>
#include "API.h"
#ifdef DUSK_EDITOR
  #include "../Editor/EditorCamera.h"
#else
  #include "Camera.h"
#endif
#include "Sun.h"

namespace Dusk
{

const std::string Weather::cSnowParticleSystem = "Dusk/SnowPS";
const std::string Weather::cRainParticleSystem = "Dusk/RainPS";

Weather::Weather()
{
  m_Fog_r = m_Fog_g = m_Fog_b = 1.0f;
  if (getAPI().getOgreRenderWindow()!=NULL)
  {
    m_orig_bgcolour = getAPI().getOgreRenderWindow()->getViewport(0)->getBackgroundColour();
  }
  else
  {
    //Just assume black as background colour. Should work for now.
    m_orig_bgcolour = Ogre::ColourValue(0.0f, 0.0f, 0.0f);
  }
  m_Daytime = 7.98f;
  m_CelestialList.clear();
  Celestial* sun = new Sun();
  m_CelestialList.push_back(sun);
  sun->show();
  sun->updateTime(m_Daytime);
}

Weather::~Weather()
{
  if (isFoggy())
  {
    stopFog();
  }
  stopSnow();
  stopRain();
  deleteAllCelestials();
}

Weather& Weather::getSingelton()
{
  static Weather Instance;
  return Instance;
}

void Weather::setFogColour(const float r, const float g, const float b)
{
  Ogre::ColourValue colVal(r,g,b);
  colVal.saturate();  //clamps values to [0;1]
  if (isFoggy())
  {
    Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
    scm->setFog(scm->getFogMode(), colVal, scm->getFogDensity(),
                scm->getFogStart(), scm->getFogEnd());
    getAPI().getOgreRenderWindow()->getViewport(0)->setBackgroundColour(colVal);
  }
  m_Fog_r = colVal.r;
  m_Fog_g = colVal.g;
  m_Fog_b = colVal.b;
}

void Weather::getFogColour(float* r, float* g, float* b) const
{
  if (r!=NULL)
  {
    *r = m_Fog_r;
  }
  if (g!=NULL)
  {
    *g = m_Fog_g;
  }
  if (b!=NULL)
  {
    *b = m_Fog_b;
  }
}

void Weather::startLinearFog(const float start_distance, const float max_distance)
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  scm->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(m_Fog_r, m_Fog_g, m_Fog_b),
              0.0, start_distance, max_distance);
  getAPI().getOgreRenderWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue(m_Fog_r, m_Fog_g, m_Fog_b));
}

void Weather::startExponentialFog(const float density)
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  scm->setFog(scm->getFogMode(), Ogre::ColourValue(m_Fog_r, m_Fog_g, m_Fog_b), density);
  getAPI().getOgreRenderWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue(m_Fog_r, m_Fog_g, m_Fog_b));
}

void Weather::stopFog()
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  scm->setFog(Ogre::FOG_NONE);
  getAPI().getOgreRenderWindow()->getViewport(0)->setBackgroundColour(m_orig_bgcolour);
}

bool Weather::isFoggy() const
{
  Ogre::SceneManager* scm = NULL;
  if (Ogre::Root::getSingletonPtr()!=NULL)
  {
    scm = Ogre::Root::getSingleton().getSceneManagerIterator().getNext();
  }
  if (scm!=NULL)
  {
    return (scm->getFogMode() != Ogre::FOG_NONE);
  }//if
  return false;
}

bool Weather::isLinearFog() const
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  return (scm->getFogMode() == Ogre::FOG_LINEAR);
}

void Weather::startSnow()
{
  if (isSnowing()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  //create new particle system for snow
  Ogre::ParticleSystem* SnowSys = scm->createParticleSystem(cSnowParticleSystem, "Dusk/Snow");
  /* The general idea is to attach the particle system to the same SceneNode as
     the camera, so the particle systems moves around with the player/ camera. */
  #ifndef DUSK_EDITOR
  Ogre::SceneNode* SnowNode = Camera::getSingleton().getOgreCamera()->getParentSceneNode();
  #else
  Ogre::SceneNode* SnowNode = EditorCamera::getSingleton().getOgreCamera()->getParentSceneNode();
  #endif
  SnowNode = SnowNode->createChildSceneNode(cSnowParticleSystem+"_Node");
  SnowNode->attachObject(SnowSys);
}

void Weather::stopSnow()
{
  if (!isSnowing()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm!=NULL)
  {
    Ogre::SceneNode* SnowNode = scm->getSceneNode(cSnowParticleSystem+"_Node");
    SnowNode->detachObject(cSnowParticleSystem);
    scm->destroyParticleSystem(cSnowParticleSystem);
    SnowNode->getParentSceneNode()->removeChild(SnowNode);
    scm->destroySceneNode(SnowNode->getName());
  }
  else
  {
    std::cout << "Warning: got NULL for Scene Manager in Weather::stopSnow().\n";
  }
}

bool Weather::isSnowing() const
{
  Ogre::SceneManager* scm = NULL;
  if (Ogre::Root::getSingletonPtr()!=NULL)
  {
    scm = Ogre::Root::getSingleton().getSceneManagerIterator().getNext();
  }
  return ((scm!=NULL) and (scm->hasParticleSystem(cSnowParticleSystem)));
}

void Weather::startRain()
{
  if (isRaining()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  //create new particle system for rain
  Ogre::ParticleSystem* RainSys = scm->createParticleSystem(cRainParticleSystem, "Dusk/Rain");
  /* The general idea is to attach the particle system to the same SceneNode as
     the camera, so the particle systems moves around with the player/ camera. */
  #ifndef DUSK_EDITOR
  Ogre::SceneNode* RainNode = Camera::getSingleton().getOgreCamera()->getParentSceneNode();
  #else
  Ogre::SceneNode* RainNode = EditorCamera::getSingleton().getOgreCamera()->getParentSceneNode();
  #endif
  RainNode = RainNode->createChildSceneNode(cRainParticleSystem+"_Node");
  RainNode->attachObject(RainSys);
}

void Weather::stopRain()
{
  if (!isRaining()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  Ogre::SceneNode* RainNode = scm->getSceneNode(cRainParticleSystem+"_Node");
  RainNode->detachObject(cRainParticleSystem);
  scm->destroyParticleSystem(cRainParticleSystem);
  RainNode->getParentSceneNode()->removeChild(RainNode);
  scm->destroySceneNode(RainNode->getName());
}

bool Weather::isRaining() const
{
  Ogre::SceneManager* scm = NULL;
  if (Ogre::Root::getSingletonPtr()!=NULL)
  {
    scm = Ogre::Root::getSingleton().getSceneManagerIterator().getNext();
  }
  return (scm!=NULL and scm->hasParticleSystem(cRainParticleSystem));
}

float Weather::getDaytime() const
{
  return m_Daytime;
}

void Weather::addDaytime(const float seconds_passed)
{
  m_Daytime = m_Daytime +(seconds_passed/3600.0f);
  if (m_Daytime>=24.0f) m_Daytime-= 24.0f;
  else if (m_Daytime<0.0f) m_Daytime+= 24.0f;
  //update objects
  size_t i;
  for (i=0; i<m_CelestialList.size(); ++i)
  {
    if (m_CelestialList.at(i)!=NULL)
      m_CelestialList[i]->updateTime(m_Daytime);
  } //for
}

void Weather::deleteAllCelestials()
{
  //delete sun, moon, star,...
  Celestial* Nerevar_MoonAndStar;
  while (!m_CelestialList.empty())
  {
    Nerevar_MoonAndStar = m_CelestialList.back();
    m_CelestialList.pop_back();
    delete Nerevar_MoonAndStar;
  }//while
}

} //namespace
