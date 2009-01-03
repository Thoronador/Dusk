#ifndef DUSKH
#define DUSKH

// This file includes all other headers in alphabetical order


/*
This Header defines the API Singelton class

API holds pointers to some important Ogre and Dusk Objects
you can get these Objects by calling getAPI().getSomething()
*/
#include "API.h"


/*
This Header defines the AnimatedObject class

provided for futur use
*/
#include "AnimatedObject.h"


/*
This Header defines the Application class

That's the main class, it's responsible for build up the Engine
You start the Enigne by creating an application object and call the initialse() function
*/
#include "Application.h"


/*
This Header defines the Camera class

That's a free flying Camera
doesn't work right now
*/
#include "Camera.h"


/*
This Header defines the abstract Command class

It provides the pure virtual function execute() for polymorpic use
*/
#include "Command.h"


/*
This Header defines the CommandAssociateSoundMedia and
 CommandDeassociateSoundMedia classes.

Their purpose is to associate or deassociate Sound's noises with media.
*/
#include "CommandAssociateSound.h"


/*
This Header defines the CommandCreateObject class

Creates an Object, register it in the Scene and loads a mesh
Not implemented yet
*/
#include "CommandCreateObject.h"


/*
This Header defines the CommandLoopSound class.

It sets a sound to infinite loop mode or can get it out of loop mode again,
depending on the given boolean value.
*/
#include "CommandLoopSound.h"

/*
This Header defines the CommandCreateMedia and CommandDestroyMedia classes.

They can create (i.e. load) sound media and destroy (i.e. free) sound media.
*/
#include "CommandMediaSound.h"


/*
This Header defines the CommandMove class

It Moves the free flying Camera
Not finished yet
*/
#include "CommandMove.h"


/*
This Header defines the CommandCreateNoise and CommandDestroyNoise classes.

They can create and destroy sound sources ("noises").
*/
#include "CommandNoiseSound.h"


/*
This Header defines the CommandPlaySound class

Its purpose is to handle sound playback commands.
It is a unification of the former CommandPlaySound, CommandPauseSound,
CommandUnPauseSound, CommandStopSound and CommandReplaySound classes.

*/
#include "CommandPlaySound.h"


/*
This Header defines the CommandQuit class

It is used to exit the engine
Doesn't work well, Programm crashes when CommandQuit is executed instead of shuting down regularly
*/
#include "CommandQuit.h"


/*
This Header defines the CommandSoundVolume class

It sets the volume values of sounds.
*/
#include "CommandSoundVolume.h"


/*
This Header defines the Console singelton class

This class translates the Input from the Inputsystem into Commands and send them to the Dispatcher
Works well but has to be extended with each new command type
*/
#include "Console.h"


/*
This Header defines the Dispatcher singelton class

The Dispatcher executes the Commands received from the Console
Works well
*/
#include "Dispatcher.h"


/*
This Header defines some global types
*/
#include "DuskTypes.h"


/*
This Header defines the FrameListener class

Cares for Timemanagment
*/
#include "FrameListener.h"


/*
This Header defines the InputEvent class

Unknown sense
erase it ?
*/
#include "InputEvent.h"


/*
This Header defines the InputSystem class

Holds the input binding and the input editor
It is used for sw�tching between binding and editor
*/
#include "InputSystem.h"


/*
This Header defines the InputSystemBinding singelton class

Handles the normal Input while playing
Works well but has to be extended with each new Command type
*/
#include "InputSystemBinding.h"


/*
This Header defines the InputSystemEditor singelton class

Handles the Input while typing Command into the Console (not the Console class)
Works well
*/
#include "InputSystemEditor.h"


/*
This Header defines the Object class

It represents an Object in the Game
Not implemented yet
*/
#include "Object.h"


/*
This Header defines the Scene singelton class

It manages all Objects
Right now it only creates a small Scene
*/
#include "Scene.h"


/*
This Header defines the Script class

It unites some Commands
Works well
*/
#include "Script.h"


/*
This Header is currently empty

It shall be used for global settings
*/
#include "Settings.h"


/*
This Header defines the Sound class

It implements the sound system.
*/
#include "Sound.h"


/*
This Header defines the StaticObject class

It represents a StaticObject
Not implemented yet
*/
#include "StaticObject.h"


#endif
