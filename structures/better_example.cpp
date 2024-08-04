/*
 Created on 2024-08-01 by TheTepidCoder
 Purpose: To explain how to store a collection
 of related variables in one convenient place.
 I'd also showed how to share structure
 definitions and function definitions across
 multiple source files (.c/.cpp).
*/
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"

struct record {
  char sz_name [ 20 ]; //[0 -> 19]
  char sz_address [ 100 ]; //[0 -> 99]
  char sz_phone_number [ 14 ]; //(123)123-1234\0 [0 -> 13]
};

void show_record ( const struct record *r ) {
  fprintf (
    stdout,
    "Name: \"%s\"\n"
    "Address: \"%s\"\n"
    "Phone number: \"%s\"\n",
    r ->sz_name,
    r ->sz_address,
    r ->sz_phone_number
  );
}

void initialize_record (
  struct record *lp_record,
  const char *lpsz_name,
  const char *lpsz_address,
  const char *lpsz_phone_number
) {
  fprintf (
    stdout,
    "[%s] Record location in memory: %p\n",
    __FUNCTION__,
    lp_record
  );

  strcpy (
    lp_record ->sz_name,
    lpsz_name
  );
  strcpy (
    lp_record ->sz_address,
    lpsz_address
  );
  strcpy (
    lp_record ->sz_phone_number,
    lpsz_phone_number
  );
  //show_record ( lp_record );
}

int main ( void ) {

/*
  char ccc [ 3 ] = { '1', 0 }; //"1";
  for ( int i = 0; i < sizeof(ccc)/sizeof(char); i ++ ) {
    fprintf ( stdout, "%02x\n", ccc [ i ] );
  }
*/

  struct record rary_records [ 2 ] = { 0, 0 };
/*
  {
    .sz_name = "Tepid_Coder",
    .sz_address = "123 Fake Street",
    .sz_phone_number = "(123)321_9876"
  };
*/

  fprintf (
    stdout,
    "Record location in memory: %p\n",
    &rary_records
  );

  fprintf (
    stdout,
    "Size of one record structure: \"%" PRIu64 "\"\n",
    static_cast<uint64_t> ( sizeof ( struct record ) )
  );

  initialize_record (
    rary_records, //(const struct record *)
    "Tepid_Coder",
    "123 Fake Street",
    "(123)321_9876"
  );

  initialize_record (
    &rary_records [ 1 ],
    "That guy",
    "124 Fake Street",
    "(123)123_4321"
  );

  const struct record *lp_record = rary_records;
  for (
    size_t st_i = 0;
    st_i < sizeof ( rary_records ) / sizeof ( struct record );
    st_i ++
  ) {
    fprintf ( stdout, "Current record location: %p.\n", lp_record );
    show_record ( lp_record );
    lp_record ++;
    //show_record ( &rary_records [ st_i ] );
  }

{
  char sz_string [ 5 ] = "sdrs"; // -=>test
  char *p = &sz_string [ 0 ];
  while ( *p != 0 ) {
    //p [ 0 ] ++;
    (*p) ++;
    p ++;
  }
  fprintf ( stdout, "\"%s\"\n", sz_string );
}

  return 0;
}
