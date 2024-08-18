/*
 Created on 2024-08-13 by TheTepidCoder
 Purpose: To show off some more out-of-the-box
 C++ functionality.
 I cover the following container methods:
 //Erase the entire string by default.
 //The first argument can be overriden
 //from its default value to erase everything
 //starting from some offset until the end
 //of the string.
 //The second argument may also be overriden
 //to delete only the first N elements, starting
 //from the starting position.
 std::string::erase (
   size_t st_offset = 0,
   size_t st_element_count = 0
 );
 //Erase only a single character referenced by
 //the std::string::iterator (pointers are accepted).
 std::string::erase ( iterator );
 //Erase elements in the range [it_start, it_end).
 std::string::erase ( it_start, it_sentinel );

 //Erase elements in the range [it_start, it_sentinel)
 std::vector<T>::erase ( it_start, it_sentinel );
 //Erase this element.
 std::vector<T>::erase ( it );
*/
#include <iostream>
#include <fstream>
#include <string>

std::string load_file (
  const char *lpsz_file_path
) {
  std::string str_file_contents;
  std::fstream fs { lpsz_file_path, std::ios_base::in };
  if ( ! fs .is_open (  ) ) {
    return str_file_contents;
  }

  //g = get (reading from); p = put (writing to)
  fs .seekg ( 0, std::ios_base::end );
  size_t st_file_size = fs .tellg (  );
  str_file_contents .resize ( st_file_size, 0 );
  fs .seekg ( 0, std::ios_base::beg );

  fs .read (
    str_file_contents .data (  ),
    st_file_size
  );

  return str_file_contents;
}

int main (  ) {

  std::string str_file_contents = load_file ( "test.txt" );

  size_t st_comment_offset = 0;
  while (
    std::string::npos !=
    (st_comment_offset = str_file_contents .find (
      "//",
      st_comment_offset ? st_comment_offset + 1 : st_comment_offset
    ))
  ) {
    std::cout << "Comment found at offset: " << st_comment_offset << std::endl;

/*
    //test\n
    //est\n
    //st\n
    //t\n
    //\n
    while (
      str_file_contents [ st_comment_offset ] != '\r' &&
      str_file_contents [ st_comment_offset ] != '\n'
    ) {
      str_file_contents .erase (
        str_file_contents .begin (  ) + st_comment_offset
      );
      //vec .erase ( vec .begin (  ) + st_comment_offset )
    } //While we're deleting characters leading up to the first line break character.

    //\r\n
    //\n\r
    //\n
    while (
      str_file_contents [ st_comment_offset ] == '\r' ||
      str_file_contents [ st_comment_offset ] == '\n'
    ) {
      str_file_contents .erase (
        str_file_contents .begin (  ) + st_comment_offset
      );
    } //While we're deleting line break characters
*/

    //\r\n, \n\r, \n
    size_t st_line_end = str_file_contents .find (
      '\n',
      st_comment_offset + 2
    );
    if ( st_line_end != std::string::npos ) {
      while (
        str_file_contents [ st_line_end ] == '\r' ||
        str_file_contents [ st_line_end ] == '\n'
      ) {
        st_line_end ++;
      }
      str_file_contents .erase (
        str_file_contents .begin (  ) + st_comment_offset,
        str_file_contents .begin (  ) + st_line_end //[s,e)
      );
      //std::vector<char> vec;
      //vec .erase ( vec.begin() + st_comment_offset, vec.begin() + st_line_end );
      //std::vector<T>::erase ( std::vector::begin() + start, std::vector<T>::begin() + end )
/*
      //test
      //0123 e-s+1
      str_file_contents .erase (
        st_comment_offset,
        st_line_end - st_comment_offset
      );
*/
    }
    else {
      str_file_contents .erase ( st_comment_offset );
    }

  } //While we're able to find comments to remove

  std::cout << "File contents:\n" << str_file_contents << std::endl;

  return 0;
}
