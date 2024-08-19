/*
 Created on 2024-08-14 by TheTepidCoder
 Purpose: To show how to use std::reverse
 and give a practical example of why want
 might want to do so.
*/
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <cmath>

//Convert a signed 64 bit integer into
//a std::string representation with
//the digits grouped into sets of
//three with commas delimiting the
//groups for improved readability.
std::string format_integer (
  int64_t i64_value
) {
  bool b_negative = i64_value < 0;
  std::string str;
  int16_t i16_remainder;
  while ( true ) {
    i16_remainder = i64_value % 1'000;
    i64_value -= i16_remainder;
    i64_value /= 1'000;
    std::string str_group = std::to_string ( abs(i16_remainder) );
    std::reverse ( str_group .begin (  ), str_group .end (  ) );
    str .append ( str_group );
    if ( ! i64_value ) {
      break;
    }
    str .append ( "," );
  }
  if ( b_negative ) {
    str .append ( "-" );
  }
  std::reverse ( str .begin (  ), str .end (  ) );
  return str;
}

int main ( void ) {
  int64_t i64_number = -123'456'789'777'666'555;
  std::string str_number = format_integer ( i64_number );
  std::cout << "Here's the formatted integer: " << str_number << "\n";

/*
  a b c --> c b a  std::reverse ( vec.begin(), vec.begin() + row_width - 1 )
  d e f --> f e d  std::reverse ( vec.begin() + row_width, vec.begin()+row_width*2 - 1 )
  g h i --> i h g  std::reverse ( vec.begin() + row_width*2, vec.begin()+row_width*3 - 1 )

  std::reverse ( vec.begin(), vec.end() );
  c b a
  f e d
  i h g

  x+y*w --> x+y*w*(h-1)
*/

  //Iterate through each character of the std::string, in reverse.
  for (
    std::string::const_reverse_iterator it = str_number .crbegin (  );
    it != str_number .crend (  );
    it ++
  ) {
    std::cout << "Next character: '" << *it << "'\n";
  }

  return 0;
}
