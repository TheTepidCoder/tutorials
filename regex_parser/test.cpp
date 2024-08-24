/*
 Created on 2024-08-16 by TheTepidCoder
 Purpose: To parse a log file from EQ
 to determine the DPS dealt, each fight.
*/
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <cstdint> //^u?int[8136][624]_t$

#include <chrono>
#include <ctime>

#include <format> //std::vformat_to
#include <ostream>
#include <iterator>

#include <string>
#include <unordered_map>
#include <map>

#include "month_hasher.h"

#ifndef _IN_
#define _IN_
#endif

#ifndef _OUT_
#define _OUT_
#endif

#ifndef _IN_OUT_
#define _IN_OUT_
#endif

class pumper {
  private:
    uint8_t m_ui8_month = 0;
    uint8_t m_ui8_day_of_month = 0;
    uint8_t m_ui8_hour = 0;
    uint8_t m_ui8_minute = 0;
    uint8_t m_ui8_second = 0;
    uint16_t m_ui16_year = 0;
    std::string m_str_actor;
    std::string m_str_method;
    uint64_t m_ui64_damage = 0;
    std::unordered_map<std::string, uint64_t> m_um_method_to_damage;

  public:
    pumper (  ); // = delete;
    pumper (
      uint16_t ui16_year,
      uint8_t ui8_month,
      uint8_t ui8_day_of_month,
      uint8_t ui8_hour,
      uint8_t ui8_minute,
      uint8_t ui8_second,
      const std::string &str_actor
    );
    ~pumper (  );
    void add_damage (
      const std::string &str_method,
      uint64_t ui64_damage
    ) noexcept;
    const std::unordered_map<std::string, uint64_t> &get_damage ( void ) const noexcept;
};
pumper::pumper (  ) {  }
pumper::pumper (
  uint16_t ui16_year,
  uint8_t ui8_month,
  uint8_t ui8_day_of_month,
  uint8_t ui8_hour,
  uint8_t ui8_minute,
  uint8_t ui8_second,
  const std::string &str_actor
) :
  m_ui16_year ( ui16_year ),
  m_ui8_month ( ui8_month ),
  m_ui8_day_of_month ( ui8_day_of_month ),
  m_ui8_hour ( ui8_hour ),
  m_ui8_minute ( ui8_minute ),
  m_ui8_second ( ui8_second ),
  m_str_actor ( str_actor )
{
}

pumper::~pumper (  ) {
}

void pumper::add_damage (
  const std::string &str_method,
  uint64_t ui64_damage
) noexcept {
  if ( m_um_method_to_damage .find ( str_method ) == m_um_method_to_damage .end (  ) ) {
    m_um_method_to_damage [ str_method ] = ui64_damage;
  }
  else {
    m_um_method_to_damage [ str_method ] += ui64_damage;
  }
}
const std::unordered_map<std::string, uint64_t> &pumper::get_damage ( void ) const noexcept {
  return m_um_method_to_damage;
}

/*
 time_t tt_then = create_date_time ( ... );
 double dbl_elapsed_seconds = difftime(time_t, time_t)
*/
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

