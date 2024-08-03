/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To expound upon what I'd already
 explained about pointers and conditional
 statements in C or C++.
*/
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

void print_character ( const char *p ) {
  fprintf ( stdout, "This is the character: '%c'\n", *p );
}

int main ( void ) {

/*
  if ( (1 && 2) || (0 && 1 && 2) ) {
    fprintf ( stdout, "Either 1 && 2 are both true; or 0 && 1 && 2 are all true.\n" );
  }
  else {
    fprintf ( stdout, "Neither 1 && 2 nor 1 && 2 && 3 are true.\n" );
  }
*/

  char cary_string [  ] = "This is only a test.";
  const char *p = cary_string + 2; //char[] -> char *
//  fprintf ( stdout, "%c\n", p [ 2 ] );
//  fprintf ( stdout, "%c\n", *(p + 2) ); //*((char *) cary_string + 2)

  print_character ( &p [ 3 ] );
  print_character ( cary_string + 2 );

  size_t st_offset = &p [ 3 ] - cary_string;
  fprintf (
    stdout,
    "Element at p[2] is %" PRIu64 " elements away from the start.\n",
    static_cast<uint64_t> ( st_offset )
  );

  //&p [ 2 ] = cary_string + 2

  return 0;

}
