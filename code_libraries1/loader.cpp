/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show an example of a how to
 create a code library in C and C++.
 Currently, this is exporting the only
 function as a C function.
*/
#include "stdio.h"
#include "inttypes.h"

extern "C" {
  int add ( int, int );
}

int main ( void ) {
  fprintf (
    stdout,
    "2+3 = %" PRIi64 "\n",
    static_cast<int64_t> ( add ( 2, 3 ) )
  );
  return 0;
}
