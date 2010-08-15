#include "Menu.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include "DuskFunctions.h"
#include "Dialogue.h"

namespace Dusk
{
  const unsigned int Menu::cMaxDialogueOptions = 5;
  /* Don't change this, or you'll have to edit the overlay script, too. */
  const std::string Menu::cDialogueOverlay = "Dusk/DialogueOverlay";

Menu& Menu::GetSingleton()
{
  static Menu Instance;
  return Instance;
}

Menu::Menu()
{
  m_DialogueLineCount = 0;
  m_OptionIDs.clear();
  m_DialoguePartner = NULL;
}

Menu::~Menu()
{
  hideDialogue();
  killDialogueOverlayLines();
  m_OptionIDs.clear();
  m_DialoguePartner = NULL;
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
  Ogre::OverlayManager* olMgr =Ogre::OverlayManager::getSingletonPtr();
  /*If overlay manager is NULL, then it is not present any more and all overlays
    should already be destroyed at that point. So we can return anyway. */
  if (olMgr==NULL) return;
  Ogre::Overlay* ol = olMgr->getByName(cDialogueOverlay);
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
  Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();
  /*If overlay manager is NULL, then it is not present any more and all overlays
    should already be destroyed at that point. So we can return anyway. */
  if (om==NULL) return;
  Ogre::OverlayContainer* oc = static_cast<Ogre::OverlayContainer*> (om->getOverlayElement(cDialogueOverlay+"/Box"));
  unsigned int i;
  for (i=0; i<m_DialogueLineCount; i=i+1)
  {
    oc->removeChild(cDialogueOverlay+"/Box/Line"+IntToString(i));
    om->destroyOverlayElement(cDialogueOverlay+"/Box/Line"+IntToString(i));
  }//for
  m_DialogueLineCount = 0;
}

bool Menu::startDialogueWithNPC(NPC* who)
{
  //we don't want to interrupt ongoing conversations
  if (isDialogueActive()) return false;

  Dialogue::Handle temp = Dialogue::GetSingleton().GetGreetingLine(who);
  if (temp.LineID == "") return false; //nothing found here

  //get text of all dialogue line options
  std::vector<std::string> sv;
  sv.clear();
  m_OptionIDs.clear();
  unsigned int i;
  for (i=0; i<temp.Choices.size(); i=i+1)
  {
    sv.push_back(Dialogue::GetSingleton().GetText(temp.Choices[i]));
    m_OptionIDs.push_back(temp.Choices[i]);
  }//for
  if (temp.Choices.size()==0)
  {
    sv.push_back("[End of Dialogue]");
    m_OptionIDs.push_back("");
  }
  showDialogue(temp.Text, sv);
  Dialogue::GetSingleton().ProcessResultScript(temp.LineID);
  m_DialoguePartner = who;
  return true;
}

bool Menu::nextDialogueChoice(const unsigned int chosenOption)
{
  //not completely implemented yet
  if (!isDialogueActive())
  {
    std::cout << "Menu::nextDialogueChoice: Hint: no active dialogue.\n";
    return false;
  }

  //zero means that player wants to quit dialogue
  if (chosenOption == 0)
  {
    hideDialogue();
    m_OptionIDs.clear();
    m_DialoguePartner = NULL;
    return true;
  }
  //end of dialogue met?
  if (chosenOption == 1 and m_OptionIDs.size()>0)
  {
    if (m_OptionIDs[0]=="")
    {
      hideDialogue();
      m_OptionIDs.clear();
      m_DialoguePartner = NULL;
      return true;
    }
  }
  //is option within valid range?
  if (chosenOption>m_OptionIDs.size())
  {
    std::cout << "Menu::nextDialogueChoice: Hint: choice is out of range, thus"
              << " it will be ignored.\n";
    return false;
  }

  Dialogue::Handle tempHandle = Dialogue::GetSingleton().GetDialogueLine(
                                  m_OptionIDs[chosenOption-1], m_DialoguePartner);
  Dialogue::GetSingleton().ProcessResultScript(m_OptionIDs[chosenOption-1]);
  if (tempHandle.Choices.size()==0)
  {
    //no more lines here... quit dialogue
    hideDialogue();
    m_OptionIDs.clear();
    m_DialoguePartner = NULL;
    return true;
  }
  //we have more choices, NPC will always select the first one available
  tempHandle = Dialogue::GetSingleton().GetDialogueLine(tempHandle.Choices[0],
                                                          m_DialoguePartner);
  m_OptionIDs.clear();
  //get text of all options
  std::vector<std::string> sv;
  sv.clear();
  unsigned int i;
  for (i=0; i<tempHandle.Choices.size(); i=i+1)
  {
    sv.push_back(Dialogue::GetSingleton().GetText(tempHandle.Choices[i]));
    m_OptionIDs.push_back(tempHandle.Choices[i]);
  }//for
  if (tempHandle.Choices.size()==0)
  {
    sv.push_back("[End of Dialogue]");
    m_OptionIDs.push_back("");
  }
  showDialogue(tempHandle.Text, sv);
  Dialogue::GetSingleton().ProcessResultScript(tempHandle.LineID);
  return true;
}

} //namespace
