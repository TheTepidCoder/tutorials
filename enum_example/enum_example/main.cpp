/*
 Created by TheTepidCoder on 2024-08-10
 Purpose: To show off enums in C and C++.

 I also reveal my preprocessor method for safely synchronizing
 enums values and their names, regardless of new values having
 been inserted or old ones being reorded.
*/
#include <iostream>
#include <cstring>
#include <cstdint> //u?int#_t

//#include "super_enum.h"
#define DO_STUFF_IMPL
#include "enums.h"

void foo ( void ); //from extra.cpp

/*
#define super_enum(namespace_name, enum_name, enum_type, ...) \
namespace namespace_name { \
enum enum_name : enum_type { \
  __VA_ARGS__, \
  __SENTINEL__ \
}; \
char lpsz_enum_members [] = #__VA_ARGS__; \
std::vector<const char *> vec_enum_member_names; \
void parse_enum_member_names ( void ) { \
  char *p = lpsz_enum_members; \
  char *e; \
  while ( 1 ) { \
    e = p; \
    while ( *e && ! strchr ( "= ,", *e ) ) e ++; \
    if ( ! *e ) break; \
    *e = 0; \
    vec_enum_member_names .emplace_back ( p ); \
    e ++; \
    p = e; \
    while ( *e && *e != ',' ) e ++; \
    if ( ! *e ) break; \
    p = e + 1; \
    while ( *p == ' ' ) p ++; \
  } \
  if ( *p == '=' ) return; \
  while ( *p == ' ' ) p ++; \
  e = strchr ( p, ' ' ); \
  *e = 0; \
  vec_enum_member_names .emplace_back ( p ); \
} \
}

super_enum (
  ns_stuff,
  stuff,
  uint8_t,
  a = 3,
  c = 0b0000'0101,
  b = 0xd34dc0d3
);
*/



const char *g_lpszary_numbers[] = {
  "zero",
  "one",
  "two",
  "three",
  "four"
};

namespace ns_numbers {
enum numbers : uint8_t {
  zero,
  one,
  two,
  three,
  four //...
};
}

namespace ns_ages {
enum ages {
  zero,
  one,
  two,
  three //...
};
}

int main () {
  int x = ns_numbers::numbers::zero;
  std::cout << g_lpszary_numbers [ x ] << " = " << x << std::endl;
  int y = ns_ages::ages::three;
  std::cout << y << std::endl;

  //No longer required with my new class,
  //ns_stuff::__dummy_instantiator being
  //generated and instantiated.
  //ns_stuff::parse_enum_member_names (  );
{
  for (
    size_t st_enum_member_index = 0;
    st_enum_member_index < ns_stuff::__SENTINEL__;
    st_enum_member_index ++
  ) {
    std::cout << "Member name: \"" << ns_stuff::lpszary_enum_member_names [ st_enum_member_index ] << "\" => " << st_enum_member_index << std::endl;
  }
}

  foo (  );

  return 0;
}