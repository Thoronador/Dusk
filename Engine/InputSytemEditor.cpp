#include "InputSystemEditor.h"

namespace Dusk
{

InputSystemEditor::InputSystemEditor(Ogre::Root *root)
{
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
   myTextbox->setParameter("colour_top","1 1 1");
   myTextbox->setParameter("colour_bottom","1 1 1");
   myTextbox->setParameter("char_height","0.03");

   myOverlay = Ogre::OverlayManager::getSingleton().create("Console");
   myOverlay->add2D((Ogre::OverlayContainer*)myTextbox);
   myOverlay->show();

   visible = false;
}

InputSystemEditor::~InputSystemEditor()
{
    delete myBackgroundRect;
    delete mySceneNode;
    delete myTextbox;
    delete myOverlay;
}

Dusk::Script* InputSystemEditor::handleEvent(Dusk::InputEvent inputEvent)
{
    return 0;
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
   return true;
}

bool InputSystemEditor::frameEnded(const Ogre::FrameEvent &evt)
{
    return true;
}

bool InputSystemEditor::keyPressed (const OIS::KeyEvent &arg)
{
    switch(arg.key)
    {
        case OIS::KC_GRAVE :
        {
            InputSystem::toggleInput();
            break;
        }
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

} // namespace
