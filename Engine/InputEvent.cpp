#include "InputEvent.h"

namespace Dusk
{

InputEvent::InputEvent()
{
}

InputEvent::~InputEvent()
{
}

InputEvent::InputEvent(InputEvent& inputEvent)
{
}

bool InputEvent::operator<(const InputEvent& inputEvent) const
{
    return true;
}

InputEvent& InputEvent::operator=(const InputEvent& inputEvent)
{
    return *this;
}

} // namespace
