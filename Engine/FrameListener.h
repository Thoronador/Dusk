#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H
#include <Ogre.h>
#include <OIS/OIS.h>
#include "Console.h"
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
        class InputHandler
        {
        public:
        protected:
        private:
            Dusk::Console* m_Console;
        };
    };
}


#endif // FRAMELISTENER_H
