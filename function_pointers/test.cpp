#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"

//void(*)(void)
void print_something ( void ) {
  fprintf ( stdout, "something\n" );
}
void print_something_else ( void ) {
  fprintf ( stdout, "something else\n" );
}

void test ( void ) {
  //assign 0
  //add 1
  //sub 2
  void (*fnary_function_array[]) ( void ) = {
     print_something,
     print_something_else
  };
  for (
    size_t st_i = 0;
    st_i < sizeof ( fnary_function_array ) /
           sizeof ( void(*)(void) );
    st_i ++
  ) {
    fnary_function_array [ st_i ] (  );
  }
}

void print_your_thing (
  //void(*)(void)
  void(*fn_call_me)(void)
) {
  fn_call_me (  );
}

/*
void qsort(
   void *base,
   size_t number,
   size_t width,
   int (__cdecl *compare )(const void *, const void *)
)
*/

int __cdecl fn_compare_signed_8bit_integers (
  const void *lpv_left,
  const void *lpv_right
) {
  const int8_t &i8_left = *((const int8_t *) lpv_left);
  const int8_t &i8_right = *((const int8_t *) lpv_right);

  //left < right 1, 2 --> 1-2 = -1
  //left == right --> 1, 1 --> 1-1 = 0
  //left > right 2, 1 --> 2-1 = 1
  return i8_left - i8_right;
}

int main ( void ) {
  int8_t i8ary_values[] = { //-2^7 -> 2^7 - 1 --> [-128, 127]
    17, 12, 0, -13, 99, 127, 2, 12
  };
  const size_t st_array_element_count = sizeof(i8ary_values)/sizeof(int8_t);
  //print_your_thing ( print_something );
  qsort (
    i8ary_values,
    st_array_element_count,
    sizeof ( int8_t ),
    fn_compare_signed_8bit_integers
  );
  for ( size_t st_i = 0; st_i < st_array_element_count; st_i ++ ) {
    fprintf (
      stdout,
      "i8ary_values[%" PRIu64 "]: %" PRIi8 "\n",
      static_cast<uint64_t> ( st_i ),
      i8ary_values [ st_i ]
    );
  }

  return 0;
}
