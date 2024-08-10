/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to define C++ template classes,
 rather than just templated functions.
*/
#include "vector.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"

template class vector<uint8_t>;

template<typename T>
vector<T>::vector (  ) {
}

template<typename T>
vector<T>::~vector (  ) {
  if ( m_lpt_data ) {
    free ( m_lpt_data );
  }
}

template<typename T>
size_t vector<T>::count ( void ) const {
  return m_st_count;
}

template<typename T>
const T *vector<T>::data ( void ) const {
  return m_lpt_data;
}
template<typename T>
T *vector<T>::data ( void ) {
  return m_lpt_data;
}

/*
template<typename T>
bool vector<T>::push_back ( const T t ) {
  if ( m_st_capacity - m_st_count ) {
    m_lpt_data [ m_st_count ] = t;
    m_st_count ++;
  }
  else if ( ! m_lpt_data ) {
    m_lpt_data = calloc ( sizeof ( T ), 1 );
    if ( ! m_lpt_data ) {
      return false;
    }
  }
  else {
    T *lpt_new = realloc (
      m_lpt_data,
      m_st_capacity << 1
    );
    if ( ! lpt_new ) {
      return false;
    }

    m_lpt_data = lpt_new;
    m_st_capacity <<= 1;
  }
  return true;
}
*/


template<typename T>
bool vector<T>::push_back ( const T &t ) {

  if ( m_st_capacity - m_st_count == 0 ) {
    if ( ! m_lpt_data ) {
      m_lpt_data = (T *) calloc ( sizeof ( T ), 1 );
      if ( ! m_lpt_data ) {
        return false;
      }
      m_st_capacity = 1;
    }
    else {
      T *lpt_new = (T *) realloc (
        m_lpt_data,
        sizeof ( T ) * (m_st_capacity << 1)
      );
      if ( ! lpt_new ) {
        return false;
      }

      m_st_capacity <<= 1;
      m_lpt_data = lpt_new;
    }
  }


  m_lpt_data [ m_st_count ] = t;
  m_st_count ++;

  debug_printf (
    "Count: %" PRIu64 "\n"
    "Capacity: %" PRIu64 "\n"
    "Address: %p\n",
    static_cast<uint64_t> ( m_st_count ),
    static_cast<uint64_t> ( m_st_capacity ),
    m_lpt_data
  );

  return true;
}

//template<typename T>
//void move_back ( T &&t ); //std::move ( t )

template<typename T>
const T *vector<T>::begin ( void ) const {
  return m_lpt_data;
}

template<typename T>
const T *vector<T>::end ( void ) const {
  return m_lpt_data + m_st_count;
}

template<typename T>
T *vector<T>::begin ( void ) {
  return m_lpt_data;
}

template<typename T>
T *vector<T>::end ( void ) {
  return m_lpt_data + m_st_count;
}
