#ifndef NO_OGRE_IN_LANDSCAPE
  //avoids using Ogre headers which we don't need for converter
  #define NO_OGRE_IN_LANDSCAPE
#endif

#include <iostream>
#include <fstream>
#include <string>
#include "Converter.h"

int main()
{
  std::string ESP_File, Dusk_File;
  std::cout << "Converter for TES3: Morrowind's ESP files to format of Dusk\n"
            << "\n\n  (Yes, it's command line only.)\n\n";
  std::cout << "Please enter path to ESP file:\n";
  std::cin >> ESP_File;
  std::cout << "Please enter path to Dusk file: (will be created during process)\n";
  std::cin >> Dusk_File;

  std::ifstream test_in;
  test_in.open(ESP_File.c_str(), std::ios::in | std::ios::binary);
  if (!test_in)
  {
    std::cout << "Error: could not open file \""<<ESP_File<<"\".\n";
    return 0;
  }

  std::cout << "Scaning file \""<<ESP_File<<"\" for landscape records.\n";
  if (ScanESP(ESP_File, Dusk_File))
  {
    std::cout << "Success!\n";
  }
  else
  {
    std::cout << "(At least one) Error occured!\n";
  }
  return 0;
}
