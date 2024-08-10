/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show an example of a how to
 create a code library in C and C++.
 Currently, this is exporting the only
 function as a C function.

 Note: The *nix version of the code with
 dlfcn.h -> dlopen/dlclose/dlsym is untested.
*/
#include "windows.h"
#include "stdio.h"
#include "inttypes.h"
#include "stdint.h"
//#include "dlfcn.h"

//int add ( int, int );
/*
void test ( void ) {
  void *lpv_dll = dlopen ( "add.so", RTLD_NOW );
  if ( ! lpv_dll ) {
    fprintf ( stderr, "Error: Couldn't load the dll file.\n" );
    return 0;
  }

  int (*fn_add) (int, int) =
    (int(*)(int, int)) dlsym ( lpv_dll, "add" );

  if ( ! fn_add ) {
    fprintf ( stderr, "Error: Couldn't load the add function from the dll file.\n" );
    dlclose ( lpv_dll );
    return 0;
  }

  fprintf (
    stdout,
    "7+2 = %" PRIi64 "\n",
    static_cast<int64_t> ( fn_add ( 7, 2 ) )
  );

  dlclose ( lpv_dll );
}
*/

int main ( void ) {
  HMODULE h_library = (HMODULE) LoadLibraryA ( "add.dll" );
  if ( ! h_library ) {
    fprintf ( stderr, "Error: Couldn't load the dll file.\n" );
    return 0;
  }

  int (*fn_add) (int, int) =
    (int(*)(int, int)) GetProcAddress ( h_library, "add" );

  if ( ! fn_add ) {
    fprintf ( stderr, "Error: Couldn't load the add function from the dll file.\n" );
    FreeLibrary ( h_library );
    return 0;
  }

  fprintf (
    stdout,
    "7+2 = %" PRIi64 "\n",
    static_cast<int64_t> ( fn_add ( 7, 2 ) )
  );

  FreeLibrary ( h_library );

  return 0;
}
