#include "InputSystemEditor.h"

namespace Dusk
{

InputSystemEditor::InputSystemEditor(Ogre::Root *root)
{
   Ogre::SceneManager* scene = root->getSceneManagerIterator().getNext();
   root->addFrameListener(this);

   height=1;

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

   visible = true;
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
    if (visible)
    {
    }

    return true;
}

bool InputSystemEditor::frameEnded(const Ogre::FrameEvent &evt)
{
    return true;
}

bool InputSystemEditor::toggle()
{
    visible = !visible;
    return visible;
}

} // namespace
