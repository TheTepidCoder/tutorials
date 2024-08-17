/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To show how to share an enum generated
 by my super_enum_[impl/decl] macro pair across
 a large project.

 In one and only one cpp file of the project,
 define DO_STUFF_IMPL before including this
 header file. In all other cpp files, simply
 include this header file as per usual.
 This will ensure that the necessary functions
 in the namespace specified by the call to the
 super_enum_impl macro exist in only a single
 object file. The rest of the cpp files which
 include this header will see only the enum
 name, its members, and a forward declaration
 of the array of constant character pointers
 so as to allow for retrieval of the names
 of the members.
 See:
 namespace_name::lpszary_enum_member_names [ 0 -> __SENTINEL__ - 1 ]
*/
#pragma once
#ifndef ENUMS_HEADER
#define ENUMS_HEADER

#include "super_enum.h"
#include <cstdint>

#ifdef IMPLEMENT_PROJECT_ENUMS
#  undef IMPLEMENT_PROJECT_ENUMS
#  define ENUM_MACRO_TYPE super_enum_impl
#else
#  define ENUM_MACRO_TYPE super_enum_decl
#endif

ENUM_MACRO_TYPE (
  ns_stuff,
  stuff,
  uint8_t,
  a,
  c,
  b
);

ENUM_MACRO_TYPE (
  ns_debugging_output,
  debugging_output,
  uint8_t,
  file_handling,
  encryption,
  ssl_validation,
  client_connections
);

#endif