/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    23.12.2009
 Purpose: DiceBox singleton class
          Provides a central point for all random numbers in the game
 History:
     - 23.12.2009 - initial version; contains d4, d6, d10 and d20

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
      static DiceBox& GetSingleton();

      /* "Throws" a four-sided die, returns a random number between 1 and 4 */
      unsigned int d4();

      /* "Throws" a six-sided die, returns a random number between 1 and 6 */
      unsigned int d6();

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

      /* "Throws" n ten-sided dice and returns the sum of all */
      unsigned int d10(const unsigned short int n);

      /* "Throws" n six-sided dice and returns the sum of all */
      unsigned int d20(const unsigned short int n);
    private:
      //constructor
      DiceBox();
      //empty copy constructor
      DiceBox(const DiceBox& op) {}
  };//class
} //namespace

#endif // DICEBOX_H
