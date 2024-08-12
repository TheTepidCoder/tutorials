/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to use T& in various
 contexts. This was also a segue into c++11+
 initializer lists.
*/
#include "stdlib.h"
#include "stdint.h"

#include <iostream> //std::cout // std::cin
#include <string> //std::string char // std::wstring wchar_t
#include <format> //std::vformat

#include <vector> //std::vector<T>
#include <map> //std::map<K, V>

void test ( void ) {
  std::vector<std::vector<char>> vv_characters;

  //vv_characters [ 0 ] -> [
  //  vec0 -> [ 0, 1, 2, 3 ],
  //  vec1 -> [ 0, 1, 2, 3 ]
  //]

  const char *lpsz_alphabet = "abcdefghijklmnopqrstuvwxyz";
  for ( size_t st_i = 0; st_i < 10; st_i ++ ) {
    std::vector<char> v_characters;
    const char *s = lpsz_alphabet + st_i;
    const char *e = s + 10;
    v_characters .insert (
      v_characters .end (  ), //[ s, e )
      s,
      e
    );
    v_characters .emplace_back ( 0 );

    vv_characters .emplace_back ( std::move ( v_characters ) );
  }

  for ( const auto &v : vv_characters ) {
    //const std::vector<char>&v
    const char *p = v.data (  );
    std::cout << "\"" << p << "\"" << std::endl;
  }

  //b
  std::vector<char> &v_character_vector_that_starts_with_a =
    vv_characters [ 0 ];
  std::vector<char> &v_character_vector_that_starts_with_d =
    vv_characters [ 3 ]; //abc[d]

  std::cout <<
    "Vector starting with a: '" <<
    v_character_vector_that_starts_with_a [ 1 ] <<
    "'" <<
    std::endl;
  std::cout <<
    "Vector starting with d: '" <<
    v_character_vector_that_starts_with_d [ 0 ] <<
    "'" <<
    std::endl;

/*
  char c = 'c';
  char &t_x = c;
  t_x ++; //c -=> d

  std::cout <<
    std::vformat (
      "C: '{}'",
      std::make_format_args ( c )
    ) <<
    std::endl;
*/
}

void test2 ( void ) {
/*
  keys:   [ 'a', 'b', 'c' ]
             V    V    V
  values: [ 'c', 'z', '7' ]
*/
  std::map<char, char> m;
  m [ 'a' ] = 'c';
  m [ 'b' ] = 'z';
  m [ 'c' ] = '7';
  char c = m [ 'a' ];
  std::cout << "The character is '" << c << "'" << std::endl;
}

void test3( void ) {
  std::map<std::string, std::vector<char>> m_names_to_vec_char;
//std::vector<char>& <-- m_names_to_vec_char [ "TheTepidCoder" ]
/*
  m_names_to_vec_char [ "TheTepidCoder" ] .emplace_back ( 'J' );
  std::cout <<
    m_names_to_vec_char [ "TheTepidCoder" ] [ 0 ] <<
    std::endl;
*/

  std::vector<char> &v = m_names_to_vec_char [ "TheTepidCoder" ];
  v .emplace_back ( 'J' );
  std::cout << v [ 0 ] << std::endl;
}


class something {
  private:
    char &m_ref_c;

  public:
    something (  ) = delete;
    something ( char &c );
    ~something (  );
    char get ( void ) const noexcept;
    void set ( char c ) noexcept;
};
something::something (
  char &c
) :
  m_ref_c ( c )
{
}
something::~something (  ) {
}

char something::get ( void ) const noexcept {
  return m_ref_c;
}
void something::set ( char c ) noexcept {
  m_ref_c = c;
}


int main ( void ) {
  char c = 'c';
  something s ( c );
  std::cout <<
    "The character referenced by the class is '" <<
    s .get (  ) <<
    "'" <<
    std::endl;

  s .set ( 'x' );
  std::cout <<
    "The character referenced by the class is '" <<
    c <<
    "'" <<
    std::endl;

  return 0;
}
