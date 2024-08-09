/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to break C++ template functions
 into multiple cpp files to decrease compilation time.
*/
#include "add.h"

#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

#include <string> //std::string std::wstring
#include <format> //std::vformat


void call_me ( void );

/*
uint16_t add ( uint8_t a, uint8_t b ) {
  return a + b;
}
*/

int main ( void ) {

  std::string str_output = std::vformat (
    "Sum: {}\n",
//    "Sum: {:.2f}\n",
    std::make_format_args (
      add ( 1.0f, 1.1f )
//      add ( 1, 2 ) //add ( 1.0f, 1.1f )
    )
  );
  fprintf ( stdout, "%s\n", str_output .c_str (  ) );

  fprintf (
    stdout,
    "7-5 = %" PRIu8 "\n",
//    sub<uint8_t> ( 7, 5 )
    add<uint8_t> ( 7, 5 )
  );

  call_me (  );

  return 0;
}
