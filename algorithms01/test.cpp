/*
 Created on 2024-08-12 by TheTepidCoder
 Purpose: To show off a couple of common <algorithm>
 functions and introduce <filesystem>, since it's
 very useful for many types of projects.

 This will be the first in a kind of mini-series
 that describes some out-of-the-box C++ features
 that you may want to use for various projects.
*/
#include <vector> //std::vector<T>::erase/insert/push_back/...
#include <algorithm> //std::find / std::set_symmetric_difference
#include <filesystem>
#include <iostream>

std::vector<std::string> get_directory_contents (
  const char *lpsz_path
) {
  std::vector<std::string> vec_paths;
  std::filesystem::path path_starting_directory { lpsz_path };
  for (
    auto &dir_entry :
    std::filesystem::directory_iterator { path_starting_directory }
  ) {
    std::cout << "Path: \"" << dir_entry << "\"" << std::endl;

    vec_paths .emplace_back (
      dir_entry .path (  ) .filename (  ) .generic_string (  )
    );
  }
  return vec_paths;
}

int main ( void ) {
  std::vector<std::string> vec_paths1 = get_directory_contents (
    "./search_me/"
  );
  std::vector<std::string> vec_paths2 = get_directory_contents (
    "./search_me2/"
  );

  std::vector<std::string> vec_unique_files;
  std::set_symmetric_difference (
    vec_paths1 .begin (  ),
    vec_paths1 .end (  ),
    vec_paths2 .begin (  ),
    vec_paths2 .end (  ),
    std::back_inserter ( vec_unique_files )
  );
  for ( const auto &str_file_name : vec_unique_files ) {
    std::cout << "Unique file: \"" << str_file_name << "\"" << std::endl;
    std::vector<std::string>::iterator it = std::find (
      vec_paths1 .begin (  ),
      vec_paths1 .end (  ),
      str_file_name
    );
    if ( it == vec_paths1 .end (  ) ) {
      std::cout << "File \"" <<  str_file_name<< "\" " <<
        "is missing from ./search_me/" << std::endl;
    }
    else {
      std::cout << "File \"" <<  str_file_name<< "\" " <<
        "is missing from ./search_me2/" << std::endl;
    }
  }

  return 0;
}
