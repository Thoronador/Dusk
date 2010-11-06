/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2010 thoronador

    The Dusk Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Editor.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
