#include "DiceBox.h"
#include <ctime>
#include <cstdlib>

namespace Dusk
{

DiceBox::DiceBox()
{
  srand(time(NULL));
}

DiceBox::~DiceBox()
{
  //empty
}

DiceBox& DiceBox::GetSingleton()
{
  static DiceBox Instance;
  return Instance;
}

unsigned int DiceBox::d4()
{
  const float res = rand()/float(RAND_MAX);
  return 1+ int(4*res);
}

unsigned int DiceBox::d6()
{
  const float res = rand()/float(RAND_MAX);
  return 1+ int(6*res);
}

unsigned int DiceBox::d8()
{
  const float res = rand()/float(RAND_MAX);
  return 1+ int(8*res);
}

unsigned int DiceBox::d10()
{
  const float res = rand()/float(RAND_MAX);
  return 1+ int(10*res);
}

unsigned int DiceBox::d20()
{
  const float res = rand()/float(RAND_MAX);
  return 1+ int(20*res);
}

unsigned int DiceBox::d4(const unsigned short int n)
{
  unsigned int result = 0;
  unsigned int i;
  for (i=0; i<n; i=i+1)
  {
    result += d4();
  }
  return result;
}

unsigned int DiceBox::d6(const unsigned short int n)
{
  unsigned int result = 0;
  unsigned int i;
  for (i=0; i<n; i=i+1)
  {
    result += d6();
  }
  return result;
}

unsigned int DiceBox::d8(const unsigned short int n)
{
  unsigned int result = 0;
  unsigned int i;
  for (i=0; i<n; i=i+1)
  {
    result += d8();
  }
  return result;
}

unsigned int DiceBox::d10(const unsigned short int n)
{
  unsigned int result = 0;
  unsigned int i;
  for (i=0; i<n; i=i+1)
  {
    result += d10();
  }
  return result;
}

unsigned int DiceBox::d20(const unsigned short int n)
{
  unsigned int result = 0;
  unsigned int i;
  for (i=0; i<n; i=i+1)
  {
    result += d20();
  }
  return result;
}

} //namespace
