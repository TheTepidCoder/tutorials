/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To allow for an enumeration which
 is scoped within a namespace that you specify,
 has the type you specify to bound the values
 within a range that is known at compile time,
 and has the ability to iterate through the
 list of values/names.

 Limitations:
 -Enums must start from 0.
 -Requires calling namespace_name::parse_enum_member_names()
 before using the member names stored in
 namespace_name::lpszary_enum_member_names.
 -Enums may not break from the linear y=x sequencing.
 -Requires #include <cstring>
 -Requires super_enum_decl in all source file consumers
 but one.
 -Requires super_enum_impl in one source file consumer
 in order to build the object.
 Making an enums.cpp which holds the implementation is
 okay but requires the super_enum_decl macro to be repeated.
 I suggest making a common enum header which takes a
 define flag for the declaration. This flag would be unset
 when it is detected and the super_enum_impl would be called
 and the define would be undefined with #undef so that
 subsequent calls to this shared enum header would call
 the super_enum_decl, instead.

Note: This could potentially be improved by defining and
instantiating a class in the generated name space which
has a default constructor that calls the parsing function.
This would force the parsing function to be called at
run-time.

 See example usage of this project @ github.com/TheTepidCoder/tutorials


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
#pragma once
#ifndef SUPER_ENUM_HEADER
#define SUPER_ENUM_HEADER

#include <iostream>
#include <cstring>

#define super_enum_decl(namespace_name, enum_name, enum_type, ...) \
namespace namespace_name { \
enum enum_name : enum_type { \
  __VA_ARGS__, \
  __SENTINEL__ \
}; \
extern const char *lpszary_enum_member_names [ __SENTINEL__ ]; \
}

#define super_enum_impl(namespace_name, enum_name, enum_type, ...) \
namespace namespace_name { \
enum enum_name : enum_type { \
  __VA_ARGS__, \
  __SENTINEL__ \
}; \
/*Define a mutable string which holds the comma delimited member names passed to super_enum_impl so that we can parse them out and enumerate them at run-time.*/ \
char lpsz_enum_members [] = #__VA_ARGS__; \
const char *lpszary_enum_member_names [ __SENTINEL__ ] = { 0 }; \
/*Define a function which allows the indices of lpszary_enum_member_names to be used.*/ \
void parse_enum_member_names ( void ) { \
  char *p = lpsz_enum_members; \
  char *e; \
  size_t st_index = 0; \
  while ( e = strchr ( p, ',' ) ) { \
    *e = 0; \
    lpszary_enum_member_names [ st_index ++ ] = p; \
    p = e + 1; \
    while ( *p == ' ' ) p ++; \
  } \
  lpszary_enum_member_names [ st_index ] = p; \
} \
/*Define a class which does nothing but call namespace_name::parse_enum_member_names in its constructor.*/ \
class __dummy_instantiator { \
  public: \
    __dummy_instantiator (  ) { \
      parse_enum_member_names (  ); \
    } \
    ~__dummy_instantiator (  ) {  } \
}; \
/* This is marked as volatile to prevent the class object from being optimized away.*/ \
volatile __dummy_instantiator __dummy; \
}

#endif