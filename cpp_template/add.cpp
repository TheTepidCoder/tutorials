/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to break C++ template functions
 into multiple cpp files to decrease compilation time.
*/
#include "stdio.h"
#include "inttypes.h"
#include "stdint.h"

template<typename T>
T add ( T a, T b ) {
  fprintf (
    stdout,
    "\"%s\" -> %" PRIu8 "\n",
    __PRETTY_FUNCTION__,
   static_cast<uint8_t> (sizeof(T))
  );

  return a + b;
}

template uint8_t add<uint8_t> ( uint8_t, uint8_t );
template float add<float> ( float, float );
//void dummy ( void ) {
//  add<int> ( 1, 2 );
//}
