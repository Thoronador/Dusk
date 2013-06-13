#!/bin/bash

# Build script for Dusk Engine

#  Copyright (C) 2013  Thoronador
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

E_FILE_ERROR=1
E_COMPILE_ERROR=2

obj_dir=obj/
src_dir=../
compiler_includes="`pkg-config --cflags OGRE` `pkg-config --cflags gl` `pkg-config --cflags OIS` `pkg-config --cflags lua50` `pkg-config --cflags lualib50` `pkg-config --cflags openal` `pkg-config --cflags vorbisfile`"

function compile_real()
{
  local cxx=$1
  local cxxflags=$2
  local src_file=$3
  
  echo "Compiling $src_file.cpp..."
  $cxx $cxxflags $src_dir$src_file.cpp -c -o $obj_dir$src_file.o
  if [[ $? -ne 0 ]]
  then
    echo "Error while compiling $src_file.cpp! Aborting."
    exit $E_COMPILE_ERROR
  fi
} # function compile

function compile()
{
  compile_real $CXX "-Wall -fexceptions -O3 $compiler_includes" $1
}


required_dirs=($obj_dir $obj_dir./console $obj_dir./database $obj_dir./lua $obj_dir./objects $obj_dir./sound)

for dir in `echo ${required_dirs[@]}`
do
  if [[ ! -d $dir ]]
  then
    mkdir $dir
    if [[ $? -ne 0 ]]
    then
      echo "Error while creating directory $dir for object files! Aborting."
      exit $E_FILE_ERROR
    fi
  fi
done

all_files=(API
Application
Camera
Celestial
DataLoader
Dialogue
DiceBox
DuskExceptions
DuskFunctions
FrameListener
InjectionManager
InputSystem
InputSystemBinding
InputSystemEditor
Inventory
Journal
Landscape
Menu
Messages
ObjectManager
QuestLog
Scene
Script
Settings
Sun
Trigger
TriggerManager
VertexDataFunc
Weather
console/Command
console/CommandAssociateSound
console/CommandAttack
console/CommandBindKey
console/CommandLoopSound
console/CommandMediaSound
console/CommandMove
console/CommandNoiseSound
console/CommandPickUp
console/CommandPlaySound
console/CommandQuestLog
console/CommandQuit
console/CommandScreenshot
console/CommandSoundVolume
console/CommandWeather
console/CommandZoom
console/Console
console/Dispatcher
database/ContainerRecord
database/Database
database/ItemRecord
database/LightRecord
database/NPCRecord
database/ObjectRecord
database/ProjectileRecord
database/ResourceRecord
database/SoundRecord
database/VehicleRecord
database/WeaponRecord
lua/LuaBindingsAnimated
lua/LuaBindingsNPC
lua/LuaBindingsObject
lua/LuaBindingsQuestLog
lua/LuaBindingsSound
lua/LuaBindingsUniformMotion
lua/LuaBindingsWeather
lua/LuaEngine
main
objects/AnimatedObject
objects/AnyConversion
objects/Container
objects/DuskObject
objects/InjectionObject
objects/Item
objects/Light
objects/NPC
objects/Player
objects/Projectile
objects/Resource
objects/UniformMotionObject
objects/Vehicle
objects/WaypointObject
objects/Weapon
sound/Media
sound/MediaOggVorbis
sound/MediaWave
sound/Sound
sound/Source)

total=${#all_files[@]}
declare -i total
current=0
declare -i current

echo "Info: $total element(s) listed for compilation."
echo

linker_libs="`pkg-config --libs OGRE` `pkg-config --libs gl` `pkg-config --libs OIS` `pkg-config --libs lua50` `pkg-config --libs lualib50` `pkg-config --libs openal` `pkg-config --libs vorbisfile`"
linker_objects=""
compiler_files=""

if [[ -z $CXX ]]
then
  echo "Info: CXX was not set, setting it to g++."
  export CXX=g++
fi

for cpp_file in `echo ${all_files[@]}`
do
  current=$current+1
  echo -n -e "\033[0;32m[$current/$total]\033[0m "
  compile $cpp_file
  linker_objects="$linker_objects $obj_dir$cpp_file.o"
done

# link it
echo "Linking Dusk..."
$CXX -o Dusk -s -L$obj_dir $linker_objects $linker_libs
if [[ $? -ne 0 ]]
then
  echo "Error while linking Dusk! Aborting."
  exit $E_COMPILE_ERROR
fi
exit 0 # all went well enough
