/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to add the
 __declspec directive to functions that
 you'd like to import and export on
 MSVC.
*/
#pragma once
#ifndef ADD_HEADER
#define ADD_HEADER

#ifdef BUILDING_ADD_LIBRARY
#  define IMPORT_OR_EXPORT __declspec(dllexport)
#else
#  define IMPORT_OR_EXPORT __declspec(dllimport)
#endif

extern "C" {
  IMPORT_OR_EXPORT int add ( int, int );
}

#endif
