#include "EditorApplicationNPC.h"
#include <CEGUI/CEGUI.h>
#include "EditorApplicationBase.h"
#include "../Engine/DuskFunctions.h"
#include "../Engine/InjectionManager.h"
#include "../Engine/ItemBase.h"

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
  first = NPCBase::getSingleton().getFirst();
  const std::map<std::string, NPCRecord>::const_iterator end = NPCBase::getSingleton().getEnd();
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
  if (!NPCBase::getSingleton().deleteNPC(ID_of_NPC_to_delete))
  {
    showHint("NPCBase class holds no NPC of the given ID ("
             +ID_of_NPC_to_delete+").");
    //delete window
    CEGUI::WindowManager::getSingleton().destroyWindow("Editor/NPCDeleteFrame");
    ID_of_NPC_to_delete = "";
    return true;
  }
  //kill references
  const unsigned int refs_deleted = InjectionManager::getSingleton().deleteReferencesOfAnimatedObject(ID_of_NPC_to_delete);
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

    const float h = 2.5f/33.0f; //height of one element
    const float d = 0.5f/33.0f; //distance between two adjacent elements
    const float o = 2.0f/33.0f; //offset of first element

    CEGUI::Window * button = NULL;
    //static text for ID
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/ID_Label");
    button->setText("ID:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for ID
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/ID_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for name
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Name_Label");
    button->setText("Name:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for name
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Name_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+h+d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for mesh
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Mesh_Label");
    button->setText("Mesh:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //editbox for mesh
    button = winmgr.createWindow("TaharezLook/Editbox", "Editor/NPCNewFrame/Mesh_Edit");
    button->setText("");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+2*h+2*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for gender
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Gender_Label");
    button->setText("Gender:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    CEGUI::RadioButton* radio = NULL;
    //radio button for male
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioMale"));
    radio->setText("Male");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(true);
    radio->setGroupID(888);
    frame->addChildWindow(radio);
    //radio button for female
    radio = static_cast<CEGUI::RadioButton*> (winmgr.createWindow("TaharezLook/RadioButton", "Editor/NPCNewFrame/RadioFemale"));
    radio->setText("Female");
    radio->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65, 0), CEGUI::UDim(o+3*h+3*d, 0)));
    radio->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    radio->setSelected(false);
    radio->setGroupID(888);
    frame->addChildWindow(radio);

    //static text for Attributes
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Attributes_Label");
    button->setText("Attributes");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Inventory
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Inventory_Label");
    button->setText("Inventory");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+4*h+4*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //static text for Str
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Str_Label");
    button->setText("Str.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Strength
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Str_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Level
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Lvl_Label");
    button->setText("Level:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Level
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Level_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Agility
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Agi_Label");
    button->setText("Agi.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Agility
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Agi_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Willpower
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Will_Label");
    button->setText("Will:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Willpower
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Will_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+6*h+6*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Vitality
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Vit_Label");
    button->setText("Vit.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Vitality
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Vit_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Charisma
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Cha_Label");
    button->setText("Cha.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Charisma
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Cha_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+7*h+7*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Intelligence
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Int_Label");
    button->setText("Int.:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Intelligence
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Int_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.175, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //static text for Luck
    button = winmgr.createWindow("TaharezLook/StaticText", "Editor/NPCNewFrame/Luck_Label");
    button->setText("Luck:");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    frame->addChildWindow(button);

    //spinner for Luck
    spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/Luck_Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425, 0), CEGUI::UDim(o+8*h+8*d, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(h, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMaximumValue(255.0f);
    spin->setMinimumValue(1.0f);
    frame->addChildWindow(spin);

    //multi column list for inventory content
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
        (winmgr.createWindow("TaharezLook/MultiColumnList", "Editor/NPCNewFrame/InventoryList"));
    mcl->setPosition(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(o+5*h+5*d, 0)));
    mcl->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(4*h+3*d, 0)));
    mcl->addColumn("#", 0, CEGUI::UDim(0.22, 0));
    mcl->addColumn("Item ID", 1, CEGUI::UDim(0.72, 0));
    mcl->setUserColumnDraggingEnabled(false);
    mcl->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameInventoryListClicked, this));
    frame->addChildWindow(mcl);
    //adds a popup menu + callbacks for clicks
    CreatePopupMenuNPCNewFrameList();

    //OK button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/OK");
    button->setText("OK");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameOKClicked, this));
    frame->addChildWindow(button);
    //Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/Cancel");
    button->setText("Cancel");
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(o+9*h+9*d, 0)));
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(h, 0)));
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::NPCNewFrameCancelClicked, this));
    frame->addChildWindow(button);
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.38, 0), CEGUI::UDim(0.125, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.75, 0)));
  frame->moveToFront();
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
  //not completely implemented yet
  //getting inventory data is not implemented yet
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/ID_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Name_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Mesh_Edit") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/RadioFemale") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Str_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Level_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Agi_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Will_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Vit_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Cha_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Int_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/Luck_Spin") and
      winmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList"))
  {
    const std::string NPC_ID = winmgr.getWindow("Editor/NPCNewFrame/ID_Edit")->getText().c_str();
    const std::string NPC_Name = winmgr.getWindow("Editor/NPCNewFrame/Name_Edit")->getText().c_str();
    const std::string NPC_Mesh = winmgr.getWindow("Editor/NPCNewFrame/Mesh_Edit")->getText().c_str();
    if (NPC_ID=="" or NPC_Name=="" or NPC_Mesh == "")
    {
      showHint("You have to enter an ID, Name and Mesh path for the NPC.");
      return true;
    }
    if (NPCBase::getSingleton().hasNPC(NPC_ID))
    {
      showHint("An NPC with the ID \""+NPC_ID+"\" already exists. Please "
              +"choose a different ID or delete the other NPC first.\n");
      return true;
    }
    const uint8 level = static_cast<uint8>((static_cast<CEGUI::Spinner*>(
          winmgr.getWindow("Editor/NPCNewFrame/Level_Spin")))->getCurrentValue());
    NPCAttributes attr;
    attr.Str = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Str_Spin")))->getCurrentValue());
    attr.Agi = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Agi_Spin")))->getCurrentValue());
    attr.Vit = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Vit_Spin")))->getCurrentValue());
    attr.Int = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Int_Spin")))->getCurrentValue());
    attr.Will = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Will_Spin")))->getCurrentValue());
    attr.Cha = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Cha_Spin")))->getCurrentValue());
    attr.Luck = static_cast<uint8>((static_cast<CEGUI::Spinner*>(winmgr.getWindow("Editor/NPCNewFrame/Luck_Spin")))->getCurrentValue());
    const bool female = (static_cast<CEGUI::RadioButton*>(winmgr.getWindow("Editor/NPCNewFrame/RadioFemale")))->isSelected();
    const Inventory tempInv = MCLToInventory(static_cast<CEGUI::MultiColumnList*>(winmgr.getWindow("Editor/NPCNewFrame/InventoryList")));
    NPCBase::getSingleton().addNPC(NPC_ID, NPC_Name, NPC_Mesh, level, attr,
                                   female, tempInv, NPCAnimations::getNullAnimations(),
                                   NPCTagPoints::getNullTagPoints());
    winmgr.destroyWindow("Editor/NPCNewFrame");
    RefreshNPCList();
  }//if
  else
  {
    showHint("Error: At least one required CEGUI window element is not present.\n");
  }
  //not completely implemented yet (inventory data)
  return true;
}

