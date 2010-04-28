#include "EditorApplicationNPC.h"
#include <CEGUI/CEGUI.h>
#include "EditorApplicationBase.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/AnimationData.h"

namespace Dusk
{

EditorApplicationNPC::EditorApplicationNPC()
{
  ID_of_NPC_to_delete = "";
}

EditorApplicationNPC::~EditorApplicationNPC()
{
  //empty
}

void EditorApplicationNPC::RefreshNPCList(void)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = NULL;
  if (!winmgr.isWindowPresent("Editor/Catalogue/Tab/NPC/List"))
  {
    showWarning("ERROR: Could not find NPC list window in CEGUI Window Manager!");
    return;
  }
  mcl = static_cast<CEGUI::MultiColumnList*> (winmgr.getWindow("Editor/Catalogue/Tab/NPC/List"));
  mcl->resetList();

  std::map<std::string, NPCRecord>::const_iterator first;
  first = NPCBase::GetSingleton().GetFirst();
  const std::map<std::string, NPCRecord>::const_iterator end = NPCBase::GetSingleton().GetEnd();
  while (first != end)
  {
    addNPCRecordToCatalogue(first->first, first->second);
    ++first;
  }//while
  return;
}

void EditorApplicationNPC::addNPCRecordToCatalogue(const std::string& ID, const NPCRecord& Record)
{
  if (!CEGUI::WindowManager::getSingleton().isWindowPresent("Editor/Catalogue/Tab/NPC/List"))
  {
    return;
  }
  CEGUI::MultiColumnList* mcl = NULL;
  mcl = static_cast<CEGUI::MultiColumnList*> (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem *lbi;
  unsigned int row;
  lbi = new CEGUI::ListboxTextItem(ID);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  row = mcl->addRow(lbi, 0);
  lbi = new CEGUI::ListboxTextItem(Record.Name);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 1, row);
  lbi = new CEGUI::ListboxTextItem(IntToString(Record.Level));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 2, row);
  lbi = new CEGUI::ListboxTextItem(BoolToString(Record.Female));
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 3, row);
  lbi = new CEGUI::ListboxTextItem(Record.Mesh);
  lbi->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
  mcl->setItem(lbi, 4, row);
}

void EditorApplicationNPC::CreatePopupMenuNPCTab(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for NPCs' tab
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/Catalogue/NPCPopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/New"));
  menu_item->setText("New NPC...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/Edit"));
  menu_item->setText("Edit selected NPC...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/Catalogue/NPCPopUp/Delete"));
  menu_item->setText("Delete selected NPC");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/Catalogue/Tab/NPC/List")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationNPC::NPCNewClicked(const CEGUI::EventArgs &e)
{
  showNPCNewWindow();
  return true;
}

bool EditorApplicationNPC::NPCEditClicked(const CEGUI::EventArgs &e)
{
  //not implemented yet
  return true;
}

bool EditorApplicationNPC::NPCDeleteClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (winmgr.getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select a NPC from the list to delete it.");
  }
  else
  {
    const unsigned int row_index = mcl->getItemRowIndex(lbi);
    lbi = mcl->getItemAtGridReference(CEGUI::MCLGridRef(row_index, 0));
    ID_of_NPC_to_delete = std::string(lbi->getText().c_str());
    showNPCConfirmDeleteWindow();
  }
  return true;
}

void EditorApplicationNPC::showNPCConfirmDeleteWindow()
{
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCDeleteFrame"));
  }
  else
  {
    //create it (frame first)
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCDeleteFrame"));
    frame->setInheritsAlpha(false);
    frame->setTitleBarEnabled(true);
    frame->setText("Delete NPC...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    //add static text box for message
    CEGUI::MultiLineEditbox* textbox;
    textbox = static_cast<CEGUI::MultiLineEditbox*> (winmgr.createWindow("TaharezLook/MultiLineEditbox", "Editor/NPCDeleteFrame/Label"));
    textbox->setSize(CEGUI::UVector2(CEGUI::UDim(0.8, 0), CEGUI::UDim(0.55, 0)));
    textbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.15, 0)));
    textbox->setWordWrapping(true);
    textbox->setReadOnly(true);
    frame->addChildWindow(textbox);

    //create yes button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCDeleteFrame/Yes");
    button->setText("Yes, go on.");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteFrameYesClicked, this));

    //create no button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCDeleteFrame/No");
    button->setText("No, wait!");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.2, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCDeleteFrameNoClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.31, 0), CEGUI::UDim(0.18, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.4, 0)));
  winmgr.getWindow("Editor/NPCDeleteFrame/Label")->setText("Do you really want to delete the NPC \""+ID_of_NPC_to_delete
                     +"\" and all of its references?");
  frame->moveToFront();
}

bool EditorApplicationNPC::NPCDeleteFrameNoClicked(const CEGUI::EventArgs &e)
{
  //delete window
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCDeleteFrame"))
  {
    winmgr.destroyWindow("Editor/NPCDeleteFrame");
  }
  return true;
}

bool EditorApplicationNPC::NPCDeleteFrameYesClicked(const CEGUI::EventArgs &e)
{
  if (ID_of_NPC_to_delete == "")
  {
    showWarning("Error: NPC ID is empty string!");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
    return true;
  }
  if (!NPCBase::GetSingleton().deleteNPC(ID_of_NPC_to_delete))
  {
    showHint("NPCBase class holds no NPC of the given ID ("
             +ID_of_NPC_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
    ID_of_NPC_to_delete = "";
    return true;
  }
  //kill references
  const unsigned int refs_deleted = AnimationData::GetSingleton().deleteReferencesOfAnimatedObject(ID_of_NPC_to_delete);
  if (refs_deleted == 0)
  {
    showHint("NPC \""+ID_of_NPC_to_delete+"\" deleted! It had no references which had to be deleted.");
  }
  else
  {
    showHint("NPC \""+ID_of_NPC_to_delete+"\" and "+IntToString(refs_deleted)+" reference(s) of it were deleted!");
  }
  //delete row in multi column list of lights
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                                (CEGUI::WindowManager::getSingleton().getWindow("Editor/Catalogue/Tab/NPC/List"));
  CEGUI::ListboxItem * lb_it = NULL;
  lb_it = mcl->findColumnItemWithText(ID_of_NPC_to_delete, 0, NULL);
  mcl->removeRow(mcl->getItemRowIndex(lb_it));
  //reset ID to empty string
  ID_of_NPC_to_delete = "";
  //delete window
  CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
  return true;
}

void EditorApplicationNPC::showNPCNewWindow(void)
{
  //not completely implemented yet
  CEGUI::FrameWindow* frame = NULL;
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New NPC...");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.275, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.4, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    //static text for gender
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Gender_Label");
    button->setText("Gender:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.525, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
    frame->addChildWindow(button);

    CEGUI::RadioButton* radio = NULL;
    //radio button for male
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioMale"));
    radio->setText("Male");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.525, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(true);
    radio->setGroupID(888);
    frame->addChildWindow(radio);
    //radio button for female
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioFemale"));
    radio->setText("Female");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(0.525, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    radio->setSelected(false);
    radio->setGroupID(888);
    frame->addChildWindow(radio);

    /* to do:
       ======
       - put more elements (for attributes and inventory) here
    */

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.8, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.38, 0), CEGUI::UDim(0.17, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.5, 0)));
  frame->moveToFront();
  //not completely implemented yet
}

bool EditorApplicationNPC::NPCNewFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame");
  }
  return true;
}

bool EditorApplicationNPC::NPCNewFrameOKClicked(const CEGUI::EventArgs &e)
{
  //not implemented yet
  return true;
}

} //namespace
