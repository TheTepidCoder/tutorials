/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To show how my super_enum_[decl]
 macro can be used to share only the enum and
 member name array with cpp files other than
 the one which contains the implementation
 generated by super_enum_impl.
*/
#include <iostream>
//In this source file, we don't define DO_STUFF_IMPL so we
//get only the enum and a forward declaration of the array
//of constant character pointers to member name C-strings.
#include "enums.h"

void foo ( void ) {
  int x = ns_stuff::stuff::a;
  std::cout << "Greetings from extra.cpp's foo function." << std::endl;

  for (
    size_t st_enum_member_index = 0;
    st_enum_member_index < ns_stuff::__SENTINEL__;
    st_enum_member_index ++
  ) {
    std::cout << "Member name: \"" << ns_stuff::lpszary_enum_member_names [ st_enum_member_index ] << "\" => " << st_enum_member_index << std::endl;
  }
}