void EditorApplicationNPC::CreatePopupMenuNPCNewFrameList(void)
{
  CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
  //PopUp Menu for inventory list
  if (wmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList/PopUp"))
  {
    return;
  }
  CEGUI::PopupMenu* popup = static_cast<CEGUI::PopupMenu*> (wmgr.createWindow("TaharezLook/PopupMenu", "Editor/NPCNewFrame/InventoryList/PopUp"));
  popup->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.3, 0)));
  popup->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.3, 0)));
  CEGUI::MenuItem* menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Add"));
  menu_item->setText("Add item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListAddClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Edit"));
  menu_item->setText("Edit selected item...");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListEditClicked, this));
  popup->addItem(menu_item);
  menu_item = static_cast<CEGUI::MenuItem*> (wmgr.createWindow("TaharezLook/MenuItem", "Editor/NPCNewFrame/InventoryList/PopUp/Delete"));
  menu_item->setText("Delete selected item");
  menu_item->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&EditorApplicationNPC::InventoryListDeleteClicked, this));
  popup->addItem(menu_item);
  wmgr.getWindow("Editor/NPCNewFrame/InventoryList")->addChildWindow(popup);
  popup->closePopupMenu();
}

bool EditorApplicationNPC::NPCNewFrameInventoryListClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::PopupMenu * popup = static_cast<CEGUI::PopupMenu*> (winmgr.getWindow("Editor/NPCNewFrame/InventoryList/PopUp"));
  if (!popup->isPopupMenuOpen())
  {
    const CEGUI::MouseEventArgs& mea = static_cast<const CEGUI::MouseEventArgs&> (e);
    if (mea.button == CEGUI::RightButton)
    {
      const CEGUI::Rect mcl_rect = winmgr.getWindow("Editor/NPCNewFrame/InventoryList")->getPixelRect();
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

bool EditorApplicationNPC::InventoryListAddClicked(const CEGUI::EventArgs &e)
{
  showInventoryListAddWindow();
  return true;
}

bool EditorApplicationNPC::InventoryListEditClicked(const CEGUI::EventArgs &e)
{
  //not implemented yet
  //not implemented yet
  return true;
}

bool EditorApplicationNPC::InventoryListDeleteClicked(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                         (winmgr.getWindow("Editor/NPCNewFrame/InventoryList"));
  CEGUI::ListboxItem* lbi = mcl->getFirstSelectedItem();
  if (lbi==NULL)
  {
    showHint("You have to select an item before you can delete it.");
    return true;
  }
  mcl->removeRow(mcl->getItemRowIndex(lbi));
  return true;
}

void EditorApplicationNPC::showInventoryListAddWindow(void)
{
  //not implemented yet (adjust size and position of child windows)
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  CEGUI::FrameWindow* frame;
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame"));
  }
  else
  {
    //create frame and child windows
    frame = static_cast<CEGUI::FrameWindow*> (winmgr.createWindow("TaharezLook/FrameWindow", "Editor/NPCNewFrame/AddInventoryFrame"));
    frame->setTitleBarEnabled(true);
    frame->setText("New NPC > Add items to inventory");
    frame->setCloseButtonEnabled(false);
    frame->setFrameEnabled(true);
    frame->setSizingEnabled(true);
    frame->setInheritsAlpha(false);
    winmgr.getWindow("Editor/Root")->addChildWindow(frame);
    //spinner for items
    CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.createWindow("TaharezLook/Spinner", "Editor/NPCNewFrame/AddInventoryFrame/Spin"));
    spin->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.2, 0)));
    spin->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.2, 0)));
    spin->setTextInputMode(CEGUI::Spinner::Integer);
    spin->setText("1");
    spin->setMinimumValue(1.0f);
    spin->setStepSize(1.0f);
    frame->addChildWindow(spin);
    //list box for items
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*> (winmgr.createWindow("TaharezLook/Combobox", "Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox"));
    CBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.55, 0), CEGUI::UDim(0.5, 0)));
    CBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.2, 0)));
    CBox->setReadOnly(true);
    frame->addChildWindow(CBox);
    UpdateItemList(CBox);
    //create Add button
    CEGUI::Window* button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/AddInventoryFrame/Add");
    button->setText("Add");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.5, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::AddInventoryFrameAddClicked, this));
    //create Cancel button
    button = winmgr.createWindow("TaharezLook/Button", "Editor/NPCNewFrame/AddInventoryFrame/Cancel");
    button->setText("Cancel");
    button->setSize(CEGUI::UVector2(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.15, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05, 0), CEGUI::UDim(0.75, 0)));
    frame->addChildWindow(button);
    button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&EditorApplicationNPC::AddInventoryFrameCancelClicked, this));
  }
  frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
  frame->setSize(CEGUI::UVector2(CEGUI::UDim(0.4, 0), CEGUI::UDim(0.3, 0)));
  frame->moveToFront();
}

