/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Editor.
    Copyright (C) 2010, 2011 thoronador

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

#include "EditorApplicationLight.h"
#include "EditorApplicationBase.h"
#include <CEGUI/CEGUI.h>
#include <OgreLight.h>
#include "../Engine/DuskFunctions.h"
#include "../Engine/ObjectManager.h"
#include "../Engine/API.h"

namespace Dusk
{

std::string OgreLightTypeToString(const Ogre::Light::LightTypes val)
{
  switch (val)
  {
    case Ogre::Light::LT_POINT:
         return "Point"; break;
    case Ogre::Light::LT_DIRECTIONAL:
         return "Directional"; break;
    case Ogre::Light::LT_SPOTLIGHT:
         return "Spotlight"; break;
    default: return "(unknown)"; //should never occur
  }//swi
}

EditorApplicationLight::EditorApplicationLight()
{
  ID_of_light_to_delete = "";
  ID_of_light_to_edit = "";
}

EditorApplicationLight::~EditorApplicationLight()
{
  //empty
}

void EditorApplicationLight::addLightRecordToCatalogue(const std::string& ID, const LightRecord& Record)
{
  CEGUI::MultiColumnList* mcl = NULL;
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/Light/List"))
  {
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Light/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(FloatToString(Record.red));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(Record.green));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(Record.blue));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(FloatToString(Record.radius));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
  lbi = new CEGUI::ListboxTextItem(OgreLightTypeToString(Record.type));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 5, row);
}

void EditorApplicationLight::refreshLightList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/Light/List"))
  {
    showWarning("ERROR: Could not find light list window in CEGUI Window Manager!");
    return;
  }
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Light/List"));
  mcl->resetList();

  LightBase::Iterator first = LightBase::getSingleton().getFirst();
  const LightBase::Iterator end = LightBase::getSingleton().getEnd();
  while (first != end)
  {
    addLightRecordToCatalogue(first->first, first->second);
    ++first;
  }//while
  return;
}

void EditorApplicationLight::createPopupMenuLightTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for lights' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/LightPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/LightPopUp/New"));
  menu_item->setText("New light...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationLight::LightNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/LightPopUp/Edit"));
  menu_item->setText("Edit selected light...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationLight::LightEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/LightPopUp/Delete"));
  menu_item->setText("Delete selected light");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationLight::LightDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/Light/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationLight::LightNewClicked(const CEGUI::EventArgs &e)
{
  showLightNewWindow();
  return true;
}

bool EditorApplicationLight::LightEditClicked(const CEGUI::EventArgs &e)
{
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Light/List"));
  CEGUI::ListboxItem* lb_item = mcl->getFirstSelectedItem();
  if (lb_item==NULL)
  {
    showHint("You have not selected a light from the list!");
    return true;
  }
  unsigned int row_index = mcl->getItemRowIndex(lb_item);
  lb_item = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
  ID_of_light_to_edit = std::string(lb_item->getText().c_str());
  showLightEditWindow();
  return true;
}

bool EditorApplicationLight::LightDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/Light/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a light from the list to delete it.");
  }
  else
  {
    unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_light_to_delete = std::string(lbi->getText().c_str());
    showLightConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationLight::showLightNewWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/LightNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/LightNewFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/LightNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New Light...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/LightNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for colour
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/Colour_Label");
    button->setText("Colour");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for red
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/Red_Label");
    button->setText("Red");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //static text for green
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/Green_Label");
    button->setText("Green");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.525, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //static text for blue
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/Blue_Label");
    button->setText("Blue");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    CEGUI::Spinner* spin = NULL;
    //spinner for red
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightNewFrame/RedSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);
    //spinner for green
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightNewFrame/GreenSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.525, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);
    //spinner for blue
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightNewFrame/BlueSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.65, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);

    //static text for light type
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightNewFrame/Type_Label");
    button->setText("Type");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    CEGUI::RadioButton* radio = NULL;
    //radio button for point light
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LightNewFrame/RadioPoint"));
    radio->setText("Point");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.4, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(true);
    radio->setGroupID(444);
    frame->addChildWindow(radio);
    //radio button for directional light
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LightNewFrame/RadioDirectional"));
    radio->setText("Directional");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.525, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(false);
    radio->setGroupID(444);
    frame->addChildWindow(radio);

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LightNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightNewFrameOKClicked, this));
    frame->addChildWindow(button);

    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LightNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.38, 0), CEGUI::UDim(0.17, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();
}

