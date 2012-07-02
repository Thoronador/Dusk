/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012 thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-30
 Purpose: ItemBase Singleton class
          holds information about all distinct items in the game

 History:
     - 2009-07-02 (rev 102) - initial version (by thoronador)
     - 2009-07-06 (rev 103) - possibility to work with error mesh added
     - 2009-07-13 (rev 104) - SaveToStream() added, integration into DataLoader
     - 2009-07-15 (rev 105) - LoadFromStream() added
     - 2009-07-24 (rev 111) - GetFirst() and GetEnd() added
     - 2009-07-31 (rev 113) - deleteItem() added
     - 2009-09-13 (rev 128) - }
     - 2009-09-24 (rev 131) - } small fixes
     - 2009-09-27 (rev 132) - }
     - 2010-01-30 (rev 161) - obsolete load/save functions removed
                            - documentation updated
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - iterator type for getFirst() and getEnd() added
     - 2012-04-05 (rev 303) - non-existent IDs in query functions can now throw
                              exceptions
     - 2012-06-30 (rev 308) - update of getMeshName() and getItemName()
                              definition

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef DUSK_ITEMRECORD_H
#define DUSK_ITEMRECORD_H

#include <fstream>
#include "DataRecord.h"

namespace Dusk
{
  //structure that can hold the data of one item
  struct ItemRecord: public DataRecord
  {
    std::string Name;
    int value;
    float weight;
    std::string Mesh;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const;

    /* Tries to save the data record to stream outStream and returns true on
       success, false otherwise.

       parameters:
           outStream - the output stream to which the data will be saved
    */
    virtual bool saveToStream(std::ofstream& outStream) const;

    /* Tries to load the data record from stream inStream and returns true on
       success, false otherwise.

       parameters:
           inStream - the input stream from which the data will be read

       remarks:
           The record may have inconsistent data, if the function fails.
    */
    virtual bool loadFromStream(std::ifstream& inStream);
  }; //struct

  //const ItemRecord cEmptyItemRecord = { "", 0, 0.0, ""};

  /* class ItemBase
           This class' purpose is to hold the IDs of all distinct items in the
           game and their corresponding data like mesh name, cost/value, weight.
           "Items" in the above context are all non-moving objects which can be
           picked up by the player or a NPC and put in the inventory. Examples
           of items would be apples, bottles and so on, maybe even weapons.
           Things like a chair are not "items", they are "static objects",
           because they cannot be picked up and/or used by the player. See the
           class ObjectBase for "objects".

           Think of ItemBase as a sort of look-up table, e.g.:

             ID   |  name          |  value | weight | mesh
           -------+----------------+--------+--------+-------------------
           apple  | Fresh Apple    |   5    |  0.2   | tree_oak.mesh
           bottle | Bottle of Milk |   8    |  0.75  | filled_bottle.mesh
             ...  |  ...           |  ...   |  ...   | ...

  */

}//namespace

#endif // DUSK_ITEMRECORD_H
