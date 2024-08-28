/*
 Created on 2024-08-20 by TheTepidCoder
MIT License

Copyright (c) 2024 Jacob (TheTepidCoder)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "sqlite3.h"
#include <iostream>

int main ( void ) {
  sqlite3 *db = nullptr;

  int i_ret = sqlite3_open ( "some_database.dat", &db );
  if ( i_ret != SQLITE_OK ) {
    std::cerr << "Error: \"" << sqlite3_errmsg ( db ) << "\"\n";
    return 0;
  }

  const char *lpsz_query = R"#(
insert into `users` (
  `str_name`,
  `i64_age`,
  `dbl_height`
) values (
  ?,
  ?,
  ?
);
)#";
  sqlite3_stmt *stmt = nullptr;
  i_ret = sqlite3_prepare_v2 (
    db,
    lpsz_query,
    -1, //until null-terminator is reached
    &stmt,
    nullptr //would redirect a &(char*) to reference the first character after the end of the query that was parsed.
  );
  if ( i_ret != SQLITE_OK ) {
    sqlite3_close ( db );
    return 0;
  }

  sqlite3_bind_text ( stmt, 1, "SomeDude", -1, SQLITE_STATIC );
  sqlite3_bind_int64 ( stmt, 2, -17 );
  sqlite3_bind_double ( stmt, 3, 99.2 );

  i_ret = sqlite3_step ( stmt );
  if ( i_ret != SQLITE_ROW && i_ret != SQLITE_DONE ) {
    std::cerr << "Error: sqlite3_step failed with message: \"" << sqlite3_errmsg ( db ) << "\"\n";
  }
  else {
    std::cout << "A new row was inserted into the database's users table.\n";
  }

  sqlite3_finalize ( stmt );

  sqlite3_close ( db );

  return 0;
}
