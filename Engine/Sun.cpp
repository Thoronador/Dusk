#include "Sun.h"
#include "API.h"
#include <iostream>
#include <string>
#include <OgreParticle.h>
#include <OgreVector3.h>
#include <OgreMath.h>

namespace Dusk
{

const std::string cSunBillboardSet = "Dusk/SunBBS";
const float cSunDistance = 400;

Sun::Sun()
{
  m_Start = 8.0f;
  m_End = 20.0f;
  m_BBSet = NULL;
  m_Light = NULL;
}

Sun::~Sun()
{
  hide();
}

void Sun::show()
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "Sun::show: ERROR: Got NULL for SceneManager.\n";
    return;
  }
  if (isVisible())
  {
    std::cout << "Sun::show: Hint: System is already visible. Skipping.\n";
    return;
  }
  m_BBSet = scm->createBillboardSet(cSunBillboardSet);
  m_BBSet->setMaterialName("Dusk/flare");
  const Ogre::ColourValue sunColour = Ogre::ColourValue(1.0f, 1.0f, 0.0f);
  m_BBSet->createBillboard(cSunDistance, 0, 0, sunColour);
  Ogre::SceneNode* node = scm->createSceneNode(cSunBillboardSet+"/Node");
  node->attachObject(m_BBSet);
  m_Light = scm->createLight(cSunBillboardSet+"/Light");
  m_Light->setType(Ogre::Light::LT_POINT);
  m_Light->setPosition(Ogre::Vector3(cSunDistance, 0.0f, 0.0f));
  m_Light->setDiffuseColour(sunColour);
  m_Light->setSpecularColour(sunColour);
  node->attachObject(m_Light);
  scm->getRootSceneNode()->addChild(node);
}

void Sun::hide()
{
  if (!isVisible())
  {
    std::cout << "Sun::hide: Hint: System is already not visible. Skipping.\n";
    return;
  }
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "Sun::hide: ERROR: Got NULL for SceneManager.\n";
    return;
  }
  Ogre::SceneNode* node = m_BBSet->getParentSceneNode();
  node->getParentSceneNode()->removeChild(node);
  node->detachObject(m_BBSet);
  node->detachObject(m_Light);
  scm->destroySceneNode(node->getName());
  node = NULL;
  scm->destroyLight(m_Light);
  m_Light = NULL;
  scm->destroyBillboardSet(m_BBSet);
  m_BBSet = NULL;
}

bool Sun::isVisible() const
{
  return m_BBSet!=NULL;
}

void Sun::updateTime(const float currentTime)
{
  if (isVisible())
  {
    if (!isInSpan(currentTime))
    {
      hide();
      return;
    }
    const float spr = getSpanRatio(currentTime);
    const Ogre::Vector3 vec3 = Ogre::Vector3(cSunDistance*Ogre::Math::Cos(Ogre::Math::PI*spr),
                               cSunDistance*Ogre::Math::Sin(Ogre::Math::PI*spr),
                               0.0f);
    m_BBSet->getBillboard(0)->setPosition(vec3);
    m_Light->setPosition(vec3);
  }//if
  else
  {
    if (isInSpan(currentTime))
    {
      show();
      return;
    }
  }
}

} //namespace
