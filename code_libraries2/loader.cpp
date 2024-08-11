/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to add the
 __declspec directive to functions that
 you'd like to import and export on
 MSVC.
*/
#include "add.h"
#include "stdio.h"
#include "inttypes.h"
#include "stdint.h"

int main ( void ) {
  fprintf (
    stdout,
    "1+2 = %" PRIi64 "\n",
    static_cast<int64_t> ( add ( 1, 2 ) )
  );
  return 0;
}
