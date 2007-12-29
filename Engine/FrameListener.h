#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H
#include <Ogre.h>

namespace Dusk
{

    class FrameListener : public Ogre::FrameListener
    {
    public:
        FrameListener();
        virtual ~FrameListener();
        virtual bool frameStarted(const Ogre::FrameEvent& evt);
        virtual bool frameEnded(const Ogre::FrameEvent& evt);
    protected:
    private:
        //local variables
        bool m_Continue;
    };
}


#endif // FRAMELISTENER_H
