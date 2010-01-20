#include "Menu.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include "DuskFunctions.h"

namespace Dusk
{
  const unsigned int Menu::cMaxDialogueOptions = 5;
  const std::string Menu::cDialogueOverlay = "Dusk/DialogueOverlay";

Menu& Menu::GetSingleton()
{
  static Menu Instance;
  return Instance;
}

Menu::Menu()
{
  m_DialogueLineCount = 0;
}

Menu::~Menu()
{
  hideDialogue();
  killDialogueOverlayLines();
}

void Menu::showDialogue(const std::string& first, const std::vector<std::string>& options)
{
  Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
  std::cout << "getByName(overlay)\n";
  Ogre::Overlay* dialOverlay = om.getByName(cDialogueOverlay);
  if (dialOverlay == NULL)
  {
    std::cout << "create(overlay)\n";
    dialOverlay = om.create(cDialogueOverlay);
  }
  Ogre::OverlayContainer* dialCont;
  std::cout << "getOE(Box)\n";
  dialCont = static_cast<Ogre::OverlayContainer*> (om.getOverlayElement(cDialogueOverlay+"/Box"));
  if ( dialCont == NULL)
  {
    std::cout << "createOE(Box)\n";
    Ogre::OverlayContainer* dialCont = static_cast<Ogre::OverlayContainer*>
                    (om.createOverlayElement("Panel", cDialogueOverlay+"/Box"));
    dialCont->setPosition(0.0, 0.75);
    dialCont->setDimensions(1.0, 0.25);
    dialCont->setMaterialName("Dusk/Dialogue/Black");
  }
  std::cout << "getOE(Box/First)\n";
  Ogre::OverlayElement* dialElem;
  dialElem = om.getOverlayElement(cDialogueOverlay+"/Box/First");
  if (dialElem==NULL)
  {
    std::cout << "createOE(Box/First)\n";
    dialElem = om.createOverlayElement("TextArea", cDialogueOverlay+"/Box/First");
    dialElem->setDimensions(1.0, 0.0625);
    dialElem->setPosition(0.0, 0.0);
    dialElem->setMaterialName("Dusk/Dialogue/Element");
    dialCont->addChild(dialElem);
  }
  dialElem->setCaption(first);
  dialElem->show();
  unsigned int elemCount = options.size();
  if (elemCount > cMaxDialogueOptions)
  {
    elemCount = cMaxDialogueOptions;
  }

  killDialogueOverlayLines();
  unsigned int i;
  const float elemHeight = 0.8/(float)elemCount;
  for (i=0; i<elemCount; i=i+1)
  {
    //dialElem = om.createOverlayElement("TextArea", cDialogueOverlay+"/Box/Line"+IntToString(i));
    dialElem = om.createOverlayElementFromTemplate(cDialogueOverlay+"/LineTemplate", "TextArea", cDialogueOverlay+"/Box/Line"+IntToString(i));
    dialElem->setPosition(0.1, dialCont->getHeight()*(0.2+i*elemHeight));
    dialElem->setDimensions(0.8, elemHeight*dialCont->getHeight());
    dialElem->setCaption(IntToString(i+1)+": "+options[i]);
    dialElem->show();
    dialCont->addChild(dialElem);
    m_DialogueLineCount = i+1;
  }//for
  dialOverlay->show();
}

void Menu::hideDialogue()
{
  Ogre::Overlay* ol = Ogre::OverlayManager::getSingleton().getByName(cDialogueOverlay);
  if (ol!=NULL)
  {
    ol->hide();
  }
}

bool Menu::isDialogueActive() const
{
  const Ogre::Overlay* ol = Ogre::OverlayManager::getSingleton().getByName(cDialogueOverlay);
  if (ol!=NULL)
  {
    return (ol->isVisible());
  }
  return false;
}

void Menu::killDialogueOverlayLines()
{
  Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
  Ogre::OverlayContainer* oc = static_cast<Ogre::OverlayContainer*> (om.getOverlayElement(cDialogueOverlay+"/Box"));
  unsigned int i;
  for (i=0; i<m_DialogueLineCount; i=i+1)
  {
    oc->removeChild(cDialogueOverlay+"/Box/Line"+IntToString(i));
    om.destroyOverlayElement(cDialogueOverlay+"/Box/Line"+IntToString(i));
  }//for
  m_DialogueLineCount = 0;
}

} //namespace
