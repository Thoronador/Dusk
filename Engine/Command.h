/*---------------------------------------------------------------------------
 Author:  ssj5000, walljumper
 Date:    2008-02-28
 Purpose: Command class
          basic class for Console commands

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2007-12-30 (rev 17)  - member count and virtual execute() added
                              (by walljumper)
     - 2008-02-28 (rev 48)  - non-virtual execute() added (by ssj5000)
     - 2010-05-06 (rev 197) - documentation update (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs.
 --------------------------------------------------------------------------*/

#ifndef COMMAND_H
#define COMMAND_H

namespace Dusk
{
    class Scene;
    class Command
    {
    public:
        /* constructor */
        Command();
        /* destructor */
        virtual ~Command();
        void execute();

        /* pure virtual function for executing command

           remarks:
               Every derived class has to have its own implementation of that
               function. This one only provides the common interface.
        */
        virtual bool execute(Dusk::Scene* scene, int count = 1) = 0;
    protected:
        int m_Count; // contains the number how many times the Command shall be executed
    };
}

#endif // COMMAND_H
