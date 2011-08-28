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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-12-23
 Purpose: DiceBox singleton class
          Provides a central point for all random numbers in the game
 History:
     - 2009-12-23 (rev 144) - initial version; contains d4, d6, d10 and d20
     - 2010-06-06 (rev 215) - d8() added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2011-08-28 (rev 298) - function random() added for random floats

 ToDo list:
     - add more functions/ dice, if needed
     - remove unneccessary functions/ dice, if they are not needed

 Bugs:
     - None. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DICEBOX_H
#define DICEBOX_H

namespace Dusk
{
  class DiceBox
  {
    public:
      /* virtual destructor */
      virtual ~DiceBox();

      /* Singleton access function */
      static DiceBox& getSingleton();

      /* "Throws" a four-sided die, returns a random number between 1 and 4 */
      unsigned int d4();

      /* "Throws" a six-sided die, returns a random number between 1 and 6 */
      unsigned int d6();

      /* "Throws" a eight-sided die, returns a random number between 1 and 8 */
      unsigned int d8();

      /* "Throws" a ten-sided die, returns a random number between 1 and 10 */
      unsigned int d10();

      /* "Throws" a 20-sided die, returns a random number between 1 and 20 */
      unsigned int d20();

      /* "Throws" n four-sided dice and returns the sum of all

         remarks:
             If n is set to zero, this function will return zero.
             If you set n to one, you should call the function variant without
             parameter instead.
      */
      unsigned int d4(const unsigned short int n);

      /* "Throws" n six-sided dice and returns the sum of all */
      unsigned int d6(const unsigned short int n);

      /* "Throws" n eight-sided dice and returns the sum of all */
      unsigned int d8(const unsigned short int n);

      /* "Throws" n ten-sided dice and returns the sum of all */
      unsigned int d10(const unsigned short int n);

      /* "Throws" n six-sided dice and returns the sum of all */
      unsigned int d20(const unsigned short int n);

      /* returns a random floating point value from within [0;1) */
      float random();
    private:
      //constructor
      DiceBox();
      //empty copy constructor
      DiceBox(const DiceBox& op) {}
  };//class
} //namespace

#endif // DICEBOX_H
