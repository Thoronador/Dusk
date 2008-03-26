#include "InputSystemEditor.h"

#include "Console.h"
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
    m_continue = true;
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
    myTextbox->setCaption("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n> " + myInputLine);
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
        case OIS::KC_GRAVE:
            InputSystem::toggleInput();
            break;
        case OIS::KC_RETURN:
            if (myInputLine.size() > 0)
            {
                Console::getInstance()->addScript(Script(myInputLine));
                myInputLine.clear();
            }
            break;
        case OIS::KC_BACK:
            if (myInputLine.size() > 0)
                myInputLine.erase(myInputLine.size() - 1);
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

} // namespace
