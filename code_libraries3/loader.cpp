/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show how to build and use an import library.
*/
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

//#include "dlfcn.h" //dlopen / dlsym / dlclose
#include "windows.h" //LoadLibraryA/W / GetProcAddress / FreeLibrary

//int add ( int, int );

int main ( void ) {

  fprintf (
    stdout,
    "6+7: %" PRIi64 "\n",
    static_cast<int64_t> ( add ( 6, 7 ) )
  );
  return 0;
}
