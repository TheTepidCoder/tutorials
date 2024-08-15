/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to define C++ template classes,
 rather than just templated functions.
*/
#pragma once
#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"

#include <stdexcept>
#include <initializer_list>
#include <bit> //std::bit_ceil --> ceil(log(n)/log(2))
#include <new>

#ifndef DEBUG_PRINTF_MACRO
#define DEBUG_PRINTF_MACRO
#define debug_printf(...) \
  fprintf ( \
    stdout, \
    "[%s:%" PRIu64 "] ", \
    __FILE__, \
    static_cast<uint64_t> ( __LINE__ ) \
  ); \
  fprintf ( \
    stdout, \
    __VA_ARGS__ \
  ); \
  fflush ( stdout )
#endif

template<typename T>
class vector {
  private:
    T *m_lpt_data = nullptr;
    size_t m_st_count = 0; //number of elements currently stored
    size_t m_st_capacity = 0; //number of elements that could be stored

  public:
    vector (  );
    vector ( typename std::initializer_list<T> initializers );
    ~vector (  );
    bool push_back ( const T &t );
    bool push_back ( T &&t );

    const T *begin ( void ) const; //for ( const auto &v : vector_instance ) { ... }
    const T *end ( void ) const;
    T *begin ( void );
    T *end ( void );

    const T *data ( void ) const;
    T *data ( void );
    size_t count ( void ) const;
};

#endif
