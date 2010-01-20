/*---------------------------------------------------------------------------
 Authors: thoronador
 Date:    2010-01-20
 Purpose: Menu Singleton class
          Manages display of in game menues.

 History:
     - 2010-01-20 (rev 158) - initial version (by thoronador)

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

namespace Dusk
{

  class Menu
  {
    public:
      /* Singleton access method */
      static Menu& GetSingleton();

      /* destructor */
      virtual ~Menu();

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

      /* returns true, if the dialogue menu is currently visible */
      bool isDialogueActive() const;

      /* maximum number of dialogue choices to display at one time */
      static const unsigned int cMaxDialogueOptions;
    private:
      Menu();
      Menu(const Menu& op) {}

      /* delete all dialogue lines, i.e. their respective overlay elements */
      void killDialogueOverlayLines();
      unsigned int m_DialogueLineCount;

      /* name of overlay for dialogue

         remarks:
             Don't change this, or you wil have to edit the overlay script, too.
      */
      static const std::string cDialogueOverlay;
  };

} //namespace

#endif // MENU_H
