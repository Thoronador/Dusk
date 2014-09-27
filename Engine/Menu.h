/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 Authors: thoronador
 Date:    2010-03-03
 Purpose: Menu Singleton class
          Manages display of in game menus.

 History:
     - 2010-01-20 (rev 158) - initial version (by thoronador)
     - 2010-01-26 (rev 159) - startDialogueWithNPC() and nextDialogueChoice()
                              added to handle display of dialogue lines
     - 2010-03-03 (rev 176) - processing of result scripts added
     - 2010-08-15 (rev 225) - fixed two bugs that could cause a crash/ error on
                              program termination
     - 2010-08-20 (rev 231) - visualisation of quest log implemented (partially)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced

 ToDo list:
     - get a better material or texture for dialogue menu background
     - implement more stuff (like visualizing the inventory?)
     - ???

 Bugs:
     - Untested. If you find a bug (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include "objects/NPC.h"

namespace Dusk
{

  class Menu
  {
    public:
      /* Singleton access method */
      static Menu& getSingleton();

      /* destructor */
      virtual ~Menu();

      /* returns true, if the dialogue menu is currently visible */
      bool isDialogueActive() const;

      /* starts dialogue with given NPC and shows dialogue menu. Returns true,
         if a conversation was started.

         remarks:
             Will return false if another dialogue is active. In this case the
             active dialogue will be continued as if this function was not
             called.

         parameters:
             who - pointer to the NPC we want to talk to
      */
      bool startDialogueWithNPC(NPC* who);

      /* tells the menu class which dialogue option the player has chosen and
         progresses the dialogue accordingly. Returns true, if chosenOption was
         a valid parameter.

         parameter:
             chosenOption - number of option the player has chosen
                            Zero indicates that the player wants to quit the
                            dialogue menu. One means the first option, two is
                            the second option, and so on.
      */
      bool nextDialogueChoice(const unsigned int chosenOption);

      /* maximum number of dialogue choices to display at one time */
      static const unsigned int cMaxDialogueOptions;

      /* sets visibility of the QuestLog entries

         parameters:
             visible - true, if entries shall be shown, false otherwise
      */
      void showQuestLog(const bool visible);

      /* returns true, if the quest log is currently visible */
      bool isQuestLogVisible() const;

      /* switches to next page in QuestLog */
      void nextQuestLogPage();

      /* switches to previous page in QuestLog */
      void previousQuestLogPage();
    private:
      /* constructor - private due to singleton pattern */
      Menu();

      /* empty, private copy constructor (singleton) */
      Menu(const Menu& op) {}

      /* shows the dialogue menu

         parameters:
             first   - the line the NPC is speaking
             options - vector of dialogue lines the player can choose from

         remarks:
             Currently, every vector containing more than cMaxDialogueOptions
             options will not be shown completely. Only the first few choices
             will be displayed.
      */
      void showDialogue(const std::string& first, const std::vector<std::string>& options);

      /* makes the dialogue menu disappear */
      void hideDialogue();

      /* delete all dialogue lines, i.e. their respective overlay elements */
      void killDialogueOverlayLines();
      unsigned int m_DialogueLineCount;

      std::vector<std::string> m_OptionIDs; //IDs of current dialogue options
      NPC* m_DialoguePartner; //pointer to NPC talking in dialogue, or NULL

      /* name of overlay for dialogue

         remarks:
             Don't change this, or you will have to edit the overlay script, too.
      */
      static const std::string cDialogueOverlay;

      /* shows all questlog entries */
      void showQuestLogEntries();
      //saves how many quest log entries are shown at the moment
      unsigned int m_QuestLogEntryCount;
      //saves quest log offset (i.e. page, somehow)
      unsigned int m_QuestLogOffset;

      /* divides a string into multiple lines by adding newline characters into
         it after a certain width

         parameters:
             src    - the original string
             h      - height of the font
             glyphs - the font used
      */
      std::string chopString(const std::string& src, const float h, const Ogre::FontPtr& glyphs);
  };

} //namespace

#endif // MENU_H
