/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To explain how variable scope
 works with C and C++.

 I covered function scope (static or not),
 variable scope (within blocks and static
 or thread_local within a function, or
 global and static or not).
*/
#include "stdio.h"
#include "inttypes.h"
#include "stdint.h"

static uint8_t a = 7;

/*
static int foo ( void ) {
  return 0;
}
*/

void foo ( void ) {
  thread_local uint32_t b = 2;

  fprintf (
    stdout,
    "[foo] a: %" PRIu8 "; b: %" PRIu32 "\n",
    a,
    b
  );

  b ++;
}

int main ( void ) {

{
  uint8_t a = 0;
  a += 1;
  fprintf ( stdout, "a: %" PRIu8 ".\n", a );
}

  fprintf ( stdout, "a: %" PRIu8 ".\n", a );

  foo (  );
  foo (  );

  return 0;
}
