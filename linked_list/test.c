/*
 Created on 2024-09-01 by TheTepidCoder
 Purpose: To show off a C-style doubly linked list.
 I was kind of sort of happy to see that I've still
 got it. It has been years since I've coded a doubly
 linked list in C. I've been spoiled with C++'s STL
 containers for many years, at this point.
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "inttypes.h"
#include "stdbool.h"

typedef struct NODE {
  char *lpsz_name; //null terminated name
  uint8_t ui8_age;

  struct NODE *lp_next;
  struct NODE *lp_prev;
} NODE;

NODE *new_node (
  NODE *lp_prev,
  const char *lpsz_name,
  uint8_t ui8_age
) {
  NODE *lp_node = (NODE *) malloc ( sizeof(NODE) );
  lp_node ->lpsz_name = strdup ( lpsz_name );
  lp_node ->ui8_age = ui8_age;
  lp_node ->lp_next = 0;
  lp_node ->lp_prev = lp_prev;

  return lp_node;
}

NODE *add_node (
  NODE **list,
  const char *lpsz_name,
  uint8_t ui8_age
) {
  if ( ! list ) {
    fprintf (
      stderr,
      "Error: Invalid reference to list root pointer variable passed to add_node.\n"
    );
    exit ( 0 );
  }
  if ( ! *list ) {
    return ((*list) = new_node ( 0, lpsz_name, ui8_age ));
  }

  NODE *node = *list;
  while ( node ->lp_next ) {
    node = node ->lp_next;
  }
  return node ->lp_next = new_node ( node, lpsz_name, ui8_age );
}

void free_list (
  NODE **list
) {
  if ( ! list ) {
    return ;
  }

  NODE *next = *list, *free_me;
  while ( next ) {
    free_me = next;
    next = next ->lp_next;
    free ( free_me );
  }

  *list = 0;
}

void delete_node (
  NODE **list,
  NODE *to_delete
) {
  if ( ! list ) {
    fprintf ( stderr, "Error: An invalid reference to the start of the list was passed to delete_node.\n" );
    exit ( 0 );
  }

  if ( to_delete ->lp_next ) {
    to_delete ->lp_next ->lp_prev = to_delete ->lp_prev;
  }
  if ( to_delete ->lp_prev ) {
    to_delete ->lp_prev ->lp_next = to_delete ->lp_next;
  }


  if ( *list == to_delete ) {
    *list = to_delete ->lp_next;
  }

  free ( to_delete );

}

bool push_front (
  NODE **list,
  const char *lpsz_name,
  uint8_t ui8_age
) {
  if ( ! list ) {
    fprintf (
      stderr,
      "The reference to the root of the list that was passed to push_front was invalid!\n"
    );
    exit ( 0 );
  }
  NODE *node = (NODE *) malloc ( sizeof(NODE) );
  if ( ! node ) {
    return false;
  }

  node ->lpsz_name = strdup ( lpsz_name );
  node ->ui8_age = ui8_age;
  node ->lp_next = *list;
  if ( *list ) {
    (*list) ->lp_prev = node;
  }
  *list = node;

  return true;
}

int main ( void ) {
  NODE *list = 0, *end = 0;

/*
  end = list = new_node ( 0, "TheTepidCoder", 10 );
  end = end ->lp_next = new_node ( end, "SomeBetterCoder", 15 );
  end = end ->lp_next = new_node ( end, "NonCoder", 105 );
*/

 push_front ( &list, "TheTepidCoder", 10 );
 end = list;
 push_front ( &list, "SomeBetterCoder", 15 );
 push_front ( &list, "NonCoder", 105 );

/*
  end = add_node ( &list, "TheTepidCoder", 10 );
  if ( ! end ) {
    fprintf (
      stderr,
      "Error: Couldn't add the first node of the linked list.\n"
    );
    return 0;
  }
  if ( ! (end = add_node ( &list, "SomeBetterCoder", 15 )) ) {
    fprintf (
      stderr,
      "Error: Couldn't add the second node of the linked list.\n"
    );
    return 0;
  }
  if ( ! (end = add_node ( &list, "NonCoder", 105 )) ) {
    fprintf (
      stderr,
      "Error: Couldn't add the final node of the linked list.\n"
    );
    return 0;
  }
*/

  //delete_node ( &list, list );
  //delete_node ( &list, list ->lp_next );
  //delete_node ( &list, list ->lp_next ->lp_next );

  for ( NODE *node = list; node; node = node ->lp_next ) {
    fprintf (
      stdout,
      "Name: \"%s\"; Age: %" PRIu8 "\n",
      node ->lpsz_name,
      node ->ui8_age
    );
  }

  for ( NODE *node = end; node; node = node ->lp_prev ) {
    fprintf (
      stdout,
      "Name: \"%s\"; Age: %" PRIu8 "\n",
      node ->lpsz_name,
      node ->ui8_age
    );
  }

  //NODE *list = make_node ( "TheTepidCoder", 10 );
  //list ->lp_next = make_node ( ... );

  return 0;
}
