/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-28
 Purpose: (former) EditorApplicationBase class
          methods for displaying the warning/ hint window

 History:
     - 2010-04-28 (rev 191) - initial version (by thoronador)
     - 2010-04-28 (rev 192) - class discarded; former member functions are
                              transformed into free functions

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

    //general message windows
    void showWarning(const std::string& Text_of_warning);
    void showHint(const std::string& hint_text, const bool big=false);
    //callbacks for OK buttons
    bool WarningFrameOKClicked(const CEGUI::EventArgs &e);
	bool HintFrameOKClicked(const CEGUI::EventArgs &e);

} //namespace

#endif // EDITORAPPLICATIONBASE_H
