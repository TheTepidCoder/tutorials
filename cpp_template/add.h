/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to break C++ template functions
 into multiple cpp files to decrease compilation time.
*/
#pragma once
#ifndef ADD_HEADER
#define ADD_HEADER

template<typename T>
T add ( T a, T b );

template<typename T>
T sub ( T a, T b ) {
  return a - b;
}

#endif