bool parse_dps_line (
  _IN_ const month_hasher &mh_month_hasher,
  _IN_ const std::string &str_dps_line,
  _IN_ const std::regex &re_dps_regex,
  _OUT_ std::map<std::string, pumper> &map_pumpers,
  _OUT_ time_t &tt_timestamp
) {
  std::smatch matches;
  bool b_matched = std::regex_match (
    str_dps_line,
    matches,
    re_dps_regex
  );
  if ( ! b_matched ) {
    //std::cout << "The regular expression pattern didn't match the string.\n";
    return false;
  }

  uint8_t ui8_month = mh_month_hasher [
    mh_month_hasher .generate_month_hash ( matches [ 1 ] .str (  ) .c_str (  ) )
  ];
  uint8_t ui8_day_of_month = std::stoi ( matches [ 2 ] .str (  ) );
  uint8_t ui8_hour = std::stoi ( matches [ 3 ] .str (  ) );
  uint8_t ui8_minute = std::stoi ( matches [ 4 ] .str (  ) );
  uint8_t ui8_second = std::stoi ( matches [ 5 ] .str (  ) );
  uint16_t ui16_year = std::stoi ( matches [ 6 ] .str (  ) );
  std::string str_actor = matches [ 7 ] .str (  );
  std::string str_method = matches [ 8 ] .str (  );
  uint64_t ui64_damage = std::stoi ( matches [ 9 ] .str (  ) );

/*
  std::vformat_to (
    cout,
    "[{:04}-{:02}-{:02} {:02}:{:02}:{:02}] {} {} for {} points of damage.\n",
    std::make_format_args (
      ui16_year,
      ui8_month,
      ui8_day_of_month,
      ui8_hour,
      ui8_minute,
      ui8_second,
      str_actor,
      str_method,
      ui64_damage
    )
  );
*/
  std::map<std::string, pumper>::iterator it = map_pumpers .find ( str_actor );
  if ( it == map_pumpers .end (  ) ) {
/*
    map_pumpers .emplace ( //try_emplace will only try to add the new key/value, if it doesn't already exist.
      std::piecewise_construct,
      std::forward_as_tuple ( str_actor ),
      std::forward_as_tuple (
        ui16_year,
        ui8_month,
        ui8_day_of_month,
        ui8_hour,
        ui8_minute,
        ui8_second,
        str_actor
      )
    );
*/
    map_pumpers .try_emplace ( //try_emplace will only try to add the new key/value, if it doesn't already exist.
      str_actor,
      ui16_year,
      ui8_month,
      ui8_day_of_month,
      ui8_hour,
      ui8_minute,
      ui8_second,
      str_actor
    );
    it = map_pumpers .find ( str_actor );
  }
  it ->second .add_damage ( str_method, ui64_damage );

/*
  std::cout << "Year: " << (unsigned int) ui16_year << std::endl;
  std::cout << "Month: " << (unsigned int) ui8_month << std::endl;
  std::cout << "Day of the month: " << (unsigned int) ui8_day_of_month << std::endl;
  std::cout << "Hour: " << (unsigned int) ui8_hour << std::endl;
  std::cout << "Minute: " << (unsigned int) ui8_minute << std::endl;
  std::cout << "Second: " << (unsigned int) ui8_second << std::endl;
  std::cout << "Actor: \"" << str_actor << "\"" << std::endl;
  std::cout << "Method: \"" << str_method << "\"" << std::endl;
  std::cout << "Damage: " << (unsigned int) ui64_damage << std::endl;

  for ( const auto &match : matches ) {
    std::cout << "Capture group: \"" << match << "\"\n";
  }
*/
  tt_timestamp = create_date_time (
    ui16_year,
    ui8_month,
    ui8_day_of_month,
    ui8_hour,
    ui8_minute,
    ui8_second
  );

  return true;
}

bool parse_combat_end (
  _IN_ const month_hasher &mh_month_hasher,
  _IN_ const std::string &str_line,
  _IN_ const std::regex &re_slay_regex,
  _IN_ const std::regex &re_zone_regex,
  _OUT_ time_t &tt_timestamp
) {
 std::smatch matches;
 if ( ! std::regex_match ( str_line, matches, re_slay_regex ) ) {
   //std::cout << "Slay didn't match.\n";
   if ( ! std::regex_match ( str_line, matches, re_zone_regex ) ) {
     //std::cout << "Zone didn't match, either.\n";
     return false;
   }
 }

 //std::cout << "Either slay or zone matched.\n";

  uint8_t ui8_month = mh_month_hasher [
    mh_month_hasher .generate_month_hash ( matches [ 1 ] .str (  ) .c_str (  ) )
  ];
  uint8_t ui8_day_of_month = std::stoi ( matches [ 2 ] .str (  ) );
  uint8_t ui8_hour = std::stoi ( matches [ 3 ] .str (  ) );
  uint8_t ui8_minute = std::stoi ( matches [ 4 ] .str (  ) );
  uint8_t ui8_second = std::stoi ( matches [ 5 ] .str (  ) );
  uint16_t ui16_year = std::stoi ( matches [ 6 ] .str (  ) );

  tt_timestamp = create_date_time (
    ui16_year,
    ui8_month,
    ui8_day_of_month,
    ui8_hour,
    ui8_minute,
    ui8_second
  );

  return true;
}

