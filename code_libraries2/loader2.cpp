/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to create/use a static
 library.
*/
#include "add.h" //This has our signature: __declspec(dllimport) int add ( int, int );

#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

int main ( void ) {
  fprintf (
    stdout,
    "4+5 = %" PRIi64 "\n",
    static_cast<int64_t> ( 4, 5 )
  );
  return 0;
}
