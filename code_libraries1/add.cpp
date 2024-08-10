/*
 Created on 2024-08-04 by TheTepidCoder
 Purpose: To show an example of a how to
 create a code library in C and C++.
 Currently, this is exporting the only
 function as a C function.

 Note: This example doesn't include
 the __declspec(dllexport) directive
 that would be required when using
 Microsoft Visual Studio in order
 for the function to actually be
 referencable in the resultant
 DLL/LIB file. I went over this
 in a later video.
*/
extern "C" {
int add ( int a, int b ) {
  return a + b;
}
}
