/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 walljumper, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
status:
    works
*/
#include "API.h"


/*
This Header defines the AnimatedObject class

description:
    The class represents an animated object in the game. This
    could be something like a flag waving in the wind.
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
This Header defines the abstract Celestial class.

description:
    This class serves as an interface which all celestial objects will
    have to implement.
status:
    Works, I guess. :D
*/
#include "Celestial.h"


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
This Header defines the CommandAttack class.

description:
    Their purpose is to manage attacks of the player (i.e. starts or stops
    them).
*/
#include "CommandAttack.h"


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
    It moves the free flying Camera.
status:
    works, but not completely finished yet
*/
#include "CommandMove.h"


/*
This Header defines the CommandCreateNoise and CommandDestroyNoise classes.

description:
    They can create and destroy sound sources ("noises").
*/
#include "CommandNoiseSound.h"


/*
This Header defines the CommandPickUp class

description:
    Its purpose is to allow the player to pick up items.
status:
    works
*/
#include "CommandPickUp.h"


/*
This Header defines the CommandPlaySound class

description:
    Its purpose is to handle sound playback commands.
    It is a unification of the former CommandPlaySound, CommandPauseSound,
    CommandUnPauseSound, CommandStopSound and CommandReplaySound classes.
*/
#include "CommandPlaySound.h"


/*
This Header defines the CommandQuestLog class.

description:
    It is used to switch visibility of the QuestLog.
status:
    Seems to work.
*/
#include "CommandQuestLog.h"


/*
This Header defines the CommandQuit class.

description:
    It is used to exit the engine.
status:
    Seems to work.
*/
#include "CommandQuit.h"


/*
This Header defines the CommandScreenshot class.

description:
    It is used to take screenshots.
status:
    Seems to work.
*/
#include "CommandScreenshot.h"


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
This Header defines the CommandZoom class.

description:
    Zooms the camera in and out.
*/
#include "CommandZoom.h"


/*
This Header defines the Console singelton class

description:
    This class translates the Input from the InputSystem into Commands
    and send them to the Dispatcher.
status:
    Works well, but has to be extended with each new command type
*/
#include "Console.h"


/*
This header defines the Container class.

description:
    Represents a container within the game (i.e. anything static,
    which can be used to store items, like a box).
status:
    untested/not used yet
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
status:
    works
*/
#include "FrameListener.h"


/*
This Header defines the InjectionManager Singleton class

description:
    The class holds/manages all animated objects in the game and
    makes sure they all get their animations played properly by
    injecting the animation time each frame.
status:
    seems to work
*/
#include "InjectionManager.h"


/*
This Header defines the InjectionObject class

description:
    This is an abstract class which serves as interface for all
    kinds of objects that need to be regularly updated, i.e.
    animated and moving objects..
status:
    testing, but seems to work
*/
#include "InjectionObject.h"


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
This Header defines the Inventory class.

description:
    This class represents the inventory of an NPC or a container
    within the game.
status:
    works
*/
#include "Inventory.h"

/*
This Header defines the Item class.

description:
    This class represents an item in the game. Items can be picked up by
    the player character.

status:
    seems to work, but not fully tested yet
*/
#include "Item.h"

/*
This Header defines the ItemBase Singleton class.

description:
    This class represents a sort of look-up table for all distinct items
    in the game.
status:
    seems to work
*/
#include "ItemBase.h"


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
This header defines the LightBase singleton class.

description:
    Holds information about all distinct lights in the game
    Think of it as a sort of look-up table for lights.
status:
    untested
*/
#include "LightBase.h"


/*
This Header defines the LuaEngine singleton class.

description:
    This class will run all Lua code/ scripts in the game.
status:
    works, but could need more testing and improvement
*/
#include "LuaEngine.h"


/*
This Header wraps all the #includes needed for Lua.

description:
    "Wrapper" for Lua-specific inludes; manages differences
    between Lua 5.0 and 5.1.
status:
    works
*/
#include "LuaIncludes.h"


