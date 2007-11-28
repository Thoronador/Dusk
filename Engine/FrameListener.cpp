#include "FrameListener.h"

namespace Dusk
{

FrameListener::FrameListener()
{
    //ctor
}

FrameListener::~FrameListener()
{
    //dtor
}

bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    return true;
}
bool FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

}

