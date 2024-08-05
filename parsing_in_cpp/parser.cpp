/*
 Created on 2024-08-02 by TheTepidCoder
 Purpose: To explain dynamic memory allocation
 and management, file access, and string parsing
 with pointers.
*/
#include "stdio.h" //printf fprintf
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"

#include <string>
#include <vector>

#ifndef _IN_
#  define _IN_
#endif

#ifndef _OUT_
#  define _OUT_
#endif

#ifndef _IN_OUT_
#  define _IN_OUT_
#endif

bool load_file (
  _IN_ const char *lpsz_file_name,
  _OUT_ std::vector<char> &v_file_contents
) {
  v_file_contents .clear (  );
  FILE *fin = fopen ( lpsz_file_name, "rb" );

  if ( ! fin ) {
    fprintf (
      stderr,
      "Error: Couldn't open the file: \"%s\"\n",
      lpsz_file_name
    );
    return false;
  }

  fseek ( fin, 0, SEEK_END );
  uint32_t ui32_file_size = ftell ( fin );
  fseek ( fin, 0, SEEK_SET );

  try {
    v_file_contents .resize ( ui32_file_size + 1, 0 );
  } catch ( std::exception &e ) {
    //std::bad_alloc
    fprintf ( stderr, "Exception: \"%s\"\n", e .what (  ) );
    fclose ( fin );
    return false;
  }

  fread (
    v_file_contents .data (  ), //void *
    sizeof ( char ), //element size
    ui32_file_size, //element count
    fin //file handle
  );

  fclose ( fin );
  return true;
}

typedef struct MAP {
  std::string str_key;
  std::string str_value;
} MAP;

int main ( void ) {
  std::vector<MAP> v_maps;
  std::vector<char> v_file_contents;
  if ( ! load_file ( "load_me.txt", v_file_contents ) ) {
    fprintf ( stderr, "Error: Couldn't load the file! Giving up.\n" );
    return 1;
  }

  fprintf (
    stdout,
    "Here's the file's contents:\n%s\n\n",
    &v_file_contents [ 0 ]
  );

{
  char *p = v_file_contents .data (  );// &v_file_contents [ 0 ]
  char *e;
  while ( e = strchr ( p, '\n' ) ) {
    //p => start of the line: sky=black\n
    //e => '\n'clouds=grey
    *e = 0;
    char *lpc_token = strchr ( p, '=' );
    if ( ! lpc_token ) {
      fprintf (
        stderr,
        "Error: Every key and value pair must be delimited by an '=' token.\n"
      );
      return 1;
    }
    *lpc_token = 0; //p => sky\0black\0...
    lpc_token ++; //lpc_token => black\0

    MAP map;
    //fprintf ( stdout, "Key location: %p\nContents: \"%s\"\n", p, p );
    //fflush ( stdout );

    map .str_key = p; //std::string ( p ); //std::string ( p, e ) //[p,e)
    //fprintf ( stdout, "Value location: %p\nContents: \"%s\"\n", lpc_token, lpc_token );
    //fflush ( stdout );

    map .str_value = lpc_token;

//    v_maps .push_back ( map );

//    v_maps .insert (
//      v_maps .end (  ),
//      map
//    );

    v_maps .emplace_back (
      std::move ( map )
    );

    p = e + 1;
  }
}
/*
  //std::deque / std::vector // std::map<K, V>
  for (
    size_t st_i = 0;
    st_i < v_maps .size (  );
    st_i ++
  ) {
    //v_maps [ st_i ]
*/
/*
  for (
    std::vector<MAP>::const_iterator it = v_maps .cbegin (  );
    it != v_maps .cend (  );
    it ++
  ) {
    fprintf (
      stdout,
      "\"%s\" => \"%s\"\n",
      (*it) .str_key .c_str (  ), //std::string::c_str() -> const char *
      it ->str_value .c_str (  )
    );
  }
*/
  for ( const auto &map : v_maps ) {
    fprintf (
      stdout,
      "\"%s\" => \"%s\"\n",
      map .str_key .c_str (  ), //std::string::c_str() -> const char *
      map .str_value .c_str (  )
    );
  }

  return 0;
}
