/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To show the basics of C++ classes.
 -Instantiation
 -Copying
 -Copy assignment
 -Moving
 -Move assignment
 -Non-default constructors
 -Unique pointers
 -Shared pointers (only brief overview)
 -Destructors
 -Scope?!?
 -new / delete
 -new[] / delete[]
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"

#include <utility>
#include <stdexcept> //"std::runtime_error" //std::exception
#include <memory> //std::shared_ptr<T> std::unique_ptr<T>

class string {
  private:
    char *m_lpsz_string = nullptr;
    size_t m_st_length = 0;

  public:
    string (  ); //default constructor
    string ( const string &old );
    string &operator= ( const string &old );
    string ( string &&old ) noexcept;
    string &operator= ( string &&old );
    ~string (  ); //destructor

    string ( const char *lpsz_string );

    [[nodiscard]] const char *get_string ( void ) const noexcept;
    bool set_string ( const char *lpsz_string );

};

string::string (  ) {
  fprintf ( stdout, "\"%s\" called.\n", __PRETTY_FUNCTION__ );
  fflush ( stdout );
}
string::string ( const char *lpsz_string ) {
  fprintf ( stdout, "\"%s\" called.\n", __PRETTY_FUNCTION__ );
  m_lpsz_string = strdup ( lpsz_string );
  if ( ! m_lpsz_string ) {
    throw std::runtime_error ( "Error: Failed to allocate a new copy of the passed string." );
  }
  m_st_length = strlen ( lpsz_string );
}

string::string ( const string &old ) {
  fprintf ( stdout, "\"%s\" (copy) called.\n", __PRETTY_FUNCTION__ );

  //m_lpsz_string = old .m_lpsz_string; //copying the address (bad -- causes double free).
  m_lpsz_string = strdup ( old .m_lpsz_string );
  m_st_length = old .m_st_length;
}
string &string::operator= ( const string &old ) {
  fprintf ( stdout, "\"%s\" (copy=) called.\n", __PRETTY_FUNCTION__ );

  m_lpsz_string = strdup ( old .m_lpsz_string );
  m_st_length = old .m_st_length;
  return *this; //string
}

string::string ( string &&old ) noexcept {
  fprintf ( stdout, "\"%s\" (move) called.\n", __PRETTY_FUNCTION__ );

  m_lpsz_string = old .m_lpsz_string; //copy the address
  m_st_length = old .m_st_length;

  old .m_lpsz_string = nullptr;
  old .m_st_length = 0;
}
string &string::operator= ( string &&old ) {
  fprintf ( stdout, "\"%s\" (move=) called.\n", __PRETTY_FUNCTION__ );

  m_lpsz_string = old .m_lpsz_string; //copy the address
  m_st_length = old .m_st_length;

  old .m_lpsz_string = nullptr;
  old .m_st_length = 0;

  return *this;
}


string::~string (  ) {
  fprintf ( stdout, "\"%s\" called.\n", __PRETTY_FUNCTION__ );
  fflush ( stdout );

  if ( m_lpsz_string ) {
    free ( m_lpsz_string );
  }
}

[[nodiscard]] const char *string::get_string ( void ) const noexcept {
  return m_lpsz_string;
}
bool string::set_string ( const char *lpsz_string ) {
  if ( m_lpsz_string ) { //nullptr, NULL, 0
    m_st_length = 0;
    free ( m_lpsz_string );
    m_lpsz_string = nullptr;
  }

  m_lpsz_string = strdup ( lpsz_string );

  if ( ! m_lpsz_string ) {
    return false;
  }

  m_st_length = strlen ( lpsz_string );

  return true;
}

int test001 ( void ) {
  string s;
  s .set_string ( "testing" );
  //s .m_lpsz_string -> "testing"

  //Copy constructor test
  //string s2 = s;

  //Copy assignment test
  //string s2;
  //s2 = s;

  //Move constructor test
  //string s2 = std::move ( s );

  //Move assignment test
  string s2;
  s2 = std::move ( s );

  //Print from the original string object.
  fprintf ( stdout, "Here's the string: \"%s\"\n", s .get_string (  ) );
  //Print from the object into which we've moved the original string object's contents.
  fprintf ( stdout, "Here's the string: \"%s\"\n", s2 .get_string (  ) );

  fprintf ( stdout, "Finished.\n" );
  fflush ( stdout );

  return 0;
}

int main ( void ) {
  std::unique_ptr<string> uptr_s;
  std::unique_ptr<string> uptr_s2;
  //std::unique_ptr<string> uptr_s2 = uptr_s; //invalid!
  uptr_s .reset();
  try {
    //uptr_s = std::unique_ptr<string>(new string ( "testing123" ));
    uptr_s = std::make_unique<string> ( "testing123" );
//    uptr_s2 = std::move ( uptr_s ); //valid

  } catch ( std::exception &e ) {
    fprintf ( stderr, "Exception: \"%s\"\n", e .what (  ) );
    return 1;
  }


  string *lp_string = uptr_s .get (  );
  if ( ! lp_string ) {
    fprintf ( stdout, "The string returned was invalid.\n" );
    return 0;
  }
  //std::unique_ptr<string> (string*)
  //std::unique_ptr<T>::get() --> string *
  fprintf ( stdout, "String \"%s\"\n", lp_string ->get_string (  ) );


  return 0;
}
