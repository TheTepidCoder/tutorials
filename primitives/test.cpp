/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To explain the most common
 primitive types that one would use
 in C and C++.

 I'd left out size_t, intptr_t uintptr_t,
 min_t, max_t...etc. which have very
 specific usages that will come up
 latter in a more organic context
 that will be more condusive to learning,
 anyway.
*/
#include "stdio.h"
#include "stdint.h" //int#_t uint#_t
#include "inttypes.h" //PRIi64 PRIu64
#include "float.h" //FLT_MAX DBL_EPSILON
#include "limits.h"

int main ( void ) {

  struct something *ps = 0;
  std::vector<struct something> v;
  v .emplace_back ( {....} );
  v .clear (  );
//  ps = (struct something *) calloc ( sizeof(struct something), # );
  ...
//  free ( ps );

  //std::vector std::map std::list std::deque ...

  const char *lpsz_string = "This is only a test.";
  //wchar_t
  //std::string
  //std::wstring

/*
  //uint#_t

  char
  wchar_t
  short
  long
  //long long
  //__int64
*/

  //int#_t (signed) can be negative
  //[-(2^(#-1)) -> 2^(#-1)) - 1]
  //-2^(#-1), -3, -2, -1, 0, 1, 2, ..., 2^(#-1) - 1
  //uint#_t (unsigned) can only be positive
  //[0 -> (2^#) - 1]
  //uint16_t -> 2^16-1 65,536-1 65,535

  //1.7 -32.6
  //float f; //4 bytes in size
  //double d; //8 bytes in size
  fprintf (
    stdout,
    "float epsilon: %e\n"
    "float min: %e\n"
    "float max: %f\n"
    "double epsilon: %e\n"
    "double min: %e\n"
    "double max: %f\n",
    FLT_EPSILON,
    FLT_MIN,
    FLT_MAX,
    DBL_EPSILON,
    DBL_MIN,
    DBL_MAX
  );

  return 0;
}
