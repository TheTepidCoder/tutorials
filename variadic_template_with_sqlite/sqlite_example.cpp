/*
 Created on 2024-08-19 by TheTepidCoder
 Purpose: To allow for type-safe SQLite3 database
 inserts which use dynamically chosen bindings
 generated at compile-time in prepared statements.
 Any extra arguments would cause an SQLite3 run-time
 complaint -- as would having too few of them.
 All that's missing is BLOB type handling. This
 could use something like a type specialization
 for std::vector<uint8_t>.

 //To build:
 // g++ test.cpp -l:libsqlite3.a -o test

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
#include <sqlite3.h>
#include <iostream>
#include <type_traits>
#include <cstdint> //u?int\d{1,2}_t
#include <string>
#include <vector>

#include <cstdio>

namespace ns_sqlite_types {
enum sqlite_types : uint8_t {
  unknown = 0, //unspecified
  blob = 1, //std::vector<uint8_t>
  integer = 2, //int64_t
  real = 3, //double
  text = 4 //std::string
};
}

class sqlite_type {
  private:
    union {
      std::string *lpstr; //TEXT / VARCHAR(#)
      int64_t *lpi64; //int64 (INTEGER)
      double *lpdbl; //double (REAL)
      std::vector<uint8_t> *lpvec; //BLOB
    } m_u_type;
    ns_sqlite_types::sqlite_types m_ste_sqlite_type = ns_sqlite_types::unknown;

  public:
    sqlite_type (  ) = delete;
    sqlite_type ( int64_t &i64 );
    sqlite_type ( double &dbl );
    sqlite_type ( std::vector<uint8_t> &vec );
    sqlite_type ( std::string &str );
    ~sqlite_type (  );

    sqlite_type &operator= ( const unsigned char * );
    void assign_blob ( const void *, size_t );
    sqlite_type &operator= ( const int64_t & );
    sqlite_type &operator= ( const double & );

    ns_sqlite_types::sqlite_types type ( void ) const noexcept;
};

sqlite_type::sqlite_type (
  int64_t &i64
) :
  m_ste_sqlite_type ( ns_sqlite_types::integer ),
  m_u_type { .lpi64 = &i64 }
{
  fprintf ( stdout, "i64: %p\n", &i64 );
}

sqlite_type::sqlite_type (
  double &dbl
) :
  m_ste_sqlite_type ( ns_sqlite_types::real ),
  m_u_type { .lpdbl = &dbl }
{
  fprintf ( stdout, "dbl: %p\n", &dbl );
}

sqlite_type::sqlite_type (
  std::vector<uint8_t> &vec
) :
  m_ste_sqlite_type ( ns_sqlite_types::blob ),
  m_u_type { .lpvec = &vec }
{
  fprintf ( stdout, "vec: %p\n", &vec );
}

sqlite_type::sqlite_type (
  std::string &str
) :
  m_u_type { .lpstr = &str },
  m_ste_sqlite_type ( ns_sqlite_types::text )
{
  fprintf ( stdout, "str: %p\n", &str );
}

sqlite_type::~sqlite_type (  ) {
  if ( m_ste_sqlite_type == ns_sqlite_types::blob ) {
    delete m_u_type .lpvec;
  }
  if ( m_ste_sqlite_type == ns_sqlite_types::text ) {
    delete m_u_type .lpstr;
  }
}

sqlite_type &sqlite_type::operator= ( const unsigned char *lpsz_string ) {
  *m_u_type .lpstr = (const char *) lpsz_string;
  return *this;
}
void sqlite_type::assign_blob ( const void *lpv_data, size_t st_size ) {
  *m_u_type .lpvec = std::vector<uint8_t> (
    (const uint8_t*) lpv_data,
    (const uint8_t*) lpv_data + st_size
  );
}
sqlite_type &sqlite_type::operator= ( const int64_t &i64 ) {
  *m_u_type .lpi64 = i64;
  return *this;
}
sqlite_type &sqlite_type::operator= ( const double &dbl ) {
  *m_u_type .lpdbl = dbl;
  return *this;
}
ns_sqlite_types::sqlite_types sqlite_type::type ( void ) const noexcept {
  return m_ste_sqlite_type;
}

///////


template<typename T, typename ...Targs>
bool bind_query_arguments (
  sqlite3_stmt *stmt,
  int i_one_based_column_index,
  T t_value,
  Targs ...fargs
) {
  //std::cout << "Type identifier: \"" << typeid ( T ).name() << "\"\n";
  if constexpr ( std::is_floating_point<T>::value ) {
    std::cout << "Binding a floating-point type.\n";
    if ( SQLITE_OK != sqlite3_bind_double ( stmt, i_one_based_column_index, static_cast<double> ( t_value ) ) ) {
      return false;
    }
  }
  else if constexpr ( std::is_integral<T>::value ) {
    std::cout << "Binding an integer type.\n";
    if ( SQLITE_OK != sqlite3_bind_int64 ( stmt, i_one_based_column_index, t_value ) ) {
      return false;
    }
  }
  else if constexpr ( std::is_same<T, const std::string &>::value ) {
    if (
      SQLITE_OK != sqlite3_bind_text (
        stmt,
        i_one_based_column_index,
        t_value,
        -1,
        SQLITE_STATIC
      )
    ) {
      return false;
    }
  }
  else if constexpr ( std::is_same<T, const char *>::value ) {
    std::cout << "Binding a const char * type.\n";
    if (
      SQLITE_OK != sqlite3_bind_text (
        stmt,
        i_one_based_column_index,
        t_value,
        -1,
        SQLITE_STATIC
      )
    ) {
      return false;
    }
  }

  if constexpr ( sizeof...(fargs) ) {
    std::cout << "There are " << (unsigned int) sizeof...(fargs) <<
    " variadic template arguments left" << std::endl;
    return bind_query_arguments (
      stmt,
      i_one_based_column_index + 1,
      fargs...
    );
    //show_types ( fargs... );
  }

  return true;
}

template<typename ...Targs>
bool update (
  sqlite3 *db,
  const char *lpsz_query,
  Targs ...fargs
) {
  sqlite3_stmt *stmt = nullptr;

  int i_ret = sqlite3_prepare_v2 (
    db,
    lpsz_query,
    -1, //read until first 0 (null terminator)
    &stmt,
    nullptr //would point to first character of unused sql (past ';' character).
  );
  if ( i_ret != SQLITE_OK ) {
    std::cerr << "Error: \"" << sqlite3_errmsg ( db ) << "\"." << std::endl;
    return false;
  }

  if constexpr ( sizeof...(fargs) ) {
    std::cout << "There were " << (sizeof...(fargs)) << " variadic arguments passed.\n";
    if (
      ! bind_query_arguments (
        stmt,
        1, //one based column index
        fargs...
      )
    ) {
      std::cerr << "Error: Failed to bind one or more arguments.\n";
      std::cerr << sqlite3_errmsg ( db ) << std::endl;
      sqlite3_finalize ( stmt );
      return false;
    }
  }

  i_ret = sqlite3_step ( stmt );
  if ( i_ret != SQLITE_ROW && i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Stepping through SQLite3 statment failed." << std::endl;
    return false;
  }

  //sqlite3_reset ( stmt );
  sqlite3_finalize ( stmt );

  return true;
}

template<typename ...Targs>
int64_t insert (
  sqlite3 *db,
  const char *lpsz_query,
  Targs ...fargs
) {
  sqlite3_stmt *stmt = nullptr;

  int i_ret = sqlite3_prepare_v2 (
    db,
    lpsz_query,
    -1, //read until first 0 (null terminator)
    &stmt,
    nullptr //would point to first character of unused sql (past ';' character).
  );
  if ( i_ret != SQLITE_OK ) {
    std::cerr << "Error: \"" << sqlite3_errmsg ( db ) << "\"." << std::endl;
    return false;
  }

  if constexpr ( sizeof...(fargs) ) {
    std::cout << "There were " << (sizeof...(fargs)) << " variadic arguments passed.\n";
    if (
      ! bind_query_arguments (
        stmt,
        1, //one based column index
        fargs...
      )
    ) {
      std::cerr << "Error: Failed to bind one or more arguments.\n";
      std::cerr << sqlite3_errmsg ( db ) << std::endl;
      sqlite3_finalize ( stmt );
      return false;
    }
  }

  i_ret = sqlite3_step ( stmt );
  if ( i_ret != SQLITE_ROW && i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Stepping through SQLite3 statment failed." << std::endl;
    return false;
  }

  //sqlite3_reset ( stmt );
  sqlite3_finalize ( stmt );

  return sqlite3_last_insert_rowid ( db );
}

template<typename ...Targs>
bool select (
  sqlite3 *db,
  const char *lpsz_query,
  std::vector<sqlite_type> &vec_vec_output_args,
  Targs ...fargs
) {
  sqlite3_stmt *stmt = nullptr;

  int i_ret = sqlite3_prepare_v2 (
    db,
    lpsz_query,
    -1, //read until first 0 (null terminator)
    &stmt,
    nullptr //would point to first character of unused sql (past ';' character).
  );
  if ( i_ret != SQLITE_OK ) {
    std::cerr << "Error: \"" << sqlite3_errmsg ( db ) << "\"." << std::endl;
    return false;
  }

  if constexpr ( sizeof...(fargs) ) {
    std::cout << "There were " << (sizeof...(fargs)) << " variadic arguments passed.\n";
    if (
      ! bind_query_arguments (
        stmt,
        1, //one based column index
        fargs...
      )
    ) {
      std::cerr << "Error: Failed to bind one or more arguments.\n";
      std::cerr << sqlite3_errmsg ( db ) << std::endl;
      sqlite3_finalize ( stmt );
      return false;
    }
  }

  i_ret = sqlite3_step ( stmt );
  if ( i_ret != SQLITE_ROW && i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Stepping through SQLite3 statment failed." << std::endl;
    return false;
  }

//!!!!!
  size_t st_column_index = 0;
  for ( auto &output_arg : vec_vec_output_args ) {
    switch ( output_arg .type() ) {
      case ns_sqlite_types::sqlite_types::integer: {
std::cout << "Assigning integer.\n" << std::flush;
        output_arg = sqlite3_column_int64 ( stmt, st_column_index );
        break;
      }
      case ns_sqlite_types::sqlite_types::real: {
std::cout << "Assigning real.\n" << std::flush;
        output_arg = sqlite3_column_double ( stmt, st_column_index );
        break;
      }
      case ns_sqlite_types::sqlite_types::text: {
std::cout << "Assigning text.\n" << std::flush;
        output_arg = sqlite3_column_text ( stmt, st_column_index );
        break;
      }
      case ns_sqlite_types::sqlite_types::blob: {
std::cout << "Assigning blob.\n" << std::flush;
        size_t st_size_bytes = sqlite3_column_bytes ( stmt, st_column_index );
        output_arg .assign_blob (
          sqlite3_column_blob ( stmt, st_column_index ),
          st_size_bytes
        );
        break;
      }
      case ns_sqlite_types::sqlite_types::unknown:
      default: {
        std::cerr << "Error: Unknown type for SQLite3 output argument.\n";
        sqlite3_finalize ( stmt );
        return false;
      } //default case
    } //switch for each type of argument

    st_column_index ++;
  } //for each output argument
//!!

  //sqlite3_reset ( stmt );
  sqlite3_finalize ( stmt );

std::cout << "Done with select function.\n" << std::flush;

  return true;
}








int main ( void ) {
  sqlite3 *db = nullptr;

  int i_ret = sqlite3_open (
    "some_database.dat",
    &db
  );
  if ( i_ret != SQLITE_OK ) {
    std::cerr << "Error: Couldn't open the database file.\n";
    return 0;
  }

  std::cout << "Success." << std::endl;


  bool b_success = update (
    db,
    R"#(create table if not exists `users` (
`str_name` text,
`i64_age` integer,
`dbl_height` real,
`id` integer primary key autoincrement
);)#"
  );
  if ( ! b_success ) {
    std::cerr << "Error: Failed to create the table, \"users.\"\n";
    sqlite3_close ( db );
    return 0;
  }

  std::cout << "Table created." << std::endl;


  int64_t i64_new_record_identifier = insert (
    db,
    R"#(insert into `users` (
  `str_name`,
  `i64_age`,
  `dbl_height`
) values (
  ?, -- str_name
  ?, -- i64_age
  ?  -- dbl_height
);)#",
    "BetterCoder",
    104,
    4.2f
  );
  if ( ! b_success ) {
    std::cerr << "Error: Failed to insert a new row into the users table.\n";
    sqlite3_close ( db );
    return 0;
  }
  std::cout << "New record identifier: " << (int) i64_new_record_identifier << std::endl;

  std::string str_name;
  int64_t i64_age;
  double dbl_height;
  //std::vector<uint8_t> vec_blob;
  std::vector<sqlite_type> vec_types;
  fprintf (
    stdout,
    "str: %p, i64: %p, dbl: %p.\n",
    &str_name,
    &i64_age,
    &dbl_height
  );
  vec_types .emplace_back ( str_name );
  vec_types .emplace_back ( i64_age );
  vec_types .emplace_back ( dbl_height );

  select (
    db,
    R"#(select * from `users` where `str_name` = ?;)#",
    vec_types,
    "TheTepidCoder"
  );
/*
  select (
    db,
    R"#(select count(*) from `users` where `str_name` = ?;)#",
    "TheTepidCoder"
  );
*/
  if ( ! b_success ) {
    std::cerr << "Error: Failed to count the rows with the user name TheTepidCoder.\n";
    sqlite3_close ( db );
    return 0;
  }
  std::cout << "Returned text: \"" << str_name << "\"\n";
  std::cout << "Returned integer: \"" << i64_age << "\"\n";
  std::cout << "Returned real: \"" << dbl_height << "\"\n";


  sqlite3_close ( db );

  return 0;
}
