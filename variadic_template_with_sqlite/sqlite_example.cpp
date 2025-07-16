/*
 Created on 2024-08-19 by TheTepidCoder
 Purpose: To allow for type-safe SQLite3 database
 inserts which use dynamically chosen bindings
 generated at compile-time in prepared statements.
 Any extra arguments would cause an SQLite3 run-time
 complaint -- as would having too few of them.
 BLOB column types are handled by std::vector<uint8_t>.

 //To build:
 // g++ sqlite_example.cpp -l:libsqlite3.a -o sqlite_example
 //Debug mode:
 // g++ -Wall -g -static -std=c++20 sqlite_example.cpp -l:libsqlite3.a -o sqlite_example

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
#include <cinttypes> //PRI[ui][1368][246]
#include <string>
#include <vector>

#include <cstdio>

#ifndef DEBUG_MODE
#  define DEBUG_MODE 0
#endif

#if DEBUG_MODE
  //For demangling a variable type during debugging.
#  include <cxxabi.h>
#endif


#ifndef dump_case
#  define dump_case(x) case x: return #x;
#endif


#ifndef debug_printf
#  if DEBUG_MODE
#    define debug_printf(fh,...) \
       fprintf ( \
         fh, \
         "[%s:%" PRIu64 "] \"%s\" --> " \
         __FILE__, \
         (uint64_t) __LINE__,
         __FUNCTION__ \
       ); \
       fprintf ( \
         fh, \
         __VA_ARGS__ \
       )
#  else
#    define debug_printf(fh,...)
#  endif
#endif

namespace ns_sqlite_types {
enum sqlite_types : uint8_t {
  unknown = 0, //unspecified
  blob = 1, //std::vector<uint8_t>
  integer = 2, //int64_t
  real = 3, //double
  text = 4 //std::string
};
}

namespace ns_sqlite_return_codes {
enum sqlite_return_codes : uint8_t {
  error = 0,
  no_records = 1,
  records = 2
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
    //default constructor isn't allow, since we demand a type to reference
    sqlite_type (  ) = delete;
    //Copy and move constructors
    sqlite_type ( const sqlite_type & );
    sqlite_type ( sqlite_type && );

    sqlite_type ( int64_t &i64 );
    sqlite_type ( double &dbl );
    sqlite_type ( std::vector<uint8_t> &vec );
    sqlite_type ( std::string &str );
    ~sqlite_type (  );

    //Copy/Move assignment operator overloads
    sqlite_type &operator= ( const sqlite_type & );
    sqlite_type &operator= ( sqlite_type && ) noexcept;

    sqlite_type &operator= ( const unsigned char * );
    void assign_blob ( const void *, size_t );
    sqlite_type &operator= ( const int64_t & );
    sqlite_type &operator= ( const double & );

    ns_sqlite_types::sqlite_types type ( void ) const noexcept;

    static const char *get_sqlite_type_name (
      ns_sqlite_types::sqlite_types ste_type
    ) noexcept;
    void handle_type_mismatch (
      const char *lpsz_function_name,
      ns_sqlite_types::sqlite_types ste_t
    ) noexcept;
};

sqlite_type::sqlite_type ( const sqlite_type &old ) {
  //Copy the variable reference and meta type enum value.
  m_ste_sqlite_type = old .m_ste_sqlite_type;
  memcpy (
    &m_u_type,
    &old .m_u_type,
    sizeof ( m_u_type )
  );
}
sqlite_type::sqlite_type ( sqlite_type &&old ) {
  //Copy the variable reference.
  m_ste_sqlite_type = old .m_ste_sqlite_type;

  //Prevent the old instance that we're moving from freeing anything.
  old .m_ste_sqlite_type = ns_sqlite_types::sqlite_types::unknown;

  //Copy meta type enum value.
  memcpy (
    &m_u_type,
    &old .m_u_type,
    sizeof ( m_u_type )
  );
}
sqlite_type &sqlite_type::operator= ( const sqlite_type &old ) {
  //Copy the type meta information and variable reference.
  m_ste_sqlite_type = old .m_ste_sqlite_type;
  memcpy (
    &m_u_type,
    &old .m_u_type,
    sizeof ( m_u_type )
  );

  //Return a reference to this object for chaining. E.g. inst = inst2 = inst3;
  return *this;
}
sqlite_type &sqlite_type::operator= ( sqlite_type &&old ) noexcept {
  m_ste_sqlite_type = old .m_ste_sqlite_type;
  old .m_ste_sqlite_type = ns_sqlite_types::sqlite_types::unknown;
  memcpy (
    &m_u_type,
    &old .m_u_type,
    sizeof ( m_u_type )
  );
/*
  //Clear the variable reference as well. This is slower and provides no tangible benefit.
  memset (
    &old .m_u_type,
    0,
    sizeof ( m_u_type )
  );
*/
  return *this;
}

