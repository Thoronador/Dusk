#include "Dispatcher.h"
#include "API.h"
namespace Dusk
{

Dispatcher::Dispatcher()
{
    //ctor
    getAPI().setDispatcher(this);
}

Dispatcher::~Dispatcher()
{
    //dtor
}

}
