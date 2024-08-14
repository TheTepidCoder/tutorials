//To build:
// g++ -shared -DBUILDING_DATE_TIME_UTILITY=1 date_time_utility.cpp -o date_time_utility
#include "date_time_utility.h"

date_time::date_time (  ) {
  time_t tt_timestamp = time ( nullptr );
  struct tm tm_time = *gmtime ( &tt_timestamp );
  m_ui32_year = tm_time .tm_year + 1900;
  m_ui8_month = tm_time .tm_mon;
  m_ui8_day = tm_time .tm_mday; //day of the month

  m_ui8_hour = tm_time .tm_hour;
  m_ui8_minute = tm_time .tm_min;
  m_ui8_second = tm_time .tm_sec;

  //tm_time .tm_wday (day of the week 0 ->6 from sunday to saturday)
  //tm_time .tm_yday (day of the year 0 ->365; 0 = January 1st)
  //tm_time .tm_isdst (is daylight savings time [0 or 1] for false/true)
}
date_time::~date_time (  ) {
}

uint32_t date_time::year ( void ) const noexcept {
  return m_ui32_year;
}
uint8_t date_time::month ( void ) const noexcept {
  return m_ui8_month;
}
uint8_t date_time::day ( void ) const noexcept {
  return m_ui8_day;
}
uint8_t date_time::hour ( void ) const noexcept {
  return m_ui8_hour;
}
uint8_t date_time::minute ( void ) const noexcept {
  return m_ui8_minute;
}
uint8_t date_time::second ( void ) const noexcept {
  return m_ui8_second;
}

//uint8_t date_time::millisecond ( void ) const noexcept {
//  return m_ui8_millisecond;
//}
//uint8_t date_time::day_of_week ( void ) const noexcept {
//  return m_ui8_day_of_week;
//}
//uint8_t date_time::day_of_year ( void ) const noexcept {
//  return m_ui16_day_of_year;
//}
//bool date_time::is_daylight_savings_time ( void ) const noexcept {
//  return m_b_is_daylight_savings_time;
//}
[[nodiscard]] bool date_time::save ( const char *lpsz_file_path ) const noexcept {
  std::fstream fsout { lpsz_file_path, fsout.binary | fsout.trunc | fsout.out };
  if ( ! fsout .is_open (  ) ) {
    return false;
  }

  fsout .write ( reinterpret_cast<const char *> (&m_ui32_year), sizeof(uint32_t) );
  fsout .write ( reinterpret_cast<const char *> (&m_ui8_month), sizeof(uint8_t) );
  fsout .write ( reinterpret_cast<const char *> (&m_ui8_day), sizeof(uint8_t) );
  fsout .write ( reinterpret_cast<const char *> (&m_ui8_hour), sizeof(uint8_t) );
  fsout .write ( reinterpret_cast<const char *> (&m_ui8_minute), sizeof(uint8_t) );
  fsout .write ( reinterpret_cast<const char *> (&m_ui8_second), sizeof(uint8_t) );
  //fsout .write ( reinterpret_cast<const char *> (&m_ui8_millisecond), sizeof(uint8_t) );

  //fsout .write ( reinterpret_cast<const char *> (&m_ui8_day_of_week), sizeof(uint8_t) );
  //fsout .write ( reinterpret_cast<const char *> (&m_ui16_day_of_year), sizeof(uint8_t) );
  //fsout .write ( reinterpret_cast<const char *> (&m_b_is_daylight_savings_time), sizeof(uint8_t) );

  return true;
}
[[nodiscard]] bool date_time::load ( const char *lpsz_file_path ) noexcept 
{
  std::fstream fsin { lpsz_file_path, fsin.binary | fsin.in };
  if ( ! fsin .is_open (  ) ) {
    return false;
  }

  fsin .read ( reinterpret_cast<char *> (&m_ui32_year), sizeof(uint32_t) );
  fsin .read ( reinterpret_cast<char *> (&m_ui8_month), sizeof(uint8_t) );
  fsin .read ( reinterpret_cast<char *> (&m_ui8_day), sizeof(uint8_t) );
  fsin .read ( reinterpret_cast<char *> (&m_ui8_hour), sizeof(uint8_t) );
  fsin .read ( reinterpret_cast<char *> (&m_ui8_minute), sizeof(uint8_t) );
  fsin .read ( reinterpret_cast<char *> (&m_ui8_second), sizeof(uint8_t) );
  //fsin .read ( reinterpret_cast<char *> (&m_ui8_millisecond), sizeof(uint8_t) );

  //fsin .read ( reinterpret_cast<char *> (&m_ui8_day_of_week), sizeof(uint8_t) );
  //fsin .read ( reinterpret_cast<char *> (&m_ui16_day_of_year), sizeof(uint8_t) );
  //fsin .read ( reinterpret_cast<char *> (&m_b_is_daylight_savings_time), sizeof(uint8_t) );

  return true;
}
