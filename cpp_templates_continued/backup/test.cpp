/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to define C++ template classes,
 rather than just templated functions.
*/
#include "vector.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"

int main ( void ) {
  vector<uint8_t> vec;

  for ( size_t st_i = 0; st_i < 1000; st_i ++ ) {
    vec .push_back ( st_i % 256 );
  }

  uint8_t *lpui8_p = vec .data (  );
  uint8_t *lpui8_e = vec .data (  ) + vec .count (  );
  while ( lpui8_p < lpui8_e ) {
    fprintf ( stdout, "Value: %" PRIu8 "\n", *lpui8_p );
    lpui8_p ++;
  }
/*
  for ( const auto &v : vec ) {
    fprintf (
      stdout,
      "This is the value: \"%" PRIu8 "\"\n",
      v
    );
  }
*/
  return 0;
}
