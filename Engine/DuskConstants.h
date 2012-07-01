/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

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

#ifndef DUSKCONSTANTS_H
#define DUSKCONSTANTS_H
#include <string>
#include <stdint.h>

//This file defines some constants which are shared among several files.

namespace Dusk
{
  #if defined(_WIN32)
    const std::string path_sep = "\\";
  #elif defined(__linux__) || defined(linux)
    const std::string path_sep = "/";
  #else
    #error "Unknown operating system!"
  #endif

  /*standard mesh name for non-existing objects/ items */
  const std::string cErrorMesh = "ErrorMarker.mesh";

  /*"Headers" for various data formats
    Originally, headers were written and read as arrays of four characters, but
    since 32bit unsigned integers have the same size and are easier to compare
    and set we use them instead, encoding the four characters into 32 bit int
    values.
    (Might not be portable for systems with different byte order.)
  */

  const uint32_t cHeaderCont = 1953394499; //"Cont" (for containers (base))
  const uint32_t cHeaderDeps = 1936745796; //"Deps" (for dependencies of save game)
  const uint32_t cHeaderDial = 1818323268; //"Dial" (for dialogue entries)
  const uint32_t cHeaderDusk = 1802728772; //"Dusk" (general file header)
  const uint32_t cHeaderInve = 1702260297; //"Inve" (for Inventory data)
  const uint32_t cHeaderItem = 1835365449; //"Item" (for item records)
  const uint32_t cHeaderJour = 1920298826; //"Jour" (for Journal records)
  const uint32_t cHeaderLand = 1684955468; //"Land" (for landscape records)
  const uint32_t cHeaderLight = 1751607628; //"Ligh" (for Light records)
  const uint32_t cHeaderMean = 1851876685; //"Mean" (for "mean" save game type)
  const uint32_t cHeaderNPC_ = 1598246990; //"NPC_" (for NPC(Base) records)
  const uint32_t cHeaderObjS = 1399480911; //"ObjS" (for static objects)
  const uint32_t cHeaderPlay = 2036427856; //"Play" (for Player object)
  const uint32_t cHeaderProj = 1785688656; //"Proj" (for projectiles)
  const uint32_t cHeaderQLog = 1735347281; //"QLog" (for QuestLog)
  const uint32_t cHeaderRefA = 1097229650; //"RefA" (for Referenced AnimatedObject)
  const uint32_t cHeaderRefC = 1130784082; //"RefC" (for Referenced Container)
  const uint32_t cHeaderRefI = 1231447378; //"RefI" (for Referenced Item)
  const uint32_t cHeaderRefL = 1281779026; //"RefL" (for Referenced Light)
  const uint32_t cHeaderRefN = 1315333458; //"RefN" (for Referenced NPC)
  const uint32_t cHeaderRefO = 1332110674; //"RefO" (for Referenced Object)
  const uint32_t cHeaderRefP = 1348887890; //"RefP" (for Referenced Projectile)
  const uint32_t cHeaderRefR = 1382442322; //"RefR" (for Referenced Resource)
  const uint32_t cHeaderRefV = 1449551186; //"RefV" (for Referenced Vehicle)
  const uint32_t cHeaderRfWe = 1700226642; //"RfWe" (for Referenced Weapon)
  const uint32_t cHeaderRfWP = 1347905106; //"RfWP" (for Referenced WaypointObject)
  const uint32_t cHeaderRsrc = 1668445010; //"Rsrc" (for ResourceBase records)
  const uint32_t cHeaderSave = 1702256979; //"Save" (for SaveGame)
  const uint32_t cHeaderSoun = 1853189971; //"Soun" (for SoundBase records)
  const uint32_t cHeaderVehi = 1768449366; //"Vehi" (for VehicleBase records)
  const uint32_t cHeaderWeap = 1885431127; //"Weap" (for Weapon(Base))
}//namespace

#endif //DUSKCONSTANTS_H