sqlite_type::sqlite_type (
  int64_t &i64
) :
  m_u_type { .lpi64 = &i64 },
  m_ste_sqlite_type ( ns_sqlite_types::integer )
{
  fprintf ( stdout, "i64: %p\n", &i64 );
}

sqlite_type::sqlite_type (
  double &dbl
) :
  m_u_type { .lpdbl = &dbl },
  m_ste_sqlite_type ( ns_sqlite_types::real )
{
  fprintf ( stdout, "dbl: %p\n", &dbl );
}

sqlite_type::sqlite_type (
  std::vector<uint8_t> &vec
) :
  m_u_type { .lpvec = &vec },
  m_ste_sqlite_type ( ns_sqlite_types::blob )
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
/*
  //I don't know what I was thinking. We hold a non-owning
  //reference to the underlying variable type and do so
  //only so as to allow reading from / writing to it when
  //doing select/update/insert queries.
  if ( m_ste_sqlite_type == ns_sqlite_types::blob ) {
    delete m_u_type .lpvec;
  }
  if ( m_ste_sqlite_type == ns_sqlite_types::text ) {
    delete m_u_type .lpstr;
  }
*/
}

sqlite_type &sqlite_type::operator= ( const unsigned char *lpsz_string ) {
  //!! TODO: Handle implicit type conversion from const unsigned char * to
  //all supported types. atoi, atod for integer/real. blob/unknown wouldn't
  //have any meaningful conversions and shouldn't be usable.
  switch ( m_ste_sqlite_type ) {
    case ns_sqlite_types::text: break;
    case ns_sqlite_types::blob:
    case ns_sqlite_types::real:
    case ns_sqlite_types::integer:
    case ns_sqlite_types::unknown: {
      handle_type_mismatch ( __FUNCTION__, ns_sqlite_types::integer );
      return *this; //won't ever occur bceause handle_type_mismatch calls exit(1).
    }
  }

  //fprintf ( stdout, "Note: We're assigning an sqlite_type object to (const char *) \"%s\"\n", lpsz_string );

  m_u_type .lpstr ->assign (
    (const char *) lpsz_string
  );
  return *this;
}
void sqlite_type::assign_blob ( const void *lpv_data, size_t st_size ) {
  switch ( m_ste_sqlite_type ) {
    case ns_sqlite_types::blob: break;

    case ns_sqlite_types::integer:
    case ns_sqlite_types::real:
    case ns_sqlite_types::text:
    case ns_sqlite_types::unknown: {
      handle_type_mismatch ( __FUNCTION__, ns_sqlite_types::integer );
      return ;
    }
  }

  m_u_type .lpvec ->assign (
    (const uint8_t*) lpv_data,
    (const uint8_t*) lpv_data + st_size
  );

  debug_printf (
    stdout,
    "Blob range: [%p, %p]\n",
    m_u_type .lpvec ->data (  ),
    m_u_type .lpvec ->data (  ) + st_size - 1
  );
}

