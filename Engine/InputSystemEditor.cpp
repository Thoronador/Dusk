#include "InputSystemEditor.h"
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include "Console.h"
#include "LuaEngine.h"
#include "API.h"

namespace Dusk
{

const std::string InputSystemEditor::cPrompt = "> ";

InputSystemEditor::InputSystemEditor()
{
   Ogre::Root* root = getAPI().getOgreRoot();
   Ogre::SceneManager* scene = root->getSceneManagerIterator().getNext();
   root->addFrameListener(this);

   height = 0;

   // Create background rectangle covering the whole screen
   myBackgroundRect = new Ogre::Rectangle2D(true);
   myBackgroundRect->setCorners(-1, 1, 1, 1-height);
   myBackgroundRect->setMaterial("console/background");
   myBackgroundRect->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
   myBackgroundRect->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
   mySceneNode = scene->getRootSceneNode()->createChildSceneNode("#Console");
   mySceneNode->attachObject(myBackgroundRect);

   myTextbox = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
   myTextbox->setCaption("hello\nworld");
   myTextbox->setMetricsMode(Ogre::GMM_RELATIVE);
   myTextbox->setPosition(0,0);
   myTextbox->setParameter("font_name","Console");
   myTextbox->setParameter("colour_top","1 0.5 0");
   myTextbox->setParameter("colour_bottom","1 0.5 0");
   myTextbox->setParameter("char_height","0.029");

   myOverlay = Ogre::OverlayManager::getSingleton().create("Console");
   myOverlay->add2D((Ogre::OverlayContainer*)myTextbox);
   myOverlay->show();

   visible = false;
   myTextbox->hide();
   m_continue = true;
   mUseLua = false;
   mInputHistory.clear();
   mOutputHistory.clear();
   unsigned int i;
   for (i=0; i<myLinesPerPage; ++i)
   {
     mInputHistory.push_back("");
     mOutputHistory.push_back("");
   }
   mInputHistoryIndex = myLinesPerPage;
}

InputSystemEditor::~InputSystemEditor()
{
    /* Due to the deletion of some Ogre Singletons it may happen that some or
       all of the created objects were already destroyed and the pointers are
       just dangling pointers. That's why we have to check for presence of
       singletons first and destroy afterwards.
    */
    if (myBackgroundRect->getParentSceneNode()!=NULL)
    {
      myBackgroundRect->getParentSceneNode()->detachObject(myBackgroundRect);
    }
    if (Ogre::HardwareBufferManager::getSingletonPtr()!=NULL)
    {
      delete myBackgroundRect;
    }
    myBackgroundRect = NULL;
    if (Ogre::Root::getSingletonPtr()!=NULL and
        Ogre::Root::getSingleton().getSceneManagerIterator().getNext()!=NULL)
    {
      delete mySceneNode;
    }
    mySceneNode = NULL;
    if (Ogre::OverlayManager::getSingletonPtr()!=NULL)
    {
      myOverlay->remove2D((Ogre::OverlayContainer*)myTextbox);
      Ogre::OverlayManager::getSingleton().destroyOverlayElement(myTextbox);
      myTextbox = NULL;
      Ogre::OverlayManager::getSingleton().destroy(myOverlay);
      myOverlay = NULL;
    }
    myTextbox = NULL;
    myOverlay = NULL;
}

bool InputSystemEditor::frameStarted(const Ogre::FrameEvent &evt)
{
    if(visible && height < 1){
      height += evt.timeSinceLastFrame * 2.0;
      myTextbox->show();
      if(height >= 1){
         height = 1;
      }
    }
    else if(!visible && height > 0){
      height -= evt.timeSinceLastFrame * 2.0;
      if(height <= 0){
         height = 0;
         myTextbox->hide();
      }
    }
    myTextbox->setPosition(0, (height - 1) * 0.5);
    myBackgroundRect->setCorners(-1, 1 + height, 1 , 1 - height);

    // now, put all contents on the textbox
    std::string contents = "";
    unsigned int i;
    for (i=0; i<mOutputHistory.size(); ++i)
    {
      contents += mOutputHistory.at(i)+"\n";
    } //for
    contents += cPrompt + myInputLine;
    myTextbox->setCaption(contents);
    return m_continue;
}

bool InputSystemEditor::frameEnded(const Ogre::FrameEvent &evt)
{
    return m_continue;
}

bool InputSystemEditor::keyPressed (const OIS::KeyEvent &arg)
{
    switch (arg.key)
    {
        case OIS::KC_F12:
            InputSystem::toggleInput();
            break;
        case OIS::KC_RETURN:
            if (myInputLine.size() > 0)
            {
              //check for built-in commands
              if (myInputLine=="noconsole" or myInputLine=="lua")
              {
                mUseLua = true;
                addToInput(myInputLine);
                addToOutput("Lua mode enabled.");
              }
              else if (myInputLine=="console" or myInputLine=="nolua")
              {
                mUseLua = false;
                addToInput(myInputLine);
                addToOutput("Console mode enabled.");
              }
              //dispatch input to Console or LuaEngine
              else
              {
                addToInput(myInputLine);
                if (mUseLua)
                  LuaEngine::getSingleton().addScript(Script(myInputLine));
                else
                  Console::getInstance()->addScript(Script(myInputLine));
              }
              myInputLine.clear();
              mInputHistoryIndex = myLinesPerPage;
            }
            break;
        case OIS::KC_BACK:
            if (myInputLine.size() > 0)
                myInputLine.erase(myInputLine.size() - 1);
            break;
        case OIS::KC_UP:
             if (mInputHistoryIndex>0)
               --mInputHistoryIndex;
             myInputLine = mInputHistory.at(mInputHistoryIndex);
             break;
        case OIS::KC_DOWN:
             if (mInputHistoryIndex<myLinesPerPage)
               ++mInputHistoryIndex;
             if (mInputHistoryIndex<mInputHistory.size())
             {
               myInputLine = mInputHistory.at(mInputHistoryIndex);
             }
             else
             {
               myInputLine = "";
             }
             break;
        default:
            const std::string legalchars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()<=>?[]{}\\*-_.:,; ";
            if (legalchars.find((char)arg.text) != std::string::npos)
                myInputLine += (char)arg.text;
    }
    return true;
}

bool InputSystemEditor::keyReleased (const OIS::KeyEvent &arg)
{
    return true;
}

bool InputSystemEditor::toggle()
{
    visible = !visible;
    return visible;
}
InputSystemEditor& InputSystemEditor::get()
{
    static InputSystemEditor Instance;
    return Instance;

}
bool InputSystemEditor::mouseMoved( const OIS::MouseEvent &arg )
{

    return true;
}
bool InputSystemEditor::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{

    return true;
}
bool InputSystemEditor::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{

    return true;
}

void InputSystemEditor::addToInput(const std::string& nl)
{
  if (!mInputHistory.empty())
    mInputHistory.pop_front();
  mInputHistory.push_back(nl);
  addToOutput(cPrompt+nl);
}

void InputSystemEditor::addToOutput(const std::string& nl)
{
  if (!mOutputHistory.empty())
    mOutputHistory.pop_front();
  mOutputHistory.push_back(nl);
}

void InputSystemEditor::addMessage(const std::string& txt)
{
  if (!txt.empty())
  {
    addToOutput(txt);
  }
}

} // namespace