void EditorApplicationLight::showLightEditWindow(void)
{
  if (ID_of_light_to_edit=="")
  {
    std::cout << "showLightEditWindow: No ID given.\n";
    return;
  }

  if (!LightBase::getSingleton().hasLight(ID_of_light_to_edit))
  {
    std::cout << "showLightEditWindow: Light not present in database.\n";
    showWarning("There seems to be no light with the ID \""+ID_of_light_to_edit
                +"\". Aborting.");
    return;
  }

  CEGUI::FrameWindow* frame = NULL;
  CEGUI::Window * button = NULL;
  CEGUI::Spinner* spin = NULL;
  CEGUI::RadioButton* radio = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();

  if (winmgr.isWindowPresent("Editor/LightEditFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/LightEditFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/LightEditFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Edit Light");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/LightEditFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for colour
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Colour_Label");
    button->setText("Colour");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for red
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Red_Label");
    button->setText("Red");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //static text for green
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Green_Label");
    button->setText("Green");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.525, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //static text for blue
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Blue_Label");
    button->setText("Blue");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //spinner for red
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightEditFrame/RedSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);
    //spinner for green
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightEditFrame/GreenSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.525, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);
    //spinner for blue
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/LightEditFrame/BlueSpin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.65, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    spin->setTextInputMode(CEGUI::Spinner::FloatingPoint);
    spin->setStepSize(0.1); spin->setMaximumValue(1.0f); spin->setMinimumValue(0.0f);
    spin->setText("1.0");
    frame->addChildWindow(spin);

    //static text for light type
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Type_Label");
    button->setText("Type");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //radio button for point light
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LightEditFrame/RadioPoint"));
    radio->setText("Point");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.4, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(true);
    radio->setGroupID(445);
    frame->addChildWindow(radio);
    //radio button for directional light
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/LightEditFrame/RadioDirectional"));
    radio->setText("Directional");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.525, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(false);
    radio->setGroupID(445);
    frame->addChildWindow(radio);

    //static text for radius
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/LightEditFrame/Radius_Label");
    button->setText("Radius:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);
    //editbox for radius
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/LightEditFrame/Radius_Edit");
    button->setText("123.4");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75, 0), CEGUI::UDim(0.65, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //Save button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LightEditFrame/Save");
    button->setText("Save");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightEditFrameSaveClicked, this));
    frame->addChildWindow(button);

    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LightEditFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightEditFrameCancelClicked, this));
    frame->addChildWindow(button);
  }//else
  //fill in the data
  LightRecord lr = LightBase::getSingleton().getLightData(ID_of_light_to_edit);
  button = winmgr.getWindow("Editor/LightEditFrame/ID_Edit");
  button->setText(ID_of_light_to_edit);
  //colour values
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/RedSpin"));
  spin->setCurrentValue(lr.red);
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/GreenSpin"));
  spin->setCurrentValue(lr.green);
  spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/BlueSpin"));
  spin->setCurrentValue(lr.blue);
  //type
  if (lr.type == Ogre::Light::LT_POINT)
  {
    radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightEditFrame/RadioPoint"));
  }
  else
  {
    radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightEditFrame/RadioDirectional"));
  }
  radio->setSelected(true);
  //radius
  button = winmgr.getWindow("Editor/LightEditFrame/Radius_Edit");
  button->setText(FloatToString(lr.radius));
  //position frame window
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.32, 0), CEGUI::UDim(0.27, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();
}

