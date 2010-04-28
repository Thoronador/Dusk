/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-28
 Purpose: EditorApplicationBase class
          EditorApplicationNase is an ancestor of EditorApplicationNPC and holds
          the methods for displaying the warning/ hint window.

 History:
     - 2010-04-28 (rev 191)  - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef EDITORAPPLICATIONBASE_H
#define EDITORAPPLICATIONBASE_H

#include <string>
#include <CEGUI/CEGUIEventArgs.h>

namespace Dusk
{

class EditorApplicationBase
{
  protected:
    //general message windows
    void showWarning(const std::string& Text_of_warning);
    void showHint(const std::string& hint_text, const bool big=false);
    //callbacks for OK buttons
    bool WarningFrameOKClicked(const CEGUI::EventArgs &e);
	bool HintFrameOKClicked(const CEGUI::EventArgs &e);
}; //class

} //namespace

#endif // EDITORAPPLICATIONBASE_H
