/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011 thoronador

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

  const unsigned int cHeaderCont = 1953394499; //"Cont" (for containers (base))
  const unsigned int cHeaderDeps = 1936745796; //"Deps" (for dependencies of save game)
  const unsigned int cHeaderDial = 1818323268; //"Dial" (for dialogue entries)
  const unsigned int cHeaderDusk = 1802728772; //"Dusk" (general file header)
  const unsigned int cHeaderInve = 1702260297; //"Inve" (for Inventory data)
  const unsigned int cHeaderItem = 1835365449; //"Item" (for item records)
  const unsigned int cHeaderJour = 1920298826; //"Jour" (for Journal records)
  const unsigned int cHeaderLand = 1684955468; //"Land" (for landscape records)
  const unsigned int cHeaderLight = 1751607628; //"Ligh" (for Light records)
  const unsigned int cHeaderMean = 1851876685; //"Mean" (for "mean" save game type)
  const unsigned int cHeaderNPC_ = 1598246990; //"NPC_" (for NPC(Base) records)
  const unsigned int cHeaderObjS = 1399480911; //"ObjS" (for static objects)
  const unsigned int cHeaderPlay = 2036427856; //"Play" (for Player object)
  const unsigned int cHeaderProj = 1785688656; //"Proj" (for projectiles)
  const unsigned int cHeaderQLog = 1735347281; //"QLog" (for QuestLog)
  const unsigned int cHeaderRefA = 1097229650; //"RefA" (for Referenced AnimatedObject)
  const unsigned int cHeaderRefC = 1130784082; //"RefC" (for Referenced Container)
  const unsigned int cHeaderRefI = 1231447378; //"RefI" (for Referenced Item)
  const unsigned int cHeaderRefL = 1281779026; //"RefL" (for Referenced Light)
  const unsigned int cHeaderRefN = 1315333458; //"RefN" (for Referenced NPC)
  const unsigned int cHeaderRefO = 1332110674; //"RefO" (for Referenced Object)
  const unsigned int cHeaderRefP = 1348887890; //"RefP" (for Referenced Projectile)
  const unsigned int cHeaderRefV = 1449551186; //"RefV" (for Referenced Vehicle)
  const unsigned int cHeaderRfWe = 1700226642; //"RfWe" (for Referenced Weapon)
  const unsigned int cHeaderRfWP = 1347905106; //"RfWP" (for Referenced WaypointObject)
  const unsigned int cHeaderSave = 1702256979; //"Save" (for SaveGame)
  const unsigned int cHeaderVehi = 1768449366; //"Vehi" (for VehicleBase records)
  const unsigned int cHeaderWeap = 1885431127; //"Weap" (for Weapon(Base))
}//namespace

#endif //DUSKCONSTANTS_H
