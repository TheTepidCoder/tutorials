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

#include <stdexcept>
#include <iostream>

#include "some_class.h"

void test001 ( void ) {
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
}

int test002 ( void ) {
  try {
    vector<uint8_t> vec = {
      0, 1, 2, 3
    };

    vec .push_back ( 7 );
    for ( const auto &value : vec ) {
      std::cout << static_cast<int> ( value ) << std::endl;
    }
  } catch ( std::exception &e ) {
    std::cerr << "Exception: \"" << e .what (  ) << "\"" << std::endl;
    return 0;
  }
  return 1;
}

int main ( void ) {
/*
  some_class scary [ 3 ] = {
    1, 2, 3
  };
  vector<some_class> vec {
    std::move ( scary[0] ),
    std::move ( scary[1] ),
    std::move ( scary[2] )
  };
*/
/*
  vector<some_class> vec {
    std::move ( some_class(1) ),
    std::move ( some_class(2) ),
    std::move ( some_class(3) )
  };
*/
  vector<some_class> vec {
    1,
    2,
    3
  };


  return 0;
}
