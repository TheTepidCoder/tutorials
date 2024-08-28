/*
 Created on 2024-08-19 by TheTepidCoder
 Purpose: To allow for type-safe SQLite3 database
 queries which use dynamically chosen input and
 output bindings, generated at compile-time,
 using prepared statements.

 Any extra arguments would cause an SQLite3 run-time
 complaint -- as would having too few of them.

 Currently, the following types are supported:
 -input
   const char* --> TEXT
   const std::string & --> TEXT
   (any detected integer type) E.g. int64_t. --> INTEGER
   (any detected floating point type) E.g. double/float --> REAL
   const std::vector<uint8_t>& (Treated as a blob.) (**untested**)
 -Output
   std::string &
   std::vector<uint8_t> & (**untested**)
   int64_t &
   double &

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

#include <memory>
#include <stdexcept>
#include <format>
#include <functional>

class sqlite_database {
  private:
    sqlite3 *m_db = nullptr;

  public:
    sqlite_database (  ) = delete;
    sqlite_database ( const char *lpsz_database_path );
    ~sqlite_database (  );

    //get last insert row id
    int64_t get_last_inserted_record_identifier ( void );
    int64_t get_updated_record_count ( void );
    const char *get_last_error ( void ) const noexcept;
    sqlite3 *get_database ( void );
};
sqlite_database::~sqlite_database (  ) {
  sqlite3_close ( m_db );
}
int64_t sqlite_database::get_updated_record_count ( void ) {
  return sqlite3_changes64 ( m_db );
}
int64_t sqlite_database::get_last_inserted_record_identifier ( void ) {
  return sqlite3_last_insert_rowid ( m_db );
}
sqlite3 *sqlite_database::get_database ( void ) {
  return m_db;
}

const char *sqlite_database::get_last_error ( void ) const noexcept {
  return sqlite3_errmsg ( m_db );
}

sqlite_database::sqlite_database (
  const char *lpsz_database_path
) {
  int i_ret = sqlite3_open (
    lpsz_database_path,
    &m_db
  );

  if ( i_ret != SQLITE_OK ) {
    std::string str_error = std::vformat (
      "Error: Couldn't open the database file: \"{}\"",
      std::make_format_args (
        lpsz_database_path
      )
    );
    throw std::runtime_error ( str_error );
  }
}

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

template<typename T, typename ...Targs>
bool retrieve_query_columns (
  sqlite3_stmt *stmt,
  size_t st_zero_based_column_index,
  T &t_value,
  Targs &...fargs
) {
  if constexpr ( std::is_integral<T>::value ) {
    //std::cout << "Assigning integer.\n" << std::flush;
    t_value = sqlite3_column_int64 ( stmt, st_zero_based_column_index );
  }
  else if constexpr ( std::is_floating_point<T>::value ) {
    //std::cout << "Assigning real.\n" << std::flush;
    t_value = sqlite3_column_double ( stmt, st_zero_based_column_index );
  }
  else if constexpr ( std::is_same<T, std::string>::value ) {
    //std::cout << "Assigning text.\n" << std::flush;
    t_value = (const char *) sqlite3_column_text ( stmt, st_zero_based_column_index );
  }
  else if constexpr ( std::is_same<T, std::vector<uint8_t>>::value ) {
    //std::cout << "Assigning blob.\n" << std::flush;
    size_t st_size_bytes = sqlite3_column_bytes ( stmt, st_zero_based_column_index );
    uint8_t *lpui8_s = (uint8_t *) sqlite3_column_blob ( stmt, st_zero_based_column_index );
    uint8_t *lpui8_e = lpui8_s + st_size_bytes;
    t_value .assign (
       lpui8_s, //[lpui8_s, lpui8_e)
      lpui8_e //sentinel
    );
  }
  else {
    std::cerr << "Error: Unknown type for SQLite3 output argument.\n";
    sqlite3_finalize ( stmt );
    return false;
  }

  //If there are more columns to retrieve.
  if constexpr ( sizeof...(fargs) ) {
    return retrieve_query_columns ( stmt, st_zero_based_column_index + 1, fargs... );
  }

  return true;
}


class sqlite_statement {
  private:
    std::shared_ptr<sqlite_database> m_sptr_db;
    sqlite3_stmt *m_stmt = nullptr;
    std::string m_str_query;

  public:
    sqlite_statement (  ) = delete;
    sqlite_statement ( std::shared_ptr<sqlite_database> sptr_db, const char *lpsz_query );
    ~sqlite_statement (  );
    void reset ( void ); //Call this between steps after hitting SQLITE_DONE to bind new input arguments.
    void finalize ( void );
    int step ( void );

    template<typename T, typename ...Targs>
    bool bind_arguments ( const T &t_value, const Targs &...fargs );

    template<typename T, typename ...Targs>
    bool retrieve_columns ( T& t_value, Targs &...fargs );
    sqlite3_stmt *get_statement ( void );

    template <typename ...Targs>
    bool foreach_row ( std::function<bool(void)> fn_row_callback, Targs &...fargs );
};

void sqlite_statement::reset ( void ) {
  sqlite3_reset ( m_stmt );
}

template <typename ...Targs>
bool sqlite_statement::foreach_row ( std::function<bool(void)> fn_row_callback, Targs &...fargs ) {
  int i_ret;
  bool b_success;
  while ( true ) {
    std::cout << "Retrieving returned columns." << std::endl << std::flush;

    i_ret = step (  );
    if ( i_ret == SQLITE_ROW ) {

      b_success = retrieve_columns ( fargs... );
      if ( ! b_success ) {
        std::cerr << "Error: Failed to retrieve the returned columns.\n";
        return false;
      }

      if ( ! fn_row_callback (  ) ) {
        return false;
      }
    }
    else if ( i_ret == SQLITE_DONE ) {
      break;
    }
    else {
      std::cerr << "Error: Stepping through SQLite3 statment failed." << std::endl;
      finalize (  );
      return false;
    }
  }

  return true;
}

template<typename T, typename ...Targs>
bool sqlite_statement::bind_arguments ( const T &t_value, const Targs &...fargs ) {
  return bind_query_arguments (
    m_stmt,
    1,
    t_value,
    fargs...
  );
}

template<typename T, typename ...Targs>
bool sqlite_statement::retrieve_columns ( T &t_value, Targs &...fargs ) {
  return retrieve_query_columns ( m_stmt, 0, t_value, fargs... );
}

sqlite3_stmt *sqlite_statement::get_statement ( void ) {
  return m_stmt;
}
void sqlite_statement::finalize ( void ) {
  if ( m_stmt ) {
    sqlite3_finalize ( m_stmt );
    m_stmt = nullptr;
  }
}
int sqlite_statement::step (  ) {
  return sqlite3_step ( m_stmt );
}

sqlite_statement::~sqlite_statement (  ) {
  finalize (  );
}
sqlite_statement::sqlite_statement (
  std::shared_ptr<sqlite_database> sptr_db,
  const char *lpsz_query
) {
  m_sptr_db = sptr_db;
  m_str_query = lpsz_query;

  int i_ret = sqlite3_prepare_v2 (
    m_sptr_db ->get_database (  ),
    lpsz_query,
    -1, //read until first 0 (null terminator)
    &m_stmt,
    nullptr //would point to first character of unused sql (past ';' character).
  );
  if ( i_ret != SQLITE_OK ) {
    std::string str_error = std::vformat (
      "Error: \"{}\"",
      std::make_format_args ( sqlite3_errmsg ( m_sptr_db ->get_database (  ) ) )
    );
    throw std::runtime_error ( str_error );
  }
}


class scoped_resource {
  private:
    std::function<void(void)> m_fn_cleanup;
  public:
    scoped_resource (  ) = delete;
    scoped_resource ( std::function<void(void)> fn_cleanup );
    ~scoped_resource (  );
};
scoped_resource::scoped_resource ( std::function<void(void)> fn_cleanup ) {
  m_fn_cleanup = fn_cleanup;
}
scoped_resource::~scoped_resource (  ) {
  m_fn_cleanup (  );
}


int main ( void ) {
  std::shared_ptr<sqlite_database> sptr_db;
  //scoped_resource sr_database ( [&db]() -> void { sqlite3_close ( db ); } );


  std::cout << "The database was successfully opened." << std::endl << std::flush;
  bool b_success;
  int i_ret;

  std::unique_ptr<sqlite_statement> uptr_ss_create_users_table,
  uptr_ss_insert, uptr_ss_update, uptr_ss_select;

  try {
    sptr_db = std::make_shared<sqlite_database> ( "some_database.dat" );

    std::cout << "Created database instance.\n" << std::flush;

    uptr_ss_select = std::make_unique<sqlite_statement> (
      sptr_db,
      R"#(select * from `users` where `str_name` = ?;)#"
    );

    std::cout << "Prepared a select statement.\n" << std::flush;


    uptr_ss_insert = std::make_unique<sqlite_statement> (
      sptr_db,
      R"#(insert into `users` (
        `str_name`,
        `i64_age`,
        `dbl_height`
      ) values (
        ?, -- str_name
        ?, -- i64_age
        ?  -- dbl_height
      );)#"
    );
    std::cout << "Prepared an insertion statement.\n" << std::flush;


    //Create the users table, if it doesn't already exist.
    uptr_ss_create_users_table = std::make_unique<sqlite_statement> (
      sptr_db,
      R"#(create table if not exists `users` (
        `str_name` text,
        `i64_age` integer,
        `dbl_height` real,
        `id` integer primary key autoincrement
      );)#"
    );
    std::cout << "Prepared a create users table statement.\n" << std::flush;

    uptr_ss_update = std::make_unique<sqlite_statement> (
      sptr_db,
      "update `users` set `i64_age` = 102 where `str_name` = ?;"
    );
    std::cout << "Prepared an update users table statement.\n" << std::flush;
  } catch ( std::exception &e ) {
    std::cerr << "Exception: " << e .what (  ) << std::endl;
    return 0;
  }


  //Handle table creation test.
  i_ret = uptr_ss_create_users_table ->step (  );
  if ( i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Failed to create a new table.\n";
    return 0;
  }
  std::cout << "Attempted to create the users table.\n" << std::flush;



  //Handle table update test.
  uptr_ss_update ->bind_arguments ( "TheTepidCoder" );
  i_ret = uptr_ss_update ->step (  );
  if ( i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Failed to update the existing record.\n";
    return 0;
  }
  std::cout << "Updated " << sptr_db ->get_updated_record_count (  ) << " records.\n" << std::flush;



  //Handle record insertion test.
  if ( ! uptr_ss_insert ->bind_arguments (
      "NewCoder",
      111,
      77.7f
    )
  ) {
    std::cerr << "Error: Couldn't bind the insertion query's arguments.\n";
    return 0;
  }
  if ( SQLITE_DONE != uptr_ss_insert ->step (  ) ) {
    std::cerr << "Error: Couldn't insert a record.\nLast error: " << sptr_db ->get_last_error (  ) << std::endl;
    return 0;
  }
  std::cout << "Called the insert statement.\n" << std::flush;

  //Allow for binding new arguments.
  uptr_ss_insert ->reset (  );

  //Insert a second record with the same insertion statement.
  if ( ! uptr_ss_insert ->bind_arguments ( "CoderGurl912", 20, 5.7f ) ) {
    std::cerr << "Error: Couldn't bind the second insertion record columns.\n";
    return 0;
  }
  if ( SQLITE_DONE != uptr_ss_insert ->step (  ) ) {
    std::cerr << "Error: Failed to insert a second record.\n";
    return 0;
  }





  int64_t i64_new_record_identifier = sptr_db ->get_last_inserted_record_identifier (  );
  std::cout << "New record identifier: " << (int) i64_new_record_identifier << std::endl << std::flush;


  if ( ! uptr_ss_select ->bind_arguments ( "TheTepidCoder" ) ) {
    std::cerr << "Error: Failed to bind one or more arguments.\n";
    std::cerr << sqlite3_errmsg ( sptr_db ->get_database (  ) ) << std::endl;
    return 0;
  }
  std::cout << "The query input arguments were bound." << std::endl << std::flush;

  std::string str_name;
  int64_t i64_age;
  double dbl_height;

  uptr_ss_select ->foreach_row (
    [&]() -> bool {
      std::cout << "name: \"" <<
      str_name << "\"; age: \"" <<
      i64_age << "\"; height: \"" <<
      dbl_height << "\"" << std::endl;
      return true; //keep grabbing rows
    },
    str_name,
    i64_age,
    dbl_height
  );

  return 0;
}