void EditorApplicationLight::showLightConfirmDeleteWindow(void)
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/LightDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/LightDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/LightDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete Light...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/LightDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    textbox->setText("Do you really want to delete the light \""+ID_of_light_to_delete
                     +"\" and all of its references?");
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/LightDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/LightDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.22, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

void EditorApplicationLight::showLightEditConfirmIDChangeWindow(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow*  frame = NULL;

  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/ConfirmLightIDChangeFrame"));
  }
  else
  {
    //create it
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/ConfirmLightIDChangeFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("Rename Light?");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox",
                                                        "Editor/ConfirmLightIDChangeFrame/Text"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.12, 0), CEGUI::UDim(0.17, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    if (winmgr.isWindowPresent("Editor/LightEditFrame/ID_Edit"))
    {
      textbox->setText("The ID of this light has changed.\nDo you want to rename the light \""
                   +ID_of_light_to_edit+"\" to \""
                   +winmgr.getWindow("Editor/LightEditFrame/ID_Edit")->getText()
                   +"\" or create a new one?");
    }
    else
    {
      textbox->setText("The ID of this light was changed.\nDo you want to rename the light \""
                       +ID_of_light_to_edit+"\" to \">insert new ID here<\" or create a new one?");
    }
    frame->addChildWindow(textbox);

    //buttons: New, Rename, Cancel
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmLightIDChangeFrame/New");
    button->setText("New Light");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.06, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightConfirmIDChangeNewClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmLightIDChangeFrame/Rename");
    button->setText("Rename Light");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightConfirmIDChangeRenameClicked, this));

    button = winmgr.createWindow("TaharezLook/Button", "Editor/ConfirmLightIDChangeFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.1, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.68, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationLight::LightConfirmIDChangeCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  frame->moveToFront();
}

bool EditorApplicationLight::LightDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/LightDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/LightDeleteFrame");
  }
  return true;
}

