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

#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>

namespace Dusk
{

void showWarning(const std::string& Text_of_warning)
{
  if (Text_of_warning=="")
  {
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/WarningFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/WarningFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/WarningFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Warning!");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static label for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/WarningFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create OK button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/WarningFrame/OK");
    button->setText("OK");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&WarningFrameOKClicked));
  }
  winmgr.getWindow("Editor/WarningFrame/Label")->setText(Text_of_warning);
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.25, 0)));
  frame->moveToFront();
}

void showHint(const std::string& hint_text, const bool big)
{
  if (hint_text=="")
  {
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::MultiLineEditbox* textbox = NULL;
  CEGUI::Window* button = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/HintFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/HintFrame"));
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.getWindow("Editor/HintFrame/Label"));
    button = winmgr.getWindow("Editor/HintFrame/OK");
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/HintFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Information");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //add static label for message
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/HintFrame/Label"));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);
    //create OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/HintFrame/OK");
    button->setText("OK");
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&HintFrameOKClicked));
  }
  winmgr.getWindow("Editor/HintFrame/Label")->setText(hint_text);
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));
  if (big)
  {
    frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.7, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.85, 0)));
  }
  else
  {
    frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.25, 0)));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.75, 0)));
  }
  frame->moveToFront();
}

bool WarningFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/WarningFrame"))
  {
    winmgr.destroyWindow("Editor/WarningFrame");
  }
  return true;
}

bool HintFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/HintFrame"))
  {
    winmgr.destroyWindow("Editor/HintFrame");
  }
  return true;
}

} //namespace
