/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to break C++ template functions
 into multiple cpp files to decrease compilation time.
*/
#include "add.h" //T sub(T, T)
#include "stdio.h" //fprintf
#include "stdint.h" //int#_t
#include "inttypes.h" //PRIu#

void call_me ( void ) {
  fprintf (
    stdout,
    "3-2 = %" PRIu8 "\n",
    sub<uint8_t> ( 3, 2 )
  );
}
