/*
 Created on 2024-08-01 by TheTepidCoder
 Purpose: To explain conditional statements
 and loop types in C and c++.
*/
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"
#include "stdbool.h"

struct USER {
  char sz_name [ 20 ];
  uint8_t ui8_age;
};

void initialize_user (
  struct USER &user,
  const char *lpsz_username,
  uint8_t ui8_age
) {
  strcpy ( user .sz_name, lpsz_username );
  user .ui8_age = ui8_age;
}

//! strcmp(p1, p2)
bool strings_are_the_same ( const char *p1, const char *p2 ) {
  while ( *p1 && *p2 ) {
    if ( *p1 != *p2 ) {
      return false;
    }
    p1 ++;
    p2 ++;
  }
  //"abc" "abcd"
  if ( *p1 || *p2 ) {
    return false;
  }

  return true;
}

bool users_are_the_same (
  const struct USER &user,
  const struct USER &user2
) {
  if ( user .ui8_age != user2 .ui8_age ) {
    return false;
  }

  //if ( ! strings_are_the_same ( user .sz_name, user2 .sz_name ) ) {
  if ( strcmp ( user .sz_name, user2 .sz_name ) ) {
    return false;
  }

  return true;
}

int main ( void ) {

  uint8_t ui8_value = 17;
  if ( ui8_value < 10 ) {
    //this will execute if the condition is true
    fprintf ( stdout, "The value was less than ten.\n" );
  }
  else {
    fprintf ( stdout, "The value was was greater than or equal to ten.\n" );
  }

/*
{
  uint8_t ui8_i = 0;
  while ( ui8_i < 200 ) {
    fprintf ( stdout, "ui8_i: %" PRIu8 "\n", ui8_i );
    ui8_i ++;
  }
}
*/

  for ( uint8_t ui8_i = 0; ui8_i < 200; ui8_i ++ ) {
    fprintf ( stdout, "[for loop] ui8_i: %" PRIu8 "\n", ui8_i );
    for ( int i = 0; i < 10; i ++ ) {
      if ( ui8_i >= 100 ) {
        goto after_both_loops;
      }
   }
  }
after_both_loops:


/*
  uint8_t ui8_i = 0;
  repeat_starting_here:
  fprintf ( stdout, "[goto] ui8_i: %" PRIu8 "\n", ui8_i );

  ui8_i ++;
  if ( ui8_i < 200 ) goto repeat_starting_here;
*/

/*
{
  uint8_t ui8_i = 0;
  do {
    fprintf (
      stdout,
      "[do_while] ui8_i: %" PRIu8 "\n",
      ui8_i
    );
  } while ( ui8_i );
}
*/

  struct USER user = { 0 };
  initialize_user ( user, "TepidCoder", 20 );

  struct USER user2 = { 0 };
  initialize_user ( user2, "TepidCoder", 20 );
//  initialize_user ( user2, "OtherCoder", 20 );
  if ( users_are_the_same ( user, user2 ) ) {
    fprintf ( stdout, "The users are the same.\n" );
  }
  else {
    fprintf ( stdout, "The users are NOT the same.\n" );
  }

  uint16_t ui16_value = 2;
  switch ( ui16_value ) {
    case 0: { //if ( ui16_value == 0 )
      fprintf ( stdout, "ui16_value is 0.\n" );
      break;
    }
    case 1:
    case 2: { //if ( ui16_value == 2 )
      fprintf ( stdout, "ui16_value is 1 or 2.\n" );
      break;
    }

    default: {
      fprintf ( stdout, "ui16_value is some number we didn't expect.\n" );
    }
  }

  return 0;
}
