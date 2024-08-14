/*
 Created on 2024-08-08 by TheTepidCoder
 Purpose: To be a common dependency of
 both our crash project and our chatroom
 DLL project. By changing the size of
 classes/structures or function/method
 signatures between versions that either
 the crash project or chatroom DLL project
 expect and depend upon, we may cause
 undefined behavior via ABI-breaking
 changes. (ABI = application binary interface)
*/
#pragma once
#ifndef DATE_TIME_UTILITY
#define DATE_TIME_UTILITY

#ifndef DATE_TIME_UTILITY_IMPORT_OR_EXPORT
#  ifdef BUILDING_DATE_TIME_UTILITY
#    define DATE_TIME_UTILITY_IMPORT_OR_EXPORT __declspec(dllexport)
#  else
#    define DATE_TIME_UTILITY_IMPORT_OR_EXPORT __declspec(dllimport)
#  endif
#endif

#include <ctime> //struct tm (time structure); time_t (timestamp)
#include <cstdint>
#include <fstream>

//Either import or export the class, depending on whether or not
//the preprocessor define BUILDING_DATE_TIME_UTILITY is present
//at compile time.
class DATE_TIME_UTILITY_IMPORT_OR_EXPORT date_time {
  private:
    uint32_t m_ui32_year = 0;
    uint8_t m_ui8_month = 0;
    uint8_t m_ui8_day = 0;
    uint8_t m_ui8_hour = 0;
    uint8_t m_ui8_minute = 0;
    uint8_t m_ui8_second = 0;

    uint16_t m_ui16_millisecond = 0;
    uint8_t m_ui8_day_of_week = 0;
    uint16_t m_ui16_day_of_year = 0;
    bool m_b_is_daylight_savings_time = false;

  public:
    date_time (  );
    ~date_time (  );
    uint32_t year ( void ) const noexcept;
    uint8_t month ( void ) const noexcept;
    uint8_t day ( void ) const noexcept;
    uint8_t hour ( void ) const noexcept;
    uint8_t minute ( void ) const noexcept;
    uint8_t second ( void ) const noexcept;

    uint16_t millisecond ( void ) const noexcept;
    uint8_t day_of_week ( void ) const noexcept;
    uint16_t day_of_year ( void ) const noexcept;
    bool is_daylight_savings_time ( void ) const noexcept;

    [[nodiscard]] bool save ( const char *lpsz_file_path ) const noexcept;
    [[nodiscard]] bool load ( const char *lpsz_file_path ) noexcept;
};

#endif
