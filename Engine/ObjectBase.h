/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
 Purpose: ObjectBase Singleton class
          holds information about all distinct objects in the game

 History:
     - 2009-07-01 (rev 101) - initial version (by thoronador)
     - 2009-07-02 (rev 102) - checks for empty ID and mesh in addObject()
     - 2009-07-06 (rev 103) - possibility to work with error mesh added
     - 2009-07-13 (rev 104) - SaveToStream() added
     - 2009-07-15 (rev 105) - LoadFromStream() added
     - 2009-07-24 (rev 111) - GetFirst() and GetEnd() added
     - 2009-07-31 (rev 113) - deleteObject() added
     - 2009-09-24 (rev 131) - } small fixes
     - 2009-09-27 (rev 132) - }
     - 2010-01-30 (rev 161) - obsolete load/save functions removed
                            - documentation updated
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-26 (rev 260) - value for collision detection added

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <map>
#include <string>
#include <fstream>

namespace Dusk
{
  struct ObjectRecord
  {
    std::string Mesh;
    bool collide;
  };

  /* class ObjectBase
           This class' purpose is to hold the IDs of all distinct static objects
           in the game and their corresponding mesh name. "Static objects" in
           the above context are all non-moving objects which cannot be picked
           up by the player or a NPC. Examples of static objects would be walls,
           trees, furniture (non-movable) and so on.
           Things like an apple or a bottle are NOT "static objects", they are
           "items", because they can be picked up and/or used by the player. See
           the class ItemBase for items.


           Think of ObjectBase as a sort of look-up table, e.g.:

             ID  |  Mesh
           ------+------------------
           tree  | tree_oak.mesh
           Chair | wooden_chair.mesh
            ...  | ...
  */
  class ObjectBase
  {
    public:
      /* destructor */
      virtual ~ObjectBase();

      /* singleton access */
      static ObjectBase& getSingleton();

      /* returns true, if an object with the given ID exists */
      bool hasObject(const std::string& IDOfObject) const;

      /* adds object with given ID and mesh. If an object with that ID already
         exists, it will be overwritten. If ID or mesh is an empty string,
         nothing will be done.

         parameters:
             ID        - ID of the object
             Mesh      - mesh path for the object
             collision - boolean value that indicates whether the object will be
                         considered for collision detection (true) or not (false)
      */
      void addObject(const std::string& ID, const std::string& Mesh, const bool collision);

      /* tries to delete the object with the given ID and returns true if such
         an object was present (before deleting it, that is)

         parameters:
             ID_of_Object - ID of the object that shall be deleted
      */
      bool deleteObject(const std::string& ID_of_Object);

      /* deletes ALL objects, use with caution (or not at all) */
      void clearAllObjects();

      /* returns the number of objects which are currently present */
      unsigned int numberOfObjects() const;

      /* returns the mesh path of the given object, or an empty string, if no
          object with that ID is present. However, if UseMarkerOnError is set
          to true (default), this function will return the path to a predefined
          error mesh instead.
      */
      std::string getMeshName(const std::string& ID, const bool UseMarkerOnError=true) const;

      /* returns whether an object of the given ID is considered during
         collision detection or not
      */
      bool getObjectCollision(const std::string& ID) const;

      /* tries to save all objects to the stream and returns true on success

         parameters:
             Stream - output stream that is used to save the data
      */
      bool saveToStream(std::ofstream& Stream) const;

      /* tries to read the next object from stream and returns true, if successful

         parameters:
             Stream - input stream that is used to load the data
      */
      bool loadFromStream(std::ifstream& Stream);

      /* helper functions to access internal map iterators - not used in-game,
         only used by Editor application.
      */
      std::map<std::string, ObjectRecord>::const_iterator getFirst() const;
      std::map<std::string, ObjectRecord>::const_iterator getEnd() const;
    private:
      /*  constuctor */
      ObjectBase();

      /* empty copy constructor - singleton pattern */
      ObjectBase(const ObjectBase& op){}

      std::map<std::string, ObjectRecord> m_ObjectList;
  };//class

}//namespace

#endif
