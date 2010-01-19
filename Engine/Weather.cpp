#include "Weather.h"
#include <OgreSceneManager.h>
#include "API.h"
#include "Camera.h"

namespace Dusk
{

const std::string Weather::cSnowParticleSystem = "Dusk/SnowPS";
const std::string Weather::cRainParticleSystem = "Dusk/RainPS";

Weather::Weather()
{
  m_Fog_r = m_Fog_g = m_Fog_b = 1.0f;
  m_orig_bgcolour = getAPI().getOgreRenderWindow()->getViewport(0)->getBackgroundColour();
}

Weather::~Weather()
{
  if (isFoggy())
  {
    stopFog();
  }
  stopSnow();
  stopRain();
}

Weather& Weather::getSingelton()
{
  static Weather Instance;
  return Instance;
}

void Weather::setFogColour(const float r, const float g, const float b)
{
  if (isFoggy())
  {
    Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
    scm->setFog(scm->getFogMode(), Ogre::ColourValue(r,g,b), scm->getFogDensity(),
                scm->getFogStart(), scm->getFogEnd());
    getAPI().getOgreRenderWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue(r,g,b));
  }
  m_Fog_r = r;
  m_Fog_g = g;
  m_Fog_b = b;
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
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  return (scm->getFogMode() != Ogre::FOG_NONE);
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
  Ogre::SceneNode* SnowNode = getAPI().getDuskCamera()->getOgreCamera()->getParentSceneNode();
  SnowNode = SnowNode->createChildSceneNode(cSnowParticleSystem+"_Node");
  SnowNode->attachObject(SnowSys);
}

void Weather::stopSnow()
{
  if (!isSnowing()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  Ogre::SceneNode* SnowNode = scm->getSceneNode(cSnowParticleSystem+"_Node");
  SnowNode->detachObject(cSnowParticleSystem);
  scm->destroyParticleSystem(cSnowParticleSystem);
  SnowNode->getParentSceneNode()->removeChild(SnowNode);
  scm->destroySceneNode(SnowNode->getName());
}

bool Weather::isSnowing() const
{
  const Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  return (scm->hasParticleSystem(cSnowParticleSystem));
}

void Weather::startRain()
{
  if (isRaining()) return;
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  //create new particle system for rain
  Ogre::ParticleSystem* RainSys = scm->createParticleSystem(cRainParticleSystem, "Dusk/Rain");
  /* The general idea is to attach the particle system to the same SceneNode as
     the camera, so the particle systems moves around with the player/ camera. */
  Ogre::SceneNode* RainNode = getAPI().getDuskCamera()->getOgreCamera()->getParentSceneNode();
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
  const Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  return (scm->hasParticleSystem(cRainParticleSystem));
}

} //namespace
