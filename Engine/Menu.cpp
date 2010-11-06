/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "Menu.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include "DuskFunctions.h"
#include "Dialogue.h"
#include "QuestLog.h"
#include "Journal.h"

namespace Dusk
{
  const unsigned int Menu::cMaxDialogueOptions = 5;
  /* Don't change this, or you'll have to edit the overlay script, too. */
  const std::string Menu::cDialogueOverlay = "Dusk/DialogueOverlay";

  //name of overlay for quest log
  const std::string cQuestLogOverlay = "Dusk/QuestLogOL";

  //number of quest log entries that are shown on one page
  const unsigned int cQuestLogEntriesPerPage = 9;
  //height of a quest log entrie
  const float cQuestLogEntryHeight = 0.1f;
  //height of characters
  const float cQuestLogCharHeight = 0.029f;

Menu& Menu::getSingleton()
{
  static Menu Instance;
  return Instance;
}

Menu::Menu()
{
  m_DialogueLineCount = 0;
  m_QuestLogEntryCount = 0;
  m_QuestLogOffset = 0;
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

  Dialogue::Handle temp = Dialogue::getSingleton().getGreetingLine(who);
  if (temp.LineID == "") return false; //nothing found here

  //get text of all dialogue line options
  std::vector<std::string> sv;
  sv.clear();
  m_OptionIDs.clear();
  unsigned int i;
  for (i=0; i<temp.Choices.size(); i=i+1)
  {
    sv.push_back(Dialogue::getSingleton().getText(temp.Choices[i]));
    m_OptionIDs.push_back(temp.Choices[i]);
  }//for
  if (temp.Choices.size()==0)
  {
    sv.push_back("[End of Dialogue]");
    m_OptionIDs.push_back("");
  }
  showDialogue(temp.Text, sv);
  Dialogue::getSingleton().processResultScript(temp.LineID);
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

  Dialogue::Handle tempHandle = Dialogue::getSingleton().getDialogueLine(
                                  m_OptionIDs[chosenOption-1], m_DialoguePartner);
  Dialogue::getSingleton().processResultScript(m_OptionIDs[chosenOption-1]);
  if (tempHandle.Choices.size()==0)
  {
    //no more lines here... quit dialogue
    hideDialogue();
    m_OptionIDs.clear();
    m_DialoguePartner = NULL;
    return true;
  }
  //we have more choices, NPC will always select the first one available
  tempHandle = Dialogue::getSingleton().getDialogueLine(tempHandle.Choices[0],
                                                          m_DialoguePartner);
  m_OptionIDs.clear();
  //get text of all options
  std::vector<std::string> sv;
  sv.clear();
  unsigned int i;
  for (i=0; i<tempHandle.Choices.size(); i=i+1)
  {
    sv.push_back(Dialogue::getSingleton().getText(tempHandle.Choices[i]));
    m_OptionIDs.push_back(tempHandle.Choices[i]);
  }//for
  if (tempHandle.Choices.size()==0)
  {
    sv.push_back("[End of Dialogue]");
    m_OptionIDs.push_back("");
  }
  showDialogue(tempHandle.Text, sv);
  Dialogue::getSingleton().processResultScript(tempHandle.LineID);
  return true;
}

void Menu::showQuestLog(const bool visible)
{
  Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();
  if (om==NULL) return;
  //Do we actually need to do something?
  if (visible==isQuestLogVisible()) return;

  if (visible)
  {
    //show it
    // -- create basic overlay
    Ogre::Overlay* ol = om->create(cQuestLogOverlay);
    // -- create container - panel
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
                 om->createOverlayElement("Panel", cQuestLogOverlay+"/Panel"));
    panel->setMetricsMode(Ogre::GMM_RELATIVE);
    panel->setPosition(0.0, 0.0);
    panel->setDimensions(1.0, 1.0);
    /*set material via panel->setMaterialName(...)?*/
    panel->setMaterialName("QuestLog/Transparency");
    //add panel to overlay
    ol->add2D(panel);

    //create elements
    // -- "vorwärts blättern"
    Ogre::TextAreaOverlayElement* text_elem = static_cast<Ogre::TextAreaOverlayElement*>(
                         om->createOverlayElement("TextArea", cQuestLogOverlay+"/Plus"));
    text_elem->setMetricsMode(Ogre::GMM_RELATIVE);
    text_elem->setPosition(0.125, 0.9);
    text_elem->setDimensions(0.25, 0.1);
    text_elem->setCaption("+ (Next)");
    text_elem->setAlignment(Ogre::TextAreaOverlayElement::Center);
    text_elem->setFontName("Console");
    text_elem->setColour(Ogre::ColourValue(1.0, 0.5, 0.0));
    text_elem->setCharHeight(cQuestLogCharHeight);
    panel->addChild(text_elem);
    //-- "rückwärts blättern"
    text_elem = static_cast<Ogre::TextAreaOverlayElement*>(
              om->createOverlayElement("TextArea", cQuestLogOverlay+"/Minus"));
    text_elem->setMetricsMode(Ogre::GMM_RELATIVE);
    text_elem->setPosition(0.425, 0.9);
    text_elem->setDimensions(0.25, 0.1);
    text_elem->setCaption("- (Previous)");
    text_elem->setAlignment(Ogre::TextAreaOverlayElement::Center);
    text_elem->setFontName("Console");
    text_elem->setColour(Ogre::ColourValue(1.0, 0.5, 0.0));
    text_elem->setCharHeight(cQuestLogCharHeight);
    panel->addChild(text_elem);

    //lists all entries that fit onto the page
    showQuestLogEntries();

    // -- page number
    text_elem = static_cast<Ogre::TextAreaOverlayElement*>(
              om->createOverlayElement("TextArea", cQuestLogOverlay+"/Page"));
    text_elem->setMetricsMode(Ogre::GMM_RELATIVE);
    text_elem->setPosition(0.725, 0.9);
    text_elem->setDimensions(0.25, 0.1);
    text_elem->setCaption("Page "+IntToString(m_QuestLogOffset/cQuestLogEntriesPerPage+1));
    text_elem->setAlignment(Ogre::TextAreaOverlayElement::Center);
    text_elem->setFontName("Console");
    text_elem->setColour(Ogre::ColourValue(1.0, 0.5, 0.0));
    text_elem->setCharHeight(cQuestLogCharHeight);
    panel->addChild(text_elem);

    //show the overlay
    ol->show();
  }//if visible
  else
  {
    //destroy all elements
    unsigned int i;
    // destroy entries
    for (i=0; i<m_QuestLogEntryCount; ++i)
    {
      om->destroyOverlayElement(cQuestLogOverlay+"/"+IntToString(i));
    }//for
    m_QuestLogEntryCount = 0;
    //destroy navigation elements
    om->destroyOverlayElement(cQuestLogOverlay+"/Plus");
    om->destroyOverlayElement(cQuestLogOverlay+"/Minus");
    om->destroyOverlayElement(cQuestLogOverlay+"/Page");
    //destroy panel
    om->destroyOverlayElement(cQuestLogOverlay+"/Panel");
    //destroy overlay
    om->destroy(cQuestLogOverlay);
  }//else
}//function

