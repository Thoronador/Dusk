#ifndef DUSKH
#define DUSKH

// This file includes all other headers in alphabetical order
/* ****needs to be updated****
    -add all currently available headers to the list
    -update descriptions of present classes, where neccessary
    -update status of classes, where neccessary
*/


/*
This Header defines the API Singelton class.

description:
    API holds pointers to some important Ogre and Dusk Objects
    you can get these Objects by calling getAPI().getSomething()
*/
#include "API.h"


/*
This Header defines the AnimatedObject class

description:
    The class represents an animated and possibly moving object in
    the game. This could be something like a flag waving in the wind
    or a rolling barrel.
status:
    testing, but seems to work
*/
#include "AnimatedObject.h"


/*
This Header defines the Application class

description:
    That's the main class, it's responsible for build up the Engine.
    You start the Enigne by creating an application object and call
    the initialse() function.
*/
#include "Application.h"


/*
This Header defines the Camera class

description:
    That's a free flying Camera.
status:
    works, but might be extended in future
*/
#include "Camera.h"


/*
This Header defines the abstract Command class

description:
    It provides the pure virtual function execute() for polymorpic use
*/
#include "Command.h"


/*
This Header defines the CommandAssociateSoundMedia and
 CommandDeassociateSoundMedia classes.

description:
    Their purpose is to associate or deassociate Sound's noises with media.
*/
#include "CommandAssociateSound.h"


/*
This Header defines the CommandCreateObject class

description:
    Creates an Object, register it in the Scene and loads a mesh
status:
    Not implemented yet
*/
#include "CommandCreateObject.h"


/*
This Header defines the CommandLoopSound class.

description:
    It sets a sound to infinite loop mode or can get it out of loop mode again,
    depending on the given boolean value.
*/
#include "CommandLoopSound.h"

/*
This Header defines the CommandCreateMedia and CommandDestroyMedia classes.

description:
    They can create (i.e. load) sound media and destroy (i.e. free) sound media.
*/
#include "CommandMediaSound.h"


/*
This Header defines the CommandMove class

description:
    It Moves the free flying Camera
status:
    Not finished yet
*/
#include "CommandMove.h"


/*
This Header defines the CommandCreateNoise and CommandDestroyNoise classes.

description:
    They can create and destroy sound sources ("noises").
*/
#include "CommandNoiseSound.h"


/*
This Header defines the CommandPlaySound class

description:
    Its purpose is to handle sound playback commands.
    It is a unification of the former CommandPlaySound, CommandPauseSound,
    CommandUnPauseSound, CommandStopSound and CommandReplaySound classes.
*/
#include "CommandPlaySound.h"


/*
This Header defines the CommandQuit class

description:
    It is used to exit the engine
status:
    Seems to work.
*/
#include "CommandQuit.h"


/*
This Header defines the CommandSoundVolume class

description:
    It sets the volume values of sounds.
*/
#include "CommandSoundVolume.h"


/*
This Header defines the CommandWeather class.

description:
    Allows to toggle fog/rain/snow or turn these effects on and off.
status:
    works
*/
#include "CommandWeather.h"


/*
This Header defines the CommandZoom class

description:
    Zooms the camera in and out.
*/
#include "CommandZoom.h"


/*
This Header defines the Console singelton class

description:
    This class translates the Input from the Inputsystem into Commands and send them to the Dispatcher
status:
    Works well but has to be extended with each new command type
*/
#include "Console.h"


/*
This header defines the Container class.

description:
    Represents a container within the game (i.e. anything static,
    which can be used to store items, like a box).
status:
    untested
*/
#include "Container.h"


/*
This header defines the ContainerBase Singleton class.

description:
    Represents the look-up table for properties of all
    distinct containers in-game.
status:
    working (I guess so)
*/
#include "ContainerBase.h"


/*
This header defines the DataLoader singelton class.

description:
    The DataLoader provides an interface to load and save game data
    from/ to a file.
status:
    Works
*/
#include "DataLoader.h"


/*
This Header defines the Dialogue Singleton class.

description:
    This class manages the data of all dialogue lines in game.
status:
    untested
*/
#include "Dialogue.h"


/*
This Header defines the DiceBox Singelton class.

description:
    DiceBox provides functions to generate random numbers, or in game
    terms "throws a die" (or more).
status:
    works
*/
#include "DiceBox.h"


/*
This Header defines the Dispatcher singelton class

description:
    The Dispatcher executes the Commands received from the Console
status:
    Works well
*/
#include "Dispatcher.h"


/*
This Header defines some global constants, which are used among various
headers and source files.

status:
    constant ;)
*/
#include "DuskConstants.h"


/*
This Header defines some functions, which are used among various
headers and source files.

status:
    working
*/
#include "DuskFunctions.h"


/*
This Header defines the DuskObject class.

description:
    It represents a static Object in the Game.
    Classes for animated objects are (or will be) derived from this class.
status:
    Works well
*/
#include "DuskObject.h"


/*
This Header defines some global types.
*/
#include "DuskTypes.h"


/*
This Header defines the FrameListener class.

description:
    Cares for Timemanagment
*/
#include "FrameListener.h"


/*
This Header defines the InputEvent class.

description:
    Unknown purpose
status:
    erase it ?
*/
#include "InputEvent.h"


/*
This Header defines the InputSystem class

description:
    Holds the input binding and the input editor
    It is used for swítching between binding and editor.
*/
#include "InputSystem.h"


/*
This Header defines the InputSystemBinding singelton class

description:
    Handles the normal Input while playing
status:
    Works well, but has to be extended with each new Command type
*/
#include "InputSystemBinding.h"


/*
This Header defines the InputSystemEditor singelton class

description:
    Handles the Input while typing Command into the Console (not the Console class)
status:
    Works well
*/
#include "InputSystemEditor.h"


/*
This Header defines the Journal Singleton class.

description:
    Holds the data of all journal entries that can possibly occur in-game.
status:
    untested
*/
#include "Journal.h"


/*
This header defines the Landscape singleton class and the LandscapeRecord class.

description:
    Holds the landscape/ height data for the game.
status:
    works, but is not completely implemented yet
*/
#include "Landscape.h"


/*
This header defines the Light class.

description:
    Defines a single light source in the game.
status:
    untested
*/
#include "Light.h"


/*
This Header defines the Menu Singleton class.

description:
    Manages display of dialogue menues.
status:
    untested
*/
#include "Menu.h"


/*
This Header defines the NPC class.

description:
    Represents a NPC in the game
status:
    not working, under development
*/
#include "NPC.h"


/*
This Header defines the NPCBase Singelton class.

description:
    NPCBase holds information about the state of NPCs at the point of their
    creation.
status:
    untested, but I don't expect to many problems in its current state
*/
#include "NPCBase.h"

/*
This Header defines the Scene singelton class.

description:
    It manages all Objects.
status:
    Right now it only creates a small Scene with a few objects.
*/
#include "Scene.h"


/*
This Header defines the Script class.

description:
    It unites some Commands
status:
    Works well
*/
#include "Script.h"


/*
This Header is currently empty.

description:
    It shall be used for global settings.
status:
    empty (apart from the namespace)
*/
#include "Settings.h"


/*
This Header defines the Sound singleton class.

description:
    It implements the sound system, which can handle WAV and Ogg-Vorbis.
status:
    Seems to work fine (until proven otherwise)
*/
#include "Sound.h"


/*
This Header defines the Weather singleton class.

description:
    This class implements all weather-related stuff like rain, fog,
    sunshine, change between day and night, and so on.
status:
    fog works, other things still need to be done
*/
#include "Weather.h"


#endif