/*
This Header defines the Menu Singleton class.

description:
    Manages display of dialogue menues.
status:
    untested
    still needs A LOT more features than in its current state
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
    creation, i.e. look-up table for NPCs.
status:
    untested, but I don't expect to many problems in its current state
*/
#include "NPCBase.h"

/*
This Header defines the ObjectBase Singelton class.

description:
    Holds information about all distinct (static) objects in the game;
    look-up table for DuskObjects.
status:
    works
*/
#include "ObjectBase.h"


/*
This Header defines the ObjectManager Singelton class.

description:
    Holds all static objects in the game.
status:
    works
*/
#include "ObjectManager.h"


/*
This Header defines the Player singleton class.

description:
    This class represents the player character in the game.
status:
    The player isn't displayed yet, Enable() works,
    player moves around with the camera.
    Needs still more work.
*/
#include "Player.h"


/*
This Header defines the Projectile class.

description:
    This class represents a single projectile in the game.
status:
    untested / currently not used
    The projectile doesn't perform collision detection yet.
    Thus, it also cannot inflict damage yet.
*/
#include "Projectile.h"


/*
This Header defines the ProjectileBase Singleton class.

description:
    Holds information about distinct projectile types in the game.
    (Look-up table...)
status:
    untested.
*/
#include "ProjectileBase.h"


/*
This Header defines the QuestLog singelton class.

description:
    It manages the player's quest entries.
status:
    untested
*/
#include "QuestLog.h"


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
    Holds a script that can be executed via Dispatcher/Console.
status:
    Works well
*/
#include "Script.h"


/*
This Header defines the Settings singleton class.

description:
    The Singleton class can store different named settings (integer,
    string and float values). It shall be used for global settings.
status:
    needs more testing
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
This Header defines the Sun class.

description:
    This class implements the behaviour of the sun in-game.
status:
    Works, but far from perfect. (It's just white during foggy weather.)
*/
#include "Sun.h"


/*
This Header defines the Trigger interface class, the AABoxTrigger class
and the SphereTrigger class.

description:
    The Trigger class provides a basic interface for triggers.

    The AABoxTrigger class is derived from trigger and implements some
    methods for a trigger with an axis aligned box as trigger area.

    The SphereTrigger class is derived from trigger and implements some
    methods for a trigger with spheric area.
status:
    not tested
*/
#include "Trigger.h"


/*
This Header defines the TriggerManager Singleton class.

description:
    The TriggerManager holds all triggers in the game.
status:
    not tested, needs load/save routines
*/
#include "TriggerManager.h"


/*
This Header defines the UniformMotionObject class

description:
    The class represents a moving object in the game, which moves
    straight to a certain point (and then stops) or into a certain
    direction at constant speed.
status:
    testing, but seems to work
*/
#include "UniformMotionObject.h"


/*
This Header defines the Vehicle class.

description:
    The class represents a Vehicle in the game, which can carry some
    NPCs
status:
    under development (ha! That's a good one in a game engine that
    still is way before maturity, huh? ;))
    untested
*/
#include "Vehicle.h"


/*
This Header defines the VehicleBase Singleton class.

description:
    Holds information about distinct vehicle types in the game.
    (Look-up table...)
status:
    untested.
*/
#include "VehicleBase.h"


/*
This Header defines two functions that return the vertex data of a mesh.

description:
    Functions that return the vertex data of a mesh; used for retrieving data
    for collision detections.
    The code of VertexDataFunc.h was take from the Ogre Wiki (see file for
    exact location/ URL) and is not licensed under GPL but is in the public
    domain instead.
status:
    Works.
*/
#include "VertexDataFunc.h"


/*
This Header defines the WaypointObject class

description:
    The class represents a moving object in the game, which moves
    between some predefined waypoints. This could be something like
    a guard on patrol.
status:
    testing, but seems to work
*/
#include "WaypointObject.h"


/*
This Header defines the Weapon class

description:
    The class represents a weapon object in the game, which can be
    used to attack NPCs.
status:
    not finished yet
*/
#include "Weapon.h"


/*
This Header defines the WeaponBase singleton class

description:
    The class holds information about all distinct weapons within the game.
    Think of it as a sort of look-up table.
status:
    not finished yet
*/
#include "WeaponBase.h"


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