void Menu::showQuestLogEntries()
{
  Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();
  if (om==NULL) return;
  Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
                            om->getOverlayElement(cQuestLogOverlay+"/Panel"));
  if (panel==NULL)
  {
    std::cout << "Menu::showQuestLogEntries: ERROR: Panel not found!\n";
    return;
  }//if
  std::vector<QLogEntry> entries = QuestLog::getSingleton().listQuestEntries(m_QuestLogOffset, cQuestLogEntriesPerPage);
  if (m_QuestLogOffset>0 and entries.size()==0)
  { //reset offset, because we've gone too far, and fetch entries again
    m_QuestLogOffset = 0;
    entries = QuestLog::getSingleton().listQuestEntries(m_QuestLogOffset, cQuestLogEntriesPerPage);
  }
  Ogre::TextAreaOverlayElement* text_elem = NULL;

  const Ogre::FontPtr glyphs = Ogre::FontManager::getSingleton().getByName("Console");
  unsigned int i;
  //create text areas for entries
  for (i=0; i<entries.size(); ++i)
  {
    text_elem = static_cast<Ogre::TextAreaOverlayElement*>(
        om->createOverlayElement("TextArea", cQuestLogOverlay+"/"+IntToString(i)));
    text_elem->setMetricsMode(Ogre::GMM_RELATIVE);
    text_elem->setPosition(0.025, cQuestLogEntryHeight*i+0.5*cQuestLogEntryHeight);
    text_elem->setDimensions(0.725, cQuestLogEntryHeight);
    text_elem->setAlignment(Ogre::TextAreaOverlayElement::Left);
    text_elem->setCaption(chopString(Journal::getSingleton().getText(entries[i].questID, entries[i].index),
                cQuestLogCharHeight, glyphs));
    text_elem->setFontName("Console");
    text_elem->setColour(Ogre::ColourValue(1.0, 0.5, 0.0));
    text_elem->setCharHeight(cQuestLogCharHeight);
    panel->addChild(text_elem);
  }//for
  //delete unneccessary textareas, if present
  for (i=entries.size(); i<m_QuestLogEntryCount; ++i)
  {
    om->destroyOverlayElement(cQuestLogOverlay+"/"+IntToString(i));
  }//for
  m_QuestLogEntryCount = entries.size();
  //show notification, if no elements are present yet
  if (entries.size()==0)
  {
    text_elem = static_cast<Ogre::TextAreaOverlayElement*>(
                    om->createOverlayElement("TextArea", cQuestLogOverlay+"/0"));
    text_elem->setMetricsMode(Ogre::GMM_RELATIVE);
    text_elem->setPosition(0.025, 0.0+0.5*cQuestLogEntryHeight);
    text_elem->setDimensions(0.725, cQuestLogEntryHeight);
    text_elem->setAlignment(Ogre::TextAreaOverlayElement::Center);
    text_elem->setCaption("You don't have any journal entries yet!");
    text_elem->setFontName("Console");
    text_elem->setColour(Ogre::ColourValue(1.0, 0.5, 0.0));
    text_elem->setCharHeight(cQuestLogCharHeight);
    panel->addChild(text_elem);
    m_QuestLogEntryCount = 1;
  }//if
}//func