bool EditorApplicationLight::LightDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_light_to_delete == "")
  {
    showWarning("Error: light ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LightDeleteFrame");
    return true;
  }
  if (!LightBase::getSingleton().deleteLight(ID_of_light_to_delete))
  {
    showHint("LightBase class holds no item of the given ID ("
             +ID_of_light_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LightDeleteFrame");
    return true;
  }
  //kill references
  unsigned int refs_deleted = ObjectManager::getSingleton().deleteReferencesOfObject(ID_of_light_to_delete);
  if (refs_deleted == 0)
  {
    showHint("Light \""+ID_of_light_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("Light \""+ID_of_light_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/Light/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_light_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_light_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LightDeleteFrame");
  return true;
}

bool EditorApplicationLight::LightNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  if (CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/LightNewFrame"))
  {
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LightNewFrame");
  }
  return true;
}

bool EditorApplicationLight::LightNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/LightNewFrame"))
  {
    CEGUI::Editbox* id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/LightNewFrame/ID_Edit"));
    CEGUI::Spinner* red_spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightNewFrame/RedSpin"));
    CEGUI::Spinner* green_spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightNewFrame/GreenSpin"));
    CEGUI::Spinner* blue_spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightNewFrame/BlueSpin"));
    CEGUI::RadioButton* point_radio = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightNewFrame/RadioPoint"));
    //make sure we have some data
    if (id_edit->getText()=="")
    {
      showWarning("You have to enter an ID string to create a new light!");
      return true;
    }
    //check for presence of light with same ID
    if (LightBase::getSingleton().hasLight(std::string(id_edit->getText().c_str())))
    {
      showWarning("A Light with the given ID already exists!");
      return true;
    }
    //add it to LightBase
    LightRecord entered_data;
    entered_data.red = red_spin->getCurrentValue();
    entered_data.green = green_spin->getCurrentValue();
    entered_data.blue = blue_spin->getCurrentValue();
    if (point_radio->isSelected())
    {
      entered_data.type = Ogre::Light::LT_POINT;
    }
    else
    {
      entered_data.type = Ogre::Light::LT_DIRECTIONAL;
    }
    //assume radius
    entered_data.radius = 250.0f;
    LightBase::getSingleton().addLight( std::string(id_edit->getText().c_str()), entered_data);
    //update item catalogue
    addLightRecordToCatalogue(std::string(id_edit->getText().c_str()), entered_data);
    //destroy window
    winmgr.destroyWindow("Editor/LightNewFrame");
  }
  return true;
}

bool EditorApplicationLight::LightEditFrameCancelClicked(const CEGUI::EventArgs &e)
{
  if (CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/LightEditFrame"))
  {
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/LightEditFrame");
  }
  if (CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/ConfirmLightIDChangeFrame"))
  {
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/ConfirmLightIDChangeFrame");
  }
  return true;
}

bool EditorApplicationLight::LightEditFrameSaveClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::Editbox* id_edit;
  CEGUI::Spinner* spin_red;
  CEGUI::Spinner* spin_green;
  CEGUI::Spinner* spin_blue;
  CEGUI::RadioButton* radio_type;
  CEGUI::Editbox* radius_edit;

  if (!winmgr.isWindowPresent("Editor/LightEditFrame/ID_Edit") ||
      !winmgr.isWindowPresent("Editor/LightEditFrame/RedSpin") ||
      !winmgr.isWindowPresent("Editor/LightEditFrame/GreenSpin") ||
      !winmgr.isWindowPresent("Editor/LightEditFrame/BlueSpin") ||
      !winmgr.isWindowPresent("Editor/LightEditFrame/RadioPoint") ||
      !winmgr.isWindowPresent("Editor/LightEditFrame/Radius_Edit"))
  {
    showWarning("Error: Editbox for ID, Spinners for Colour or RadioButtons for"
               +std::string(" light type are not registered at window manager!"));
    return true;
  }//if
  id_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/LightEditFrame/ID_Edit"));
  spin_red = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/RedSpin"));
  spin_green = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/GreenSpin"));
  spin_blue = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/BlueSpin"));
  radio_type = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightEditFrame/RadioPoint"));
  radius_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/LightEditFrame/Radius_Edit"));

  if (std::string(id_edit->getText().c_str())=="")
  {
    showHint("You have to enter an ID for this light!");
    return true;
  }
  if (std::string(radius_edit->getText().c_str())=="")
  {
    showHint("You have to enter a radius for this light!");
    return true;
  }
  if (StringToFloat(std::string(radius_edit->getText().c_str()), -1.0f)<=0.0f)
  {
    showHint("The radius has to be a positive floating point value!");
    return true;
  }

  if (std::string(id_edit->getText().c_str())!=ID_of_light_to_edit)
  {
    //ID was changed
   showLightEditConfirmIDChangeWindow();
   return true;
  }
  LightRecord lr;
  lr.red = spin_red->getCurrentValue();
  lr.green = spin_green->getCurrentValue();
  lr.blue = spin_blue->getCurrentValue();
  if (radio_type->isSelected())
  {
    lr.type = Ogre::Light::LT_POINT;
  }
  else
  {
    lr.type = Ogre::Light::LT_DIRECTIONAL;
  }
  lr.radius = StringToFloat(std::string(radius_edit->getText().c_str()), 123.45f);
  //check if data has remained the same
  if (lr == (LightBase::getSingleton().getLightData(ID_of_light_to_edit)))
  {
    showHint("You have not changed the data of this Light, thus there are no changes to be saved.");
    return true;
  }
  //save it
  LightBase::getSingleton().addLight(ID_of_light_to_edit, lr);
  //update list
  refreshLightList();
  //reference update
  unsigned int ref_count =
  ObjectManager::getSingleton().reenableReferencesOfObject(ID_of_light_to_edit, getAPI().getOgreSceneManager()/* mSceneMgr*/);
  showHint("The Light \""+ID_of_light_to_edit+"\" and "+IntToString(ref_count)
          +" references were updated!");
  //delete window
  if (winmgr.isWindowPresent("Editor/LightEditFrame"))
  {
    winmgr.destroyWindow("Editor/LightEditFrame");
  }
  ID_of_light_to_edit = "";
  return true;
}

bool EditorApplicationLight::LightConfirmIDChangeRenameClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/ID_Edit") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/RedSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/GreenSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/BlueSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/RadioPoint") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/Radius_Edit"))
  {
    winmgr.destroyWindow("Editor/ConfirmLightIDChangeFrame");
    //get the windows with the needed entries
    std::string LightID;
    LightID = std::string(winmgr.getWindow("Editor/LightEditFrame/ID_Edit")->getText().c_str());
    if (LightBase::getSingleton().hasLight(LightID))
    {
      showWarning("A Light with the ID \""+LightID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" light the same ID.");
      return true;
    }//if
    CEGUI::Spinner* spin_red; CEGUI::Spinner* spin_green; CEGUI::Spinner* spin_blue;
    CEGUI::RadioButton* radio_type;
    CEGUI::Editbox* radius_edit;
    spin_red = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/RedSpin"));
    spin_green = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/GreenSpin"));
    spin_blue = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/BlueSpin"));
    radio_type = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightEditFrame/RadioPoint"));
    radius_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/LightEditFrame/Radius_Edit"));

    LightRecord lr;
    lr.red = spin_red->getCurrentValue();
    lr.green = spin_green->getCurrentValue();
    lr.blue = spin_blue->getCurrentValue();
    lr.radius = StringToFloat(std::string(radius_edit->getText().c_str()), 123.45f);
    if (radio_type->isSelected())
    {
      lr.type = Ogre::Light::LT_POINT;
    }
    else
    {
      lr.type = Ogre::Light::LT_DIRECTIONAL;
    }
    //"rename", i.e. create light with new ID and delete object with old ID
    LightBase::getSingleton().addLight(LightID, lr);
    LightBase::getSingleton().deleteLight(ID_of_light_to_edit);
    //update all lights with same ID
    ObjectManager::getSingleton().updateReferencesAfterIDChange(ID_of_light_to_edit, LightID, getAPI().getOgreSceneManager()/*mSceneMgr*/);
    //add row for new light to catalogue
    addLightRecordToCatalogue(LightID, lr);
    //remove row of old ID
    CEGUI::MultiColumnList * mcl;
    CEGUI::ListboxItem * lb_item = NULL;
    mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/Light/List"));
    lb_item = mcl->findColumnItemWithText(ID_of_light_to_edit, 0, NULL);
    mcl->removeRow(mcl->getItemRowIndex(lb_item));
    //close edit window
    winmgr.destroyWindow("Editor/LightEditFrame");
    ID_of_light_to_edit = "";
  }
  return true;
}

bool EditorApplicationLight::LightConfirmIDChangeNewClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/ID_Edit") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/RedSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/GreenSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/BlueSpin") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/RadioPoint") &&
      winmgr.isWindowPresent("Editor/LightEditFrame/Radius_Edit"))
  {
    //close confirmation window
    winmgr.destroyWindow("Editor/ConfirmLightIDChangeFrame");
    //get the editboxes/ spinners/ radio boxes with the needed entries
    std::string LightID;
    LightRecord lr;
    LightID = std::string(winmgr.getWindow("Editor/LightEditFrame/ID_Edit")->getText().c_str());

    CEGUI::Spinner* spin_red; CEGUI::Spinner* spin_green; CEGUI::Spinner* spin_blue;
    CEGUI::RadioButton* radio_type;
    CEGUI::Editbox* radius_edit;
    spin_red = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/RedSpin"));
    spin_green = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/GreenSpin"));
    spin_blue = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/LightEditFrame/BlueSpin"));
    radio_type = static_cast<CEGUI::RadioButton*> (winmgr.getWindow("Editor/LightEditFrame/RadioPoint"));
    radius_edit = static_cast<CEGUI::Editbox*> (winmgr.getWindow("Editor/LightEditFrame/Radius_Edit"));

    lr.red = spin_red->getCurrentValue();
    lr.green = spin_green->getCurrentValue();
    lr.blue = spin_blue->getCurrentValue();
    lr.radius = StringToFloat(std::string(radius_edit->getText().c_str()), 123.45f);
    if (radio_type->isSelected())
    {
      lr.type = Ogre::Light::LT_POINT;
    }
    else
    {
      lr.type = Ogre::Light::LT_DIRECTIONAL;
    }

    if (LightBase::getSingleton().hasLight(LightID))
    {
      showWarning("A Light with the ID \""+LightID+"\" already exists. "
                  +"Change that one as needed or delete it before giving another"
                  +" light the same ID.");
      return true;
    }//if
    //add new row to catalogue
    addLightRecordToCatalogue(LightID, lr);
    //add new object to database (ObjectBase)
    LightBase::getSingleton().addLight(LightID, lr);
    //close edit window
    winmgr.destroyWindow("Editor/LightEditFrame");
    ID_of_light_to_edit = "";
  }
  return true;
}

