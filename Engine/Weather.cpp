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
  #ifndef DUSK_EDITOR
  Ogre::SceneNode* SnowNode = getAPI().getDuskCamera()->getOgreCamera()->getParentSceneNode();
  #else
  Ogre::SceneNode* SnowNode = EditorCamera::GetSingleton().getOgreCamera()->getParentSceneNode();
  #endif
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
  #ifndef DUSK_EDITOR
  Ogre::SceneNode* RainNode = getAPI().getDuskCamera()->getOgreCamera()->getParentSceneNode();
  #else
  Ogre::SceneNode* RainNode = EditorCamera::GetSingleton().getOgreCamera()->getParentSceneNode();
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
  const Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  return (scm->hasParticleSystem(cRainParticleSystem));
}

} //namespace