sqlite_type &sqlite_type::operator= ( const int64_t &i64 ) {
  switch ( m_ste_sqlite_type ) {
    case ns_sqlite_types::integer: break;

    case ns_sqlite_types::blob:
    case ns_sqlite_types::real: //!! TODO: Consider allowing for implicit type conversion between double<->int
    case ns_sqlite_types::text:
    case ns_sqlite_types::unknown: {
      handle_type_mismatch ( __FUNCTION__, ns_sqlite_types::integer );
      return *this; //won't ever occur bceause handle_type_mismatch calls exit(1).
    }
  }

  *m_u_type .lpi64 = i64;
  return *this;
}
sqlite_type &sqlite_type::operator= ( const double &dbl ) {
  switch ( m_ste_sqlite_type ) {
    case ns_sqlite_types::real: break;

    case ns_sqlite_types::blob:
    case ns_sqlite_types::integer: //!! TODO: Consider allowing for implicit type conversion between double<->int
    case ns_sqlite_types::text:
    case ns_sqlite_types::unknown: {
      handle_type_mismatch ( __FUNCTION__, ns_sqlite_types::integer );
      return *this; //won't ever occur bceause handle_type_mismatch calls exit(1).
    }
  }

  *m_u_type .lpdbl = dbl;
  return *this;
}
ns_sqlite_types::sqlite_types sqlite_type::type ( void ) const noexcept {
  return m_ste_sqlite_type;
}

const char *sqlite_type::get_sqlite_type_name (
  ns_sqlite_types::sqlite_types ste_type
) noexcept {
  switch ( ste_type ) {
    dump_case ( ns_sqlite_types::unknown )
    dump_case ( ns_sqlite_types::integer )
    dump_case ( ns_sqlite_types::blob )
    dump_case ( ns_sqlite_types::real )
    dump_case ( ns_sqlite_types::text )

    default: break;
  }
  fprintf (
    stderr,
    "An unknown type was passed to %s.\n",
    __FUNCTION__
  );
  fflush ( stderr );
  exit ( 1 );
  return 0;
}

//This will dump an error message to stderr and terminate the application.
void sqlite_type::handle_type_mismatch (
  const char *lpsz_function_name,
  ns_sqlite_types::sqlite_types ste_t
) noexcept {

  fprintf (
    stderr,
    "[%s] Error: \"%s\" passed to instance handling \"%s\"\n",
    lpsz_function_name,
    sqlite_type::get_sqlite_type_name ( m_ste_sqlite_type ),
    sqlite_type::get_sqlite_type_name ( ste_t )
  );
  fflush ( stderr );
  exit ( 1 );

}
///////


template<typename T, typename ...Targs>
bool bind_query_arguments (
  sqlite3_stmt *stmt,
  int i_one_based_column_index,
  const T &t_value,
  const Targs &...fargs
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
        t_value .c_str (  ),
        -1,
        SQLITE_STATIC
      )
    ) {
      return false;
    }
  }
  else if constexpr (
    std::is_same<
      std::decay_t<T>, //const char [#] -> const char *
      char * //const char* (apparently typename std::decay<T>::type e.g. std::decay_t<T> removes const qualifications.
    >::value //||
//    std::is_same<
//      typename std::remove_extent<T>::type, //T[#] -->T[]
//      typename std::remove_extent<const char(&)[]>::type //std::remove_extent_t<const char[]>
//    >::value
  ) {
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
  else if constexpr (
    std::is_same<
      std::decay_t<T>, //T,
      std::vector<uint8_t> //const std::vector<uint8_t>&
    >::value
  ) {
    std::cout << "Binding a BLOB type (via std::vector<uint8_t>).\n";
    if (
      SQLITE_OK != sqlite3_bind_blob (
        stmt,
        i_one_based_column_index,
        t_value .data (  ), //Reference to the first byte of the BLOB's buffer.
        t_value .size (  ), //Size in bytes of the BLOB's buffer.
        SQLITE_STATIC //It's our responsibility to free the BLOB's buffer.
      )
    ) {
      return false;
    }
  }
  else {
#if DEBUG_MODE
    size_t st_length;
    int i_unknown;
    const char *lpsz_demangled_type_name = abi::__cxa_demangle (
      typeid (
        std::decay_t<decltype(t_value)>
      ) .name (  ),
      0,
      &st_length,
      &i_unknown
    );

    fprintf (
      stderr,
      "Error: A variadic type (%s) that couldn't be resolved was passed to bind_query_arguments.\n",
      lpsz_demangled_type_name
    );
#else
    //If we're not in debug mode, just show the mangled type name.
    fprintf (
      stderr,
      "Error: A variadic type (%s) that couldn't be resolved was passed to bind_query_arguments.\n",
      typeid ( t_value ) .name (  )
    );
#endif
    exit ( 1 );
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
  const Targs &...fargs
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
  const Targs &...fargs
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
[[nodiscard]] ns_sqlite_return_codes::sqlite_return_codes select (
  sqlite3 *db,
  const char *lpsz_query,
  std::vector<sqlite_type> &vec_output_args,
  const Targs &...fargs
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
    return ns_sqlite_return_codes::error;
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
      return ns_sqlite_return_codes::error;
    }
  }

  i_ret = sqlite3_step ( stmt );
  if ( i_ret != SQLITE_ROW && i_ret != SQLITE_DONE ) {
    std::cerr << "Error: Stepping through SQLite3 statment failed." << std::endl;
    return ns_sqlite_return_codes::error;
  }

  //!! TODO: Change the return type to signal success with data; success without data; failure.
  if ( i_ret == SQLITE_DONE ) {
    return ns_sqlite_return_codes::no_records;
  }

//!!!!!
  size_t st_column_index = 0;
  for ( auto &output_arg : vec_output_args ) {
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
        return ns_sqlite_return_codes::sqlite_return_codes::error;
      } //default case
    } //switch for each type of argument

    st_column_index ++;
  } //for each output argument
