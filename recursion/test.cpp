/*
 Created on 2024-08-03 by TheTepidCoder
 Purpose: To explain recursion.
 As an aside, I used this as a chance to
 show the flood-fill algorithm and some
 bitwise operations (shifting, and and or).
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"

#include <vector>

class bits {
  public:
    bits ( size_t st_width, size_t st_height );
    bits (  ) = delete;
    ~bits (  );
    void set ( size_t st_x, size_t st_y ); //set to 0 or 1 @ coordinates
    void clear ( size_t st_x, size_t st_y ); //set to 0 or 1 @ coordinates
    uint8_t get ( size_t st_x, size_t st_y ) const; //get a 0 or 1 from coordinates
    void show_picture ( void ) const noexcept;
    void flood_fill (
      size_t st_x,
      size_t st_y,
      uint8_t ui8_value_to_fill_with
    );
    void debug_dump ( void ) const;

  private:
    size_t m_st_width = 0;
    size_t m_st_height = 0;
    std::vector<uint8_t> m_vec;
};

bits::bits ( size_t st_width, size_t st_height ) {
  //floor(1234 / 10) >> 1 123 > 12 > 1 > 0
  //0101 0101 >> 1 --> 0010 1010
  //floor ( ... / 2 )
  //a / (2*2*2) a/8
  size_t st_size = (st_width*st_height) >> 3;
  st_size += (st_width * st_height) % 8 != 0 ? 1 : 0;
  //13*11 => (10+1)*(10+3) = 100 + 30 + 10 + 3 = 143
  //16 / 8 = 2R0
  m_vec .resize ( st_size, 0 );

  m_st_width = st_width;
  m_st_height = st_height;
}
bits::~bits (  ) {
}

void bits::set ( size_t st_x, size_t st_y ) {
  size_t st_offset = (st_x + st_y * m_st_width);
  uint8_t ui8_bit = st_offset % 8; //take the remainder as the bit index
  st_offset >>= 3; //divide by 8
/*
  fprintf (
    stdout,
    "The byte offset is %" PRIu64 "\n"
    "The bit offset is %" PRIu8 "\n"
    "The current byte value: %" PRIu8 "\n",
    static_cast<uint64_t> ( st_offset ),
    ui8_bit,
    m_vec [ st_offset ]
  );
*/
  uint8_t ui8_value = 1 << ui8_bit;

  //0000 0001 << 3 --> 0000 1000
  m_vec [ st_offset ] |= ui8_value;
/*
  fprintf (
    stdout,
    "{%" PRIu64 ",%" PRIu64 "}OR with this value: %" PRIu8 "\n"
    "Value after ORing: %" PRIu8 "\n",
    static_cast<uint64_t> ( st_x ),
    static_cast<uint64_t> ( st_y ),
    ui8_value,
    m_vec [ st_offset ]
  );
*/
}
void bits::clear ( size_t st_x, size_t st_y ) {
  size_t st_offset = (st_x + st_y * m_st_width);
  uint8_t ui8_bit = st_offset % 8; //take the remainder as the bit index
  st_offset >>= 3; //divide by 8

  //1111 0000 & 1000 0000 --> 1000 0000
  m_vec [ st_offset ] &= (uint8_t) ~(1 << ui8_bit);
}
uint8_t bits::get ( size_t st_x, size_t st_y ) const {
  size_t st_offset = (st_x + st_y * m_st_width);
  uint8_t ui8_bit = st_offset % 8;
  st_offset >>= 3;

/*
  fprintf (
    stdout,
    "{%" PRIu64 ", %" PRIu64 "} -> "
    "Byte offset: %" PRIu64 "; Bit offset: %" PRIu8 "\n",
    static_cast<uint64_t> ( st_x ),
    static_cast<uint64_t> ( st_y ),
    static_cast<uint64_t> ( st_offset ),
    ui8_bit
  );
*/
/*
  fprintf (
    stdout,
    "Getting ( %" PRIu64 ", %" PRIu64 " ) -> %" PRIu8 "\n",
    static_cast<uint64_t> ( st_x ),
    static_cast<uint64_t> ( st_y ),
    m_vec [ st_offset ]
  );
*/
  const uint8_t &ui8_value = m_vec [ st_offset ];
  uint8_t ui8_and = 1 << ui8_bit;
/*
  fprintf (
    stdout,
    "Checking bit %" PRIu8 " in byte %" PRIu8 "\n",
    ui8_and,
    ui8_value
  );
*/
  if ( ui8_value & ui8_and ) {
    return 1;
  }
  return 0;
  //return (m_vec [ st_offset ] & (1 << ui8_bit)) ? 1 : 0;
}

