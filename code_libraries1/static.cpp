/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show an example of statically
 linking one's dependencies. The example
 kind of fell flat because I'm unsure
 whether Mingw64 is already statically
 linking the required library dependencies
 on/for Windows. This will make more sense
 as an approach when linking your own
 .a and .lib files, in the future.
*/
#include "stdio.h"

int main ( void ) {
  puts ( "Hello." );
  return 0;
}