//!!

  //sqlite3_reset ( stmt );
  sqlite3_finalize ( stmt );

std::cout << "Done with select function.\n" << std::flush;

  return ns_sqlite_return_codes::records;
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
`blob_data` blob,
`id` integer primary key autoincrement
);)#"
  );
  if ( ! b_success ) {
    std::cerr << "Error: Failed to create the table, \"users.\"\n";
    sqlite3_close ( db );
    return 0;
  }

  std::cout << "Table created." << std::endl;

  std::vector<uint8_t> v_blob = { 0, 1, 2, 3 };

  int64_t i64_new_record_identifier = insert (
    db,
    R"#(insert into `users` (
  `str_name`,
  `i64_age`,
  `dbl_height`,
  `blob_data`
) values (
  ?, -- str_name
  ?, -- i64_age
  ?, -- dbl_height
  ?  -- blob_data
);)#",
    //"BetterCoder",
    "TheTepidCoder", //Ensure that a user with the name "TheTepidCoder" exists for the select test, later on.
    104,
    4.2f,
    v_blob
  );
  if ( ! b_success ) {
    std::cerr << "Error: Failed to insert a new row into the users table.\n";
    sqlite3_close ( db );
    return 0;
  }
  std::cout << "New record identifier: " << (int) i64_new_record_identifier << std::endl;

  v_blob .clear (  );
  fprintf (
    stdout,
    "[%" PRIu64 "] The blob vector was cleared to ensure that we're not viewing stale data.\n",
    v_blob .size (  )
  );

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
  vec_types .emplace_back ( v_blob );

  ns_sqlite_return_codes::sqlite_return_codes rc = select (
    db,
    R"#(select * from `users` where `str_name` = ?;)#",
    vec_types,
    "TheTepidCoder"
  );
  if ( rc == ns_sqlite_return_codes::sqlite_return_codes::error ) {
    fprintf ( stderr, "Error: The selection failed with an SQLITE3 error.\n" );
    goto cleanup;
  }
  else if ( rc == ns_sqlite_return_codes::sqlite_return_codes::no_records ) {
    fprintf (
      stdout,
      "Note: While the query completed successfully, there were no records matching the specified filters.\n"
    );
    goto cleanup;
  }
  else {
    fprintf (
      stdout,
      "Note: The query completed successfully and returned some records.\n"
    );
  }
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
  for ( const auto &ui8 : v_blob ) {
    std::cout << "value: " << (int) ui8 << std::endl;
  }


//We can jump here early to free the database connection,
//if an error occurs.
cleanup:

  sqlite3_close ( db );

  std::cout << "Terminating the application." << std::endl;

  return 0;
}
