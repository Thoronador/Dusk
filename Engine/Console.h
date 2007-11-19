#ifndef CONSOLE_H
#define CONSOLE_H
#include "Dispatcher.h"
namespace Dusk
{
    class Console
    {
    public:
        Console();
        virtual ~Console();
    protected:
    private:
        Dusk::Dispatcher* m_Dispatcher;
    };
}

#endif // CONSOLE_H
