#include "Sun.h"
#include "API.h"
#include <iostream>
#include <string>
#include <OgreParticle.h>
#include <OgreVector3.h>

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
    m_BBSet->getParentSceneNode()->resetOrientation();
    m_BBSet->getParentSceneNode()->rotate(Ogre::Vector3::NEGATIVE_UNIT_Z,
                               Ogre::Degree(180.0f*getSpanRatio(currentTime)));
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
