#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"

#include <algorithm> //std::find, std::sort, ...
#include <functional> //std::function<R(T, T, T, ...)>

/*
void qsort(
   void *base,
   size_t number,
   size_t width,
   int (__cdecl *compare )(const void *, const void *)
   //std::function<int(const void *,const void *)> compare
)
*/

void pass_me_something_to_call (
  void (*lpfn_c_function)(void),
  std::function<void(void)> fn_cpp_function
) {
  lpfn_c_function (  );
  fn_cpp_function (  );
}

int main ( void ) {

  int8_t i8ary_values[] = { //-2^7 -> 2^7 - 1 --> [-128, 127]
    17, 12, 0, -13, 99, 127, 2, 12
  };
  const size_t st_array_element_count = sizeof(i8ary_values)/sizeof(int8_t);
  //print_your_thing ( print_something );
  size_t st_number_of_times_the_function_was_called = 0;
  //std::function<>
//template< class RandomIt, class Compare >
//void sort( RandomIt first, RandomIt last, Compare comp );
//[first, sentinel)
  std::sort ( //<int8_t, int(const int8_t&, const int8_t&)> (
    i8ary_values,
    i8ary_values + st_array_element_count,
    [&st_number_of_times_the_function_was_called] ( //std::function
     const int8_t &i8_left,
     const int8_t &i8_right
    ) -> bool {

      //fprintf ( stdout, "Something happened.\n" );
      st_number_of_times_the_function_was_called ++;

      return i8_left < i8_right;
    }
  );

  fprintf (
    stdout,
    "It took %" PRIu64 " function calls to std::sort's callback function in "
    "order to sort the array.\n",
    static_cast<uint64_t> ( st_number_of_times_the_function_was_called )
  );

  for ( size_t st_i = 0; st_i < st_array_element_count; st_i ++ ) {
    fprintf (
      stdout,
      "i8ary_values[%" PRIu64 "]: %" PRIi8 "\n",
      static_cast<uint64_t> ( st_i ),
      i8ary_values [ st_i ]
    );
  }

  pass_me_something_to_call (
    []() -> void {
      fprintf ( stdout, "This is the C function as a lambda.\n" );
    },
    [&]() -> void {
      fprintf ( stdout, "This is the C++ function as a lambda.\n" );
    }
  );

  return 0;
}
