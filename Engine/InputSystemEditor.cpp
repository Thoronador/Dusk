#include "InputSystemEditor.h"

#include "Console.h"
#include "LuaEngine.h"
#include "API.h"

namespace Dusk
{

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
   myOutput.clear();
   unsigned int i;
   for (i=0; i<myLinesPerPage; ++i)
   {
     myOutput.push_back("");
   }
   mInputHistoryIndex = myLinesPerPage;
}

InputSystemEditor::~InputSystemEditor()
{
    delete myBackgroundRect;
    delete mySceneNode;
    delete myTextbox;
    delete myOverlay;
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
    //myTextbox->setCaption("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n> " + myInputLine);
    std::string contents = "";
    unsigned int i;
    for (i=0; i<myOutput.size(); ++i)
    {
      contents += myOutput.at(i)+"\n";
    } //for
    contents += "> " + myInputLine;
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
              }
              else if (myInputLine=="console" or myInputLine=="nolua")
              {
                mUseLua = false;
              }
              //dispatch input to Console or LuaEngine
              else
              {
                if (mUseLua)
                  LuaEngine::GetSingleton().addScript(Script(myInputLine));
                else
                  Console::getInstance()->addScript(Script(myInputLine));
              }
              addToOutput(myInputLine);
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
             myInputLine = myOutput.at(mInputHistoryIndex);
             break;
        case OIS::KC_DOWN:
             if (mInputHistoryIndex<myLinesPerPage)
               ++mInputHistoryIndex;
             if (mInputHistoryIndex<myOutput.size())
             {
               myInputLine = myOutput.at(mInputHistoryIndex);
             }
             else
             {
               myInputLine = "";
             }
             break;
        default:
            const std::string legalchars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
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

void InputSystemEditor::addToOutput(const std::string& nl)
{
  if (!myOutput.empty())
    myOutput.pop_front();
  myOutput.push_back(nl);
}

} // namespace