bool parse_log_file (
  const char *lpsz_file_path,
  std::ostream_iterator<char> &cout
) {
  std::fstream fs { lpsz_file_path, std::ios_base::in | std::ios_base::binary };
  if ( ! fs .is_open (  ) ) {
    std::cerr << "Couldn't open \"" << lpsz_file_path << "\"\n";
    return false;
  }


  std::string str_slay_regex = R"#(\[\w+ (\w+) (\d+) (\d{2}):(\d{2}):(\d{2}) (\d{4})\] (\w+) (?:have|has) slain .*!)#";
  std::string str_zone_regex = R"#(\[\w+ (\w+) (\d+) (\d{2}):(\d{2}):(\d{2}) (\d{4})\] LOADING, PLEASE WAIT\.\.\.)#";
  std::regex re_slay_regex (
    str_slay_regex,
    std::regex_constants::ECMAScript | std::regex_constants::icase
  );
  std::regex re_zone_regex (
    str_zone_regex,
    std::regex_constants::ECMAScript | std::regex_constants::icase
  );

  std::string str_dps_regex = R"#(\[\w+ (\w+) (\d+) (\d{2}):(\d{2}):(\d{2}) (\d{4})\] (\w+) (\w+) [^\d]+(\d+) points of(?: non\-melee)? damage.)#";
  //std::cout << "\"" << str_dps_regex << "\"" << std::endl;
  //std::string str_dps_line = "[Thu Oct 18 10:29:52 2018] You slash a burnt protector for 13 points of damage.";
  std::regex re_dps_regex (
    str_dps_regex,
    std::regex_constants::ECMAScript | std::regex_constants::icase
  );


/*
  std::string str_slay = "[Thu Oct 18 10:54:14 2018] You have slain a fire goblin scout!";
  std::string str_zone = "[Thu Oct 18 10:54:37 2018] LOADING, PLEASE WAIT...";
{
  std::smatch slay_matches;
  if ( std::regex_match ( str_slay, slay_matches, re_slay_regex ) ) {
    std::cout << "The slay regular expression matched." << std::endl;
  }
}
{
  std::smatch zone_matches;
  if ( std::regex_match ( str_zone, zone_matches, re_zone_regex ) ) {
    std::cout << "The zone regular expression matched." << std::endl;
  }
}
*/


  month_hasher mh_month_hasher;
  std::map<std::string, pumper> map_pumpers;

  auto fn_dump_dps = [&] ( double dbl_elapsed_seconds ) -> void {
    if ( ! dbl_elapsed_seconds ) {
      dbl_elapsed_seconds = 1.0;
    }
    for ( const auto &[str_actor, p] : map_pumpers ) {
      const std::unordered_map<std::string, uint64_t> &um_method_to_damage = p .get_damage (  );
      std::cout << "Pumper actor: \"" << str_actor << "\" damaged with " << um_method_to_damage.size() << " methods.\n";

      for ( const auto &[str_method, ui64_damage] : um_method_to_damage ) {
        std::vformat_to (
          cout,
          "{} {} for {} points of damage ({:.1f} DPS over {:.0f} seconds).\n",
          std::make_format_args (
            str_actor,
            str_method,
            ui64_damage,
            (double) ui64_damage / dbl_elapsed_seconds,
            dbl_elapsed_seconds
          )
        );
      }
    }
  };


  bool b_in_combat = false;
  time_t tt_combat_start = { 0 }, tt_combat_end = { 0 };
  std::string str_line;
  while ( true ) {
    if ( ! std::getline ( fs, str_line ) ) {
      if ( fs .bad (  ) ) {
        std::cerr << "Error: Couldn't read from file.\n";
        return false;
      }

      break;
    }

    //std::cout << "Here's the line: \"" << str_line << "\"\n";
    if ( ! str_line .empty (  ) ) {
      char c_back = str_line .back (  );
      if ( c_back == '\r' || c_back == '\n' ) {
        str_line .erase ( str_line .size (  ) - 1, 1 );
      }
    }

    if ( b_in_combat ) {
      //std::cout << "WE'RE IN COMBAT.\n";

      //If someone/something was slain or the
      //logger zoned for some reason, then
      //we're done parsing this fight.
      bool b_combat_ended = parse_combat_end (
        mh_month_hasher,
        str_line,
        re_slay_regex,
        re_zone_regex,
        tt_combat_end
      );
      if ( b_combat_ended ) {
        std::cout << "Combat end detected.\n";

        double dbl_elapsed_seconds = difftime ( tt_combat_end, tt_combat_start );
        std::vformat_to (
          cout,
          "Elapsed seconds: {:.0f}\n",
          std::make_format_args (
            dbl_elapsed_seconds
          )
        );


        //!! TODO: Show the dps from all of the actors categorized by each of the damage sources/methods.
        fn_dump_dps ( dbl_elapsed_seconds );
        map_pumpers .clear (  ); //std::map<std::string, pumper>::clear()
        b_in_combat = false;

        //std::cout << "Zone or slay line was parsed.\n";

        continue;
      }
    }

    time_t tt_then;
    bool b_dps_line_parsed = parse_dps_line (
      mh_month_hasher,
      str_line,
      re_dps_regex,
      map_pumpers,
      tt_then
    );
    if ( b_dps_line_parsed ) {
      if ( ! b_in_combat ) {
        memcpy ( &tt_combat_start, &tt_then, sizeof(time_t) );
      }

      b_in_combat = true;
      //std::cout << "DPS line was parsed.\n";
      continue; //Skip any other checks, in case more are
      //added for things like looting or group/raid/guild/...etc. chat
    }

  } //while we're still parsing lines from the file.


  return true;
}

int main (
  int argc,
  char *argv [  ] //test.exe 1st_arg 2nd_arg
) {
  if ( argc < 2 ) {
    std::cerr << "Error: " << argv [ 0 ] << " expects one arguments (the path to the file to parse).\n";
    return 0;
  }
  std::ostream_iterator<char> cout { std::cout };
  std::vformat_to ( cout, "This is only a {}.\n", std::make_format_args ( "test" ) );

  parse_log_file ( argv [ 1 ], cout );

  return 0;
}
