/*---------------------------------------------------------------------------
 Author:  ssj5000, walljumper
 Date:    2008-02-22
 Purpose: Dispatcher Singleton class

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2008-01-17 (rev 23)  - dispatcher now registers itself at API
                              (by walljumper)
     - 2008-01-17 (rev 24)  - executeCommand() added
     - 2008-01-24 (rev 27)  - Dispatcher is now singleton
     - 2008-01-26 (rev 28)  - }
     - 2008-01-26 (rev 29)  - }  fixed some bugs
     - 2008-02-22 (rev 39)  - }
     - 2010-01-01 (rev 148) - documentation update

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Scene.h"
namespace Dusk
{
    class Command;
    class Dispatcher
    {
    public:
        static Dispatcher& get();
        virtual ~Dispatcher();
        bool executeCommand(Dusk::Command* command);
    protected:
    private:
        Dusk::Scene* m_Scene;
        Dispatcher();
        Dispatcher(const Dispatcher& op){}
    };
} // namespace

#endif // DISPATCHER_H
