#ifndef INPUTEVENT_H_INCLUDED
#define INPUTEVENT_H_INCLUDED

namespace Dusk
{
    /**
     * This class holds an internal input event that can be processes by
     * any InputSystem.
     */
    class InputEvent {
    public:

        /**
         * Standard constructor
         */
        InputEvent();

        /**
         * Copy constructor
         *
         * @param inputEvent    Input event from which to make the copy.
         */
        InputEvent(InputEvent& inputEvent);

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputEvent();

        /**
         * Compare operator.
         *
         * @param inputEvent    Input event to compare itself with.
         * @return              True if the given input event preceeds this event.
         */
        bool operator<(const InputEvent& inputEvent) const;


        /**
         * Assignment operator.
         *
         * @param inputEvent    Input event to assing.
         * @return              The assigned input event.
         */
        InputEvent& operator=(const InputEvent& inputEvent);
    };
}

#endif // INPUTEVENT_H_INCLUDED
