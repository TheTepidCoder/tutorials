/*
 Created on 2024-08-16 by TheTepidCoder
 Purpose: To parse a log file from EQ
 to determine the DPS dealt, each fight.
*/
#include <cstdint> //^u?int[8136][624]_t$
#include "month_hasher.h"

uint8_t month_hasher::operator[] ( uint16_t ui16_abbreviated_month_hash ) const noexcept {
  return m_ui8ary_month_hash_to_month_index [ ui16_abbreviated_month_hash ];
}

month_hasher::month_hasher ( ) {
  _generate_month_character_value_map (  );
  _generate_month_hashes (  );
}
month_hasher::~month_hasher (  ) {
}
void month_hasher::_generate_month_character_value_map ( void ) {
  static const char cary_set [ 22 ] = {
    'A', 'D', 'F', 'J', 'M', 'N', 'O', 'S',
    'a', 'b', 'c', 'e', 'g', 'l', 'n', 'o',
    'p', 'r', 't', 'u', 'v', 'y'
  };

  for (
    uint8_t ui8_i = 0;
    ui8_i < element_count ( cary_set );
    ui8_i ++
  ) {
    m_ui8ary_month_character_value_map [ cary_set [ ui8_i ] ] = ui8_i;
  }
}

//This accepts a three-character month abbreviation
//and returns a unique hash that represents this abbreviation.
uint16_t month_hasher::generate_month_hash (
  const char *lpsz_month
) const noexcept {

  //std::cout << "Characters in set: " << (unsigned int) element_count ( cary_set ) << std::endl;

  uint16_t ui16_value = 0;
  uint16_t ui16_multiplier = 1;
  for ( const char *p = lpsz_month; *p; p ++ ) {
    ui16_value += m_ui8ary_month_character_value_map [ *p ] * ui16_multiplier;
    //std::cout << "Current value: " << (unsigned int) ui16_value << "\n";
    ui16_multiplier *= 22;
    //+ lpsz_month [ 1 ]*22 + lpsz_month [ 2 ]*(22*22)
  }
  //std::cout << "Here's the hash of the month: " << (unsigned int) ui16_value << "\n";

  return ui16_value;
}

void month_hasher::_generate_month_hashes ( void ) {

  _generate_month_character_value_map (  );

  static const char *lpszary_month_abbreviations[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
  };

  uint8_t ui8_index = 0;
  for (
    uint8_t ui8_index = 0;
    ui8_index < element_count ( lpszary_month_abbreviations );
    ui8_index ++
  ) {
    m_ui8ary_month_hash_to_month_index [
      generate_month_hash ( lpszary_month_abbreviations [ ui8_index ] )
    ] = ui8_index;
  }
}
