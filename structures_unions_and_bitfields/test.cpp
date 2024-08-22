#include <iostream>
#include <iterator>
#include <ostream>
#include <format>

#include <cstdint>
#include <cstring>

#define show_offset(s, m) \
  std::cout << "Range of " #m ": [" << \
    offsetof ( s, m ) << \
    ", " << \
   ((offsetof ( s, m ) + sizeof ( ((struct s *) 0) ->m )) - 1) << \
    "]" << std::endl


/*
Offset of ui32_something: 0
Offset of ui8_small_number: 4
Offset of ui16_slightly_bigger_number: 6
Offset of ui64_very_large_number: 8
*/
//#pragma pack(push, 1) //align to one byte and preserve old packing scheme so that we can return to it.
//#pragma pack(1) //align to one and don't preserve old packing scheme
//Idiomatic GCC/G++ way of doing it.
//struct __attribute__((packed)) some_structure {
/*
struct some_structure {
  uint32_t ui32_some_number; //4 (0-3)
  float f; //4 (4-7)
  uint8_t ui8_small_number; //1 (4-5)

union things {
  char c;
  uint8_t ui8_byte;
  float f_thing;
  uint64_t ui64_number;
} u_things;


  uint16_t ui16_slightly_bigger_number; //2 (6-7)
  //Maybe padding
  uint64_t ui64_very_large_number; //8 (8-15)
};
*/
//#pragma pack() //return to default packing
//#pragma pack(pop) //return to previously used packing

struct some_structure {
  uint64_t ui64_very_large_number;
  union things {
    char c;
    uint8_t ui8_byte;
    float f_thing;
    uint64_t ui64_number;
  } u_things;

  uint32_t ui32_some_number;
  float f;
  uint16_t ui16_slightly_bigger_number;
  uint8_t ui8_small_number;
};

//37 bits total
//actually have 8 bytes = 64 bits
//7*8 = 56
struct __attribute__((packed)) date_time {
  uint16_t ui16_year : 11; //[0,2047]
  uint8_t ui8_month : 4; //[0,15]
  uint8_t ui8_day_of_month : 5; //[0,31]
  uint8_t ui8_hour : 5; //[0,31]
  uint8_t ui8_minute : 6; //[0,63]
  uint8_t ui8_second : 6; //[0,63]
};

void set_date_time (
  struct date_time &dt,
  uint16_t ui16_year,
  uint8_t ui8_month,
  uint8_t ui8_day_of_month,
  uint8_t ui8_hour,
  uint8_t ui8_minute,
  uint8_t ui8_second
) {
  dt .ui16_year = ui16_year;
  dt .ui8_month = ui8_month;
  dt .ui8_day_of_month = ui8_day_of_month;
  dt .ui8_hour = ui8_hour;
  dt .ui8_minute = ui8_minute;
  dt .ui8_second = ui8_second;
}

void show_date_time ( const struct date_time &dt ) {
  std::ostream_iterator<char> cout { std::cout };

  std::vformat_to (
    cout,
    "{:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
    std::make_format_args (
      dt .ui16_year,
      dt .ui8_month,
      dt .ui8_day_of_month,
      dt .ui8_hour,
      dt .ui8_minute,
      dt .ui8_second
    )
  );
}

void pack_date_time (
  uint8_t *lpui8ary_date_time,
  uint16_t ui16_year, //8 + (32 - 4-5-5-6-6 = 6) = 14 bits = [0,2^14-1] = [0 -> 16,383]
  uint8_t ui8_month, //4 [0,15]
  uint8_t ui8_day_of_month, //5 [0,31]
  uint8_t ui8_hour, //5 [0,31]
  uint8_t ui8_minute, //6 [0,63]
  uint8_t ui8_second //6 [0,63]
) {
  memset (
    lpui8ary_date_time,
    0,
    5
  ); //40 bits

  uint16_t *lpui16_date_time = (uint16_t *) lpui8ary_date_time;
  uint16_t ui16_value = 0x0001;
  if ( (*(uint8_t *) &ui16_value) == 1 ) {
    //std::cout << "Little endian." << std::endl;
    *lpui16_date_time = ui16_year & ((1 << 14)-1); //14 bits --> 8 + 6
  }
  else {
    uint8_t *ui8_year_part1 = (uint8_t *) &ui16_year;
    uint8_t *ui8_year_part2 = ((uint8_t *) &ui16_year) + 1;
    std::swap ( *ui8_year_part1, *ui8_year_part2 );
    //std::cout << "Swapped year: " << ui16_year << std::endl;
    *lpui16_date_time = ui16_year & ((1 << 14)-1); //14 bits --> 8 + 6
  }

  lpui8ary_date_time [ 1 ] |= (ui8_month & ((1<<2)-1)) << 6;
  lpui8ary_date_time [ 2 ] = (ui8_month & ( ((1<<2)-1) << 2 ) ) >> 2; //2 bits taken of byte 3
  lpui8ary_date_time [ 2 ] |= (ui8_day_of_month & ((1<<5)-1)) << 2; //7 bits taken of byte 3
  lpui8ary_date_time [ 2 ] |= (ui8_hour & 0x01) << 7;
  //0000 1111 << 1 --> 0001 1110 --> 0001 1110 >> 1 = 0000 1111
  lpui8ary_date_time [ 3 ] = (ui8_hour & (((1<<4)-1) << 1) ) >> 1; //4 bits used of byte 4
  lpui8ary_date_time [ 3 ] |= (ui8_minute & ((1<<4)-1)) << 4; //4/6 minute bits used 8/8 bits of byte 4 used
  lpui8ary_date_time [ 4 ] = (ui8_minute & ( ((1<<2)-1) << 4) ) >> 4; //6/6 minute bits used 2/8 bits of byte 5 used.
  lpui8ary_date_time [ 4 ] |= (ui8_second & ((1<<6)-1)) << 2; //6/6 second bits used & 8/8 bits of byte 5 used.
}

