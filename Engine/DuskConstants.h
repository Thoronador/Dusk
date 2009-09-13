#ifndef DUSKCONSTANTS_H
#define DUSKCONSTANTS_H
#include <string>

//This file defines some constants which are shared among several files.

namespace Dusk
{
  #if defined(_WIN32)
    const std::string path_sep = "\\";
  #else
    const std::string path_sep = "/";
  #endif

  //standard mesh name for non-existing objects/ items
  const std::string cErrorMesh = "ErrorMarker.mesh";

  /*"Headers" for various data formats
    Originally, headers were written and read as arrays of four characters, but
    since 32bit unsigned integers have the same size and are easier to compare
    and set we use them instead, encoding the four characters into 32 bit int
    values.
  */

  const unsigned int cHeaderDusk = 1802728772; //"Dusk" (general file header)
  const unsigned int cHeaderItem = 1835365449; //"Item" (for item records)
  const unsigned int cHeaderLand = 1684955468; //"Land" (for landscape records)
  const unsigned int cHeaderLight = 1751607628; //"Ligh" (for Light records)
  const unsigned int cHeaderObjS = 1399480911; //"ObjS" (for static objects)
  const unsigned int cHeaderRefO = 1332110674; //"RefO" (for Referenced Object)
}//namespace

#endif //DUSKCONSTANTS_H
