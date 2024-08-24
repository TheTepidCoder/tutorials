/*
 Created on 2024-08-16 by TheTepidCoder
 Purpose: To show how to get the number of
 elapsed seconds between two date/times in
 C and C++
*/
#include <iostream>
#include <chrono>
#include <ctime>

#include <format> //std::vformat_to
#include <ostream>
#include <iterator>

#include <cstdint>

time_t create_date_time (
  uint16_t ui16_year,
  uint8_t ui8_month,
  uint8_t ui8_month_day,
  uint8_t ui8_millitary_hour,
  uint8_t ui8_minute,
  uint8_t ui8_second
) {
  struct tm tm_timestamp = { 0 };
  tm_timestamp .tm_year = ui16_year - 1'900;
  tm_timestamp .tm_mon = ui8_month - 1;
  tm_timestamp .tm_mday = ui8_month_day;
  tm_timestamp .tm_hour = ui8_millitary_hour;
  tm_timestamp .tm_min = ui8_minute;
  tm_timestamp .tm_sec = ui8_second;
  tm_timestamp .tm_isdst = -1;
  return mktime ( &tm_timestamp );
}

int main ( void ) {

  std::ostream_iterator<char> cout { std::cout };

  time_t tt_timestamp = create_date_time ( 2'000, 8, 16, 12+3, 45, 0 );

  std::chrono::system_clock::time_point tp_now = std::chrono::system_clock::now (  ); //Current timestamp
  std::chrono::system_clock::time_point tp_then = //timestamp from explicitly defined date/time.
    std::chrono::system_clock::from_time_t ( tt_timestamp );

  std::chrono::duration<double> dur_elapsed_seconds =
    std::chrono::duration_cast<std::chrono::seconds> ( tp_now - tp_then );

  std::vformat_to (
    cout,
    "Elapsed seconds {:.0f}.\n",
    std::make_format_args (
      dur_elapsed_seconds .count (  )
    )
  );

  time_t tt_now = time ( nullptr ); //std::chrono::system_clock::now() but for time_t

  std::vformat_to (
    cout,
    "Elapsed seconds {:.0f}.\n",
    std::make_format_args (
      difftime ( tt_now, tt_timestamp )
    )
  );

  return 0;
}
