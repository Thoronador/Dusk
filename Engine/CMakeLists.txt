# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

project(Dusk)

set(Dusk_sources API.cpp Application.cpp Camera.cpp Celestial.cpp DataLoader.cpp
                 Dialogue.cpp DiceBox.cpp DuskExceptions.cpp DuskFunctions.cpp
                 FrameListener.cpp InjectionManager.cpp InputSystem.cpp InputSystemBinding.cpp
                 InputSystemEditor.cpp Inventory.cpp Journal.cpp Landscape.cpp
                 Menu.cpp Messages.cpp ObjectManager.cpp QuestLog.cpp Scene.cpp
                 Script.cpp Settings.cpp Sun.cpp Trigger.cpp
                 TriggerManager.cpp VertexDataFunc.cpp Weather.cpp
                 console/Command.cpp console/CommandAssociateSound.cpp
                 console/CommandAttack.cpp console/CommandBindKey.cpp
                 console/CommandLoopSound.cpp console/CommandMediaSound.cpp
                 console/CommandMove.cpp console/CommandNoiseSound.cpp
                 console/CommandPickUp.cpp console/CommandPlaySound.cpp
                 console/CommandQuestLog.cpp console/CommandQuit.cpp
                 console/CommandScreenshot.cpp console/CommandSoundVolume.cpp
                 console/CommandWeather.cpp console/CommandZoom.cpp
                 console/Console.cpp console/Dispatcher.cpp
                 database/ContainerRecord.cpp database/Database.cpp
                 database/ItemRecord.cpp database/LightRecord.cpp
                 database/NPCRecord.cpp database/ObjectRecord.cpp
                 database/ProjectileRecord.cpp database/ResourceRecord.cpp
                 database/SoundRecord.cpp database/VehicleRecord.cpp
                 database/WeaponRecord.cpp
                 lua/LuaBindingsAnimated.cpp lua/LuaBindingsNPC.cpp
                 lua/LuaBindingsObject.cpp lua/LuaBindingsQuestLog.cpp
                 lua/LuaBindingsSound.cpp lua/LuaBindingsUniformMotion.cpp
                 lua/LuaBindingsWeather.cpp lua/LuaEngine.cpp
                 objects/AnyConversion.cpp
                 objects/AnimatedObject.cpp objects/Container.cpp
                 objects/DuskObject.cpp objects/InjectionObject.cpp objects/Item.cpp
                 objects/Light.cpp objects/NPC.cpp objects/Player.cpp
                 objects/Projectile.cpp objects/Resource.cpp objects/UniformMotionObject.cpp
                 objects/Vehicle.cpp objects/WaypointObject.cpp objects/Weapon.cpp
                 sound/Media.cpp sound/MediaOggVorbis.cpp sound/MediaWave.cpp
                 sound/Sound.cpp sound/Source.cpp
                 main.cpp)

message ( "CMAKE_CXX_COMPILER is set to ${CMAKE_CXX_COMPILER}." )

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

add_definitions(`pkg-config --cflags OGRE`)

execute_process(COMMAND pkg-config --libs OGRE OUTPUT_VARIABLE OGRE_PKC)

add_definitions(${OGRE_PKC})

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s ${OGRE_PKC}" )

add_executable(Dusk ${Dusk_sources})

# OpenGL
find_package (OpenGL)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIRS})
  target_link_libraries (Dusk ${OPENGL_LIBRARIES})
else ()
  message ( FATAL_ERROR "OpenGL was not found!" )
endif (OPENGL_FOUND)

# OpenAL
find_package (OpenAL)
if (OPENAL_FOUND)
  include_directories(${OPENAL_INCLUDE_DIRS})
  target_link_libraries (Dusk ${OPENAL_LIBRARIES})
else ()
  message ( FATAL_ERROR "OpenAL was not found!" )
endif (OPENAL_FOUND)

# Lua
find_package (Lua51)
if (LUA51_FOUND)
  include_directories(${LUA51_INCLUDE_DIRS})
  target_link_libraries (Dusk ${LUA51_LIBRARIES})
else ()
  message ( FATAL_ERROR "Lua 5.1 was not found!" )
endif (LUA51_FOUND)

# OIS
find_package (OIS)
if (OIS_FOUND)
  include_directories(${OIS_INCLUDE_DIRS})
  target_link_libraries (Dusk ${OIS_LIBRARIES})
else ()
  message ( FATAL_ERROR "OIS was not found!" )
endif (OIS_FOUND)

# Ogre
#find_package (OGRE)
#if (OGRE_FOUND)
#  include_directories(${OGRE_INCLUDE_DIRS})
#  target_link_libraries (Dusk ${OGRE_LIBRARIES})
#else ()
#  message ( FATAL_ERROR "OGRE was not found!" )
#endif (OGRE_FOUND)
