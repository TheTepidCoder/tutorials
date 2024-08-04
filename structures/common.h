/*
 Created on 2024-08-01 by TheTepidCoder
 Purpose: To explain how to store a collection
 of related variables in one convenient place.
 I'd also showed how to share structure
 definitions and function definitions across
 multiple source files (.c/.cpp).
*/
#pragma once
#ifndef COMMON_HEADER
#define COMMON_HEADER

#include "stdint.h"

#ifndef DATE_TIME_STRUCTURE
#define DATE_TIME_STRUCTURE
typedef struct date_time {
  uint32_t ui32_year;
  //4 bits for month
  //ceil(log(31)/log(2)) --> 2^5 = 32 (5 bits)
  uint8_t ui8_month : 4;
  uint8_t ui8_day : 5;
} date_time;
#endif

//Functions from test.cpp
void intialize_date_time (
   date_time &dt,
   const uint32_t ui32_year,
   const uint8_t ui8_month,
   const uint8_t ui8_day
);

//Functions from test2.cpp
void show_year ( const date_time &dt );

#endif