bool EditorApplicationLight::LightConfirmIDChangeCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmLightIDChangeFrame");
  }
  return true;
}

void EditorApplicationLight::closeEditWindowsLight(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  //frame window for new lights
  if (winmgr.isWindowPresent("Editor/LightNewFrame"))
  {
    winmgr.destroyWindow("Editor/LightNewFrame");
  }
  //frame window for editing lights
  if (winmgr.isWindowPresent("Editor/LightEditFrame"))
  {
    winmgr.destroyWindow("Editor/LightEditFrame");
  }
  //frame for deleting Lights
  if (winmgr.isWindowPresent("Editor/LightDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/LightDeleteFrame");
  }
  //frame to change ID of lights
  if (winmgr.isWindowPresent("Editor/ConfirmLightIDChangeFrame"))
  {
    winmgr.destroyWindow("Editor/ConfirmLightIDChangeFrame");
  }
}

void EditorApplicationLight::createLightCatalogueTab(CEGUI::WindowManager& winmgr, CEGUI::TabControl * tab)
{
  //Light tab
  CEGUI::Window* pane = winmgr.createWindow("TaharezLook/TabContentPane", "Editor/Catalogue/Tab/Light");
  pane->setSize(CEGUI::UVector2(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
  pane->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0, 0), CEGUI::UDim(0.0, 0)));
  pane->setText("Lights");
  tab->addTab(pane);

  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/Catalogue/Tab/Light/List"));
  mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0.9, 0)));
  mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.05, 0)));
  mcl->addColumn("ID", 0, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Red", 1, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Green", 2, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Blue", 3, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Radius", 4, CEGUI::UDim(0.16, 0));
  mcl->addColumn("Type", 5, CEGUI::UDim(0.16, 0));
  mcl->setUserColumnDraggingEnabled(false);
  pane->addChildWindow(mcl);
  mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationLight::LightTabClicked, this));

  //sample data
  LightBase::getSingleton().addLight("light_red", LightRecord::getRed(123.4));
  LightBase::getSingleton().addLight("light_green", LightRecord::getGreen(23.4));
  LightBase::getSingleton().addLight("light_blue", LightRecord::getBlue(3.4));
  refreshLightList();
}

bool EditorApplicationLight::LightTabClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/Catalogue/LightPopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/Catalogue/Tab/Light/List")->getPixelRect();
      const float pu_x = (mea.position.d_x-mcl_rect.d_left)/mcl_rect.getWidth();
      const float pu_y = (mea.position.d_y-mcl_rect.d_top)/mcl_rect.getHeight();
      popup->setPosition(CEGUI::UVector2(CEGUI::UDim(pu_x, 0), CEGUI::UDim(pu_y, 0)));
      popup->openPopupMenu();
    }
  }
  else
  {
    popup->closePopupMenu();
  }
  return true;
}

} //namespace
