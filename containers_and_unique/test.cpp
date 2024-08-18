/*
 Created on 2024-08-13 by TheTepidCoder
 Purpose: To show a few different STL
 container types and for what purposes
 one might use them. I show std::vector<T>,
 std::set<T>, std::map<K, V>, and
 std::unordered_map<K, V>.
 I also show off how to use std::unique
 to shrink a vector to only its unique
 elements.
*/
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>

int main ( void ) {
  std::vector<std::string> vec_options = {
    "cat",
    "dog",
    "lemons",
    "mud",
    "mud",
    "cat",
    "dog"
  };

/*
  std::sort (
    vec_options .begin (  ),
    vec_options .end (  )
  );

  std::vector<std::string>::iterator it = std::unique (
    vec_options .begin (  ),
    vec_options .end (  )
  );
  vec_options .erase (
    it,
    vec_options .end (  )
  );

  for ( const auto &str_option : vec_options ) {
    std::cout << "Option: \"" << str_option << "\"" << std::endl;
  }
*/

/*
  std::set<std::string> set_options;
  for ( const auto &str_option : vec_options ) {
    std::cout << "Adding option: \"" << str_option << "\" to the set" << std::endl;
    set_options .insert ( str_option );
  }

  for ( const auto &str_option : set_options ) {
    std::cout << "Option: \"" << str_option << "\"" << std::endl;
  }
*/

/*
  std::unordered_map<std::string, size_t> um_options;
  for ( const auto &str_option : vec_options ) {
    if ( um_options .find ( str_option ) == um_options .end (  ) ) {
      um_options [ str_option ] = 1;
    }
    else {
      um_options [ str_option ] ++;
    }
  }

  for ( const auto &[str_option, st_instance_count] : um_options ) {
    std::cout << "Option \"" << str_option << "\" => " << st_instance_count << ".\n";
  }
*/

  std::map<std::string, size_t> m_options;
  for ( const auto &str_option : vec_options ) {
    if ( m_options .find ( str_option ) == m_options .end (  ) ) {
      m_options [ str_option ] = 1;
    }
    else {
      m_options [ str_option ] ++;
    }
  }

  for ( const auto &[str_option, st_instance_count] : m_options ) {
    std::cout << "Option \"" << str_option << "\" => " << st_instance_count << ".\n";
  }


  return 0;
}
