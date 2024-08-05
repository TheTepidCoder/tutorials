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

char *load_file (
  const char *lpsz_file_name
) {
  FILE *fin = fopen ( lpsz_file_name, "rb" );

  if ( ! fin ) {
    fprintf (
      stderr,
      "Error: Couldn't load the file: \"%s\"\n",
      lpsz_file_name
    );
    return 0;
  }

  fseek ( fin, 0, SEEK_END );
  uint32_t ui32_file_size = ftell ( fin );
  fseek ( fin, 0, SEEK_SET );
  //void * -> char *
  //char *lpsz_file_contents = calloc ( sizeof ( char ), ui32_file_size + 1 );
  char *lpsz_file_contents = (char *) malloc ( ui32_file_size + 1 );
  if ( ! lpsz_file_contents ) {
    fprintf ( stderr, "Error: Couldn't allocate a buffer large enough to read the file.\n" );
    fclose ( fin );
    return 0;
  }
  lpsz_file_contents [ ui32_file_size ] = 0;

  fread (
    lpsz_file_contents, //void *
    sizeof ( char ), //element size
    ui32_file_size, //element count
    fin //file handle
  );

  fclose ( fin );
  return lpsz_file_contents;
}

typedef struct MAP {
  char *sz_key; //key\0
  char *sz_value; //value\0
} MAP;

int main ( void ) {
  MAP *lp_maps = 0;

  //Attempt to load the file into a buffer.
  char *lpsz_file_contents = load_file ( "load_me.txt" );

  //If we've failed to load the file, bail.
  if ( ! lpsz_file_contents ) {
    return 1;
  }

  fprintf (
    stdout,
    "Here's the file's contents:\n%s\n\n",
    lpsz_file_contents
  );

  size_t st_line_break_count = 0;
{
  const char *p = lpsz_file_contents;
  while ( *p ) {
    if ( *p == '\r' || *p == '\n' ) {
      st_line_break_count ++;
      if ( p [ 1 ] == '\r' || p [ 1 ] == '\n' ) {
        p += 2;
        continue;
      }
    }
    p ++;
  }
}

  lp_maps = (MAP *) calloc ( sizeof ( MAP ), st_line_break_count );
  if ( ! lp_maps ) {
    fprintf ( stderr, "Error: Couldn't allocate enough MAP structures.\n" );
    free ( lpsz_file_contents );
    return 1;
  }

{
  MAP *lp_current_map = lp_maps;
  char *p = lpsz_file_contents;
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
      free ( lpsz_file_contents );
      return 1;
    }
    *lpc_token = 0; //p => sky\0black\0...
    lpc_token ++; //lpc_token => black\0

    lp_current_map ->sz_key = p; //sky\0black\0...
    lp_current_map ->sz_value = lpc_token; //black\0

    p = e + 1;
    lp_current_map ++;
  }
}

  for (
    size_t st_i = 0;
    st_i < st_line_break_count;
    st_i ++
  ) {
    fprintf (
      stdout,
      "\"%s\" => \"%s\"\n",
      lp_maps [ st_i ] .sz_key,
      lp_maps [ st_i ] .sz_value
    );
  }

  if ( lp_maps ) {
    free ( lp_maps );
    lp_maps = 0;
  }

  if ( lpsz_file_contents ) {
    free ( lpsz_file_contents );
    lpsz_file_contents = 0;
  }

  return 0;
}
