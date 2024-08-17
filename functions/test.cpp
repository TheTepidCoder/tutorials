/*
 Created on 2024-08-12 by TheTepidCoder
 Purpose: To show what kinds of functions
 can be created/used in C and how arguments
 are passed to them on the stack and
 cleaned up after the function returns,
 depending upon the calling convention.
*/
#include <iostream>
#include <cstdarg> //stdarg.h

do_something ( int a ) {
  std::cout << "Function \"" << __PRETTY_FUNCTION__ << "\"" << std::endl;
  return a;
}

//Expects a count of variadic arguments and
//for the variadic arguments to all be of type int.
void variadic ( size_t st_count, ... ) {
  va_list args;
  va_start ( args, st_count );

  for (
    size_t st_i = 0;
    st_i < st_count;
    st_i ++
  ) {
    std::cout << "Argument #" << (st_i + 1) << ": " <<
    va_arg ( args, int ) << std::endl;
  }

  va_end ( args );
}

/*
0: local variable va_args (for example)
1: argument 1
2: ...
3: n-1
4: argument n-1
.. argument n
n: push return point to stack


push last argument
push next to last (penultimate argument)
push n-2
n-3
...
push first argument
call function //__stdcall
;esp ebp
add esp, size of all of the arguments we pushed for the function call
*/

//__stdcall, __vectorcall, __fastcall
void __cdecl loop ( int i ) {
  std::cout << "Loop called with value " << i << "." << std::endl;
  if ( i > 0 ) {
    loop ( i - 1 );
  }
}

int main ( void ) {
  std::cout << "Result: " << do_something ( 6 ) << std::endl;

  variadic ( 3, 3, 1, 4 );

  loop ( 3 );

  return 0;
}