void unpack_date_time (
  uint8_t *lpui8ary_date_time,
  struct date_time &dt
) {
  uint16_t *lpui16_date_time = (uint16_t *) lpui8ary_date_time;
  dt .ui16_year = *lpui16_date_time & ((1<<14)-1); //14 bits used = 8 + 6
  //dt .ui16_year = *lpui16_date_time;
  //get remaining 2 bits for part of the month
  dt .ui8_month = (lpui8ary_date_time [ 1 ] & ( ((1<<2)-1) << 6)) >> 6;

  dt .ui8_month |= (lpui8ary_date_time [ 2 ] & ((1<<2)-1)) << 2; //2 bits of byte 3 used
  dt .ui8_day_of_month = ( lpui8ary_date_time [ 2 ] & ( ((1<<5)-1) << 2 ) ) >> 2; //7 bits of byte 3 used
  dt .ui8_hour = (lpui8ary_date_time [ 2 ] & (1 << 7)) >> 7; //8/8 of byte 3 used 1/5 bits of hour used

  dt .ui8_hour |= (lpui8ary_date_time [ 3 ] & ((1<<4)-1)) << 1; //4 bits used from byte 4; 5/5 bits of hour used
  dt .ui8_minute = (lpui8ary_date_time [ 3 ] & (((1<<4)-1)<<4)) >> 4; //8/8 bits used from byte 4; 4/6 bits from minute used

  dt .ui8_minute |= (lpui8ary_date_time [ 4 ] & ((1<<2)-1)) << 4;  //2 bits used of byte 5; 6/6 bits used from minute
  dt .ui8_second = (lpui8ary_date_time [ 4 ] & ((1<<6)-1) << 2) >> 2; //6/6 bits used from second; 8/8 bit used from byte 5

}

int main ( void ) {
  struct some_structure s;

/*
  show_offset ( some_structure, ui32_some_number );
  show_offset ( some_structure, f );
  show_offset ( some_structure, ui8_small_number );
  show_offset ( some_structure, u_things );
  show_offset ( some_structure, ui16_slightly_bigger_number );
  show_offset ( some_structure, ui64_very_large_number );
*/

  show_offset ( some_structure, ui64_very_large_number );
  show_offset ( some_structure, u_things );
  show_offset ( some_structure, ui32_some_number );
  show_offset ( some_structure, f );
  show_offset ( some_structure, ui16_slightly_bigger_number );
  show_offset ( some_structure, ui8_small_number );


  struct date_time dt;
  set_date_time (
    dt,
    2024,
    8,
    19,
    12+5,
    47,
    0
  );
  show_date_time ( dt );
  std::cout << "Sizeof date_time in bytes: " <<
  sizeof ( struct date_time ) << ".\n";

/*
  ui16_year : 11; //[0,2047]
  ui8_month : 4; //[0,15]
  ui8_day_of_month : 5; //[0,31]
  ui8_hour : 5; //[0,31]
  ui8_minute : 6; //[0,63]
  ui8_second : 6; //[0,63]
*/

  uint8_t ui8ary_date_time [ 5 ] = { 0 }; //40 bits

  struct date_time dt_packed;
  pack_date_time (
    ui8ary_date_time,
    2024,
    8,
    19,
    12+6,
    38,
    13
  );
  //Output: 2024-08-19 18:38:13

  unpack_date_time ( ui8ary_date_time, dt_packed );
  show_date_time ( dt_packed );

  return 0;
}
