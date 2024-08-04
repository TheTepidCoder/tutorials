/*
 Created on 2024-08-01 by TheTepidCoder
 Purpose: To explain how to store a collection
 of related variables in one convenient place.
 I'd also showed how to share structure
 definitions and function definitions across
 multiple source files (.c/.cpp).
*/
#include "common.h"

#include "stdio.h"
#include "stdint.h"

//uint8_t ui8_ary_bits [ 80 / 8 ];
//ui8_ary_bits |= ui8_month & 0x0f; //4 used
//ui8_ary_bit |= (ui8_day & 0x1f) << 4; //5 used


void intialize_date_time (
   date_time &dt,
   const uint32_t ui32_year,
   const uint8_t ui8_month,
   const uint8_t ui8_day
) {
  dt .ui32_year = ui32_year;
  dt .ui8_month = ui8_month;
  dt .ui8_day = ui8_day;
}

void print_date_time ( const date_time &dt ) {
  fprintf (
    stdout,
    "%04d-%02d-%02d\n",
    dt .ui32_year,
    dt .ui8_month,
    dt .ui8_day
  );
}

int main ( void ) {

  date_time dt; // = { 0 };
  intialize_date_time (
    dt,
    2024,
    8,
    1
  );
  print_date_time (
    dt
  );

  show_year ( dt );

  return 0;
}
