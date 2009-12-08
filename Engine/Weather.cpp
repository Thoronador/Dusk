#include "Weather.h"
#include <OgreSceneManager.h>
#include "API.h"

namespace Dusk
{

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

} //namespace
