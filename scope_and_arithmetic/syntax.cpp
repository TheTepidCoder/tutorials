/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To explain basic C and C++
 syntax for mutating variables.

 I don't cover anything advanced like
 creating functions, function pointer
 variables, classes, enums, namespaces,
 operator overloads, ...etc. These will
 be explained as I move onto C++ topics.
*/
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

void foo ( const char *const* p ) {
  *p = 'd';
  p ++;
}

int main ( void ) {

  uint8_t a = 0;
  fprintf ( stdout, "a: %" PRIu8 "\n", a );

  a = 2;
  fprintf ( stdout, "a: %" PRIu8 "\n", a );

  a = a + 1; //3
  a += 17; //a = a + 17
  a -= 1; //a = a - 1
  a /= 2; //a = a / 2
  a *= 2; //a = a * 2
  a <<= 2; //a *= 4 a = a*(1*(2^2)) --> a = a << 2.
  a >>= 1; //a = a/2 --> a = a >> 1.
  a %= 2; //a = a % 2 (% means modulo -- which means the remainder of some division operation)
  //3 / 2 = 1.5 = 1 + 1/2 = 1 R 1 --> 3 % 2 = 1
  a |= 1; //0000 0000 -> 0000 0001 -> 0000 0001
  a &= 0xff; //0101 1010 & 1111 1111 = 0101 1010
  a ^= 0; //0000 0001 ^ 1111 1110 = 1111 1111 ^ (0xffFFffFFffffffff)
  a = ~a; //a ^= 0xfffff...ff
  a != a; //if anything but 0 -> 1; otherwise -> 0

  const int8_t i = 17;

  char c = 'c';
  foo ( (const char *const*) &c );

  return 0;
}