bool Menu::isQuestLogVisible() const
{
  Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();
  if (om==NULL) return false;
  const Ogre::Overlay* o_lay = om->getByName(cQuestLogOverlay);
  if (o_lay!=NULL) return o_lay->isVisible();
  return false;
}

void Menu::nextQuestLogPage()
{
  if (isQuestLogVisible())
  {
    m_QuestLogOffset = m_QuestLogOffset + cQuestLogEntriesPerPage;
    //update quest log
    showQuestLog(false);
    showQuestLog(true);
  }//if
}

void Menu::previousQuestLogPage()
{
  if (isQuestLogVisible())
  {
    if (m_QuestLogOffset>cQuestLogEntriesPerPage)
    {
      m_QuestLogOffset = m_QuestLogOffset - cQuestLogEntriesPerPage;
    }
    else
    {
      m_QuestLogOffset = 0;
    }
    //update quest log
    showQuestLog(false);
    showQuestLog(true);
  }//if
}

std::string Menu::chopString(const std::string& src, const float h, const Ogre::FontPtr& glyphs)
{
  if (h<=0.0f or glyphs.isNull()) return src;
  const float maxWidth = 1.05;
  unsigned int i;
  std::string result = "";
  float currentWidth = 0.0f;
  for (i=0; i<src.size(); ++i)
  {
    if (src.at(i)=='\n')
    {
      currentWidth = 0.0f;
      result.append(1, '\n');
    }
    else
    {
      const float add_w = glyphs->getGlyphAspectRatio(src.at(i))*h;
      if (currentWidth+add_w>maxWidth)
      {
        result = result+"\n"+src.at(i);
        currentWidth = add_w;
      }//if
      else
      {
        result = result + src.at(i);
        currentWidth = currentWidth + add_w;
      }//else
    }//else
  }//for
  return result;
}

} //namespace