void EditorApplicationNPC::UpdateItemList(CEGUI::Combobox* combo)
{
  if (combo!=NULL)
  {
    combo->resetList();
    CEGUI::ListboxItem* lbi = NULL;
    std::map<std::string, ItemRecord>::const_iterator itemFirst = ItemBase::getSingleton().getFirst();
    const std::map<std::string, ItemRecord>::const_iterator itemEnd = ItemBase::getSingleton().getEnd();
    while (itemFirst!=itemEnd)
    {
      lbi = new CEGUI::ListboxTextItem(itemFirst->first);
      lbi->setTooltipText(itemFirst->second.Name);
      combo->addItem(lbi);
      ++itemFirst;
    }//while
  }
}

bool EditorApplicationNPC::AddInventoryFrameAddClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame/Spin") &&
      winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox") &&
      winmgr.isWindowPresent("Editor/NPCNewFrame/InventoryList"))
  {
    //add stuff
    CEGUI::Combobox* CBox = static_cast<CEGUI::Combobox*>
      (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame/ItemCombobox"));
    const CEGUI::ListboxItem* lbi = CBox->getSelectedItem();
    if (lbi==NULL)
    {
      showHint("You have to select an item from the list. If you don't see any"
              +std::string(" items there, you haven't created any items yet.")
              +" Go to the item tab in the catalogue and create one.");
      return true;
    }
    const CEGUI::Spinner* spin = static_cast<CEGUI::Spinner*> (winmgr.getWindow("Editor/NPCNewFrame/AddInventoryFrame/Spin"));
    CEGUI::MultiColumnList* mcl = static_cast<CEGUI::MultiColumnList*>
                (winmgr.getWindow("Editor/NPCNewFrame/InventoryList"));
    //add it to catalogue
    unsigned int row;
    CEGUI::ListboxItem* lbi2 = new CEGUI::ListboxTextItem(IntToString(
                          static_cast<unsigned int>(spin->getCurrentValue())));
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    row = mcl->addRow(lbi2, 0);
    lbi2 = new CEGUI::ListboxTextItem(lbi->getText());
    lbi2->setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush");
    mcl->setItem(lbi2, 1, row);
    //destroy window
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
    return true;
  }
  return true;
}

bool EditorApplicationNPC::AddInventoryFrameCancelClicked(const CEGUI::EventArgs &e)
{
  CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton();
  if (winmgr.isWindowPresent("Editor/NPCNewFrame/AddInventoryFrame"))
  {
    winmgr.destroyWindow("Editor/NPCNewFrame/AddInventoryFrame");
  }
  return true;
}

Inventory EditorApplicationNPC::MCLToInventory(const CEGUI::MultiColumnList* mcl)
{
  Inventory temp;
  temp.makeEmpty();
  if (mcl!=NULL)
  {
    if (mcl->getColumnCount()==2)
    {
      const unsigned int rows = mcl->getRowCount();
      unsigned int i;
      for (i=0; i<rows; ++i)
      {
        temp.addItem(mcl->getItemAtGridReference(CEGUI::MCLGridRef(i,1))->getText().c_str(),
          StringToInt(mcl->getItemAtGridReference(CEGUI::MCLGridRef(i,0))->getText().c_str(),0));
      }
    }
  }
  return temp;
}

} //namespace
