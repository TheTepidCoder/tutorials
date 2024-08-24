#pragma once
#ifndef MONTH_HASHER_HEADER
#define MONTH_HASHER_HEADER
#include <cstdint> //^u?int[8136][624]_t$

#ifndef element_count
#define element_count(n) (sizeof(n)/sizeof(n[0]))
#endif

class month_hasher {
  private:
    uint8_t m_ui8ary_month_character_value_map [ 256 ] = { 0 };
    uint8_t m_ui8ary_month_hash_to_month_index [ 10'345 ] = { 0 };

    void _generate_month_character_value_map ( void );
    void _generate_month_hashes ( void );

  public:
    month_hasher (  );
    ~month_hasher (  );

   uint16_t generate_month_hash (
     const char *lpsz_month
   ) const noexcept;
   uint8_t operator[] ( uint16_t ui64_abbreviated_month_hash ) const noexcept; //pass hash, get 0-based month index
};

#endif
