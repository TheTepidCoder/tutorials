/*
 Created on 2024-08-01 by TheTepidCoder
 Purpose: To explain how to store a collection
 of related variables in one convenient place.
 I'd also showed how to share structure
 definitions and function definitions across
 multiple source files (.c/.cpp).
*/
#include "stdint.h"
#include "stdio.h"
#include "common.h"

void show_year ( const date_time &dt ) {
  date_time ndt = { 0 };
  intialize_date_time (
    ndt,
    dt .ui32_year,
    dt .ui8_month,
    dt .ui8_day
  );

  fprintf ( stdout, "year %02d\n", ndt .ui32_year );
}