void bits::show_picture ( void ) const noexcept {
  for ( size_t st_y = 0; st_y < m_st_height; st_y ++  ) {
    for ( size_t st_x = 0; st_x < m_st_width; st_x ++ ) {
      fprintf ( stdout, "%c", get ( st_x, st_y ) ? '1' : '0' );
    }
    fprintf ( stdout, "\n" );
  }
}

void bits::flood_fill (
  size_t st_x,
  size_t st_y,
  uint8_t ui8_value_to_fill_with
) {
  if ( get ( st_x, st_y ) == ui8_value_to_fill_with ) {
    return ;
  }

  if ( ui8_value_to_fill_with ) {
    set ( st_x, st_y );
  }
  else {
    clear ( st_x, st_y );
  }

  //If the value left of us isn't out of bounds.
  if ( st_x > 0 ) {
    if ( get ( st_x-1, st_y ) != ui8_value_to_fill_with ) {
      flood_fill ( st_x-1, st_y, ui8_value_to_fill_with );
    }
  }

  //If the value to the right of us isn't out of bounds.
  if ( st_x < m_st_width-1 ) {
    if ( get ( st_x+1, st_y ) != ui8_value_to_fill_with ) {
      flood_fill ( st_x+1, st_y, ui8_value_to_fill_with );
    }
  }

  //If the value above us isn't out of bounds.
  if ( st_y > 0 ) {
    if ( get ( st_x, st_y-1 ) != ui8_value_to_fill_with ) {
      flood_fill ( st_x, st_y-1, ui8_value_to_fill_with );
    }
  }

  //If the value below us isn't out of bounds.
  if ( st_y < m_st_height - 1 ) {
    if ( get ( st_x, st_y+1 ) != ui8_value_to_fill_with ) {
      flood_fill (st_x, st_y+1, ui8_value_to_fill_with );
    }
  }
}

void bits::debug_dump ( void ) const {
  for ( const auto &v : m_vec ) {
    fprintf ( stdout, "%" PRIu8 " ", v );
  }
  fprintf ( stdout, "\n" );
}

int main ( void ) {

  uint8_t ui8ary_picture_test [ 6 ] = {
    0b0000'0000, //8w
    0b0011'1100,
    0b0100'0100, //2*8 + 2
    0b0100'0010,
    0b0010'0100,
    0b0001'1000 //6h
  };
/*
0000 0
0001 1
0010 2
0011 3
0100 4
0101 5
0110 6
0111 7
1000 8
1001 9
1010 a
1011 b
1100 c
1101 d
1110 e
1111 f

0xa2df2
1010 0010 1101 1111 0010
*/
  uint8_t ui8ary_picture [ 6 ] = {
    0x00,
    0x3c,
    0x44,
    0x42,
    0x24,
    0x18
  };
  if ( ! memcmp ( ui8ary_picture_test, ui8ary_picture, 6 ) ) {
    fprintf ( stdout, "They match!\n" );
  }
  else {
    fprintf ( stdout, "They DON'T match!\n" );
  }

  bits b ( 8, 6 );
  for ( size_t st_y = 0; st_y < 6; st_y ++ ) {
    for ( size_t st_x = 0; st_x < 8; st_x ++ ) {
      if ( ui8ary_picture [ st_y ] & (1 << (7 - st_x)) ) {
        b .set ( st_x, st_y );
      }
    }
  }

  //b .debug_dump (  );


  b .show_picture (  );

  b .flood_fill (
    2, //x
    2, //y
    1
  );

  b .show_picture (  );
  return 0;
}
