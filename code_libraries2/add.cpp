/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to add the
 __declspec directive to functions that
 you'd like to import and export on
 MSVC.
*/
// Build with:
// g++ -shared -DBUILDING_ADD_LIBRARY=1 add.cpp -o add.dll
#include "add.h"
extern "C" {
  IMPORT_OR_EXPORT int add ( int a, int b ) {
    return a + b;
  }
}
