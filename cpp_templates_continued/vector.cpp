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

#include "some_class.h"

template class vector<some_class>;
template class vector<uint8_t>;

template<typename T>
vector<T>::vector (  ) {
}

template<typename T>
vector<T>::vector ( typename std::initializer_list<T> initializers ) {
  size_t st_elements_to_allocate = std::bit_ceil (
    initializers .size (  )
  );
  m_lpt_data = (T *) calloc ( sizeof ( T ), st_elements_to_allocate );
  if ( ! m_lpt_data ) {
    throw std::runtime_error ( "Unable to allocate enough memory to hold the elements specified in the initializer list." );
  }

  m_st_capacity = st_elements_to_allocate;

  for ( const auto &value : initializers ) {
    push_back ( value );
  }
}

template<typename T>
vector<T>::~vector (  ) {
  if ( m_lpt_data ) {
    for ( size_t st_i = 0; st_i < m_st_count; st_i ++ ) {
      m_lpt_data [ st_i ] .~T (  );
    }

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

  new (&m_lpt_data [ m_st_count ]) T ( t );
  //m_lpt_data [ m_st_count ] = t;
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

template<typename T>
bool vector<T>::push_back ( T &&t ) {
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

  new (&m_lpt_data [ m_st_count ]) T ( std::move ( t ) );
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
