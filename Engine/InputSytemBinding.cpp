#include "InputSystemBinding.h"

namespace Dusk{

InputSystemBinding::InputSystemBinding()
{

}

InputSystemBinding::~InputSystemBinding()
{

}

Dusk::Script* InputSystemBinding::handleEvent(Dusk::InputEvent inputEvent)
{
    Script* script = 0;
    std::map<Dusk::InputEvent, Dusk::Script>::iterator it = myBindList.find(inputEvent);
    if (it != myBindList.end())
        script = new Dusk::Script(it->second);
    return script;
}

} // end namespace
