/*
 Created on 2024-09-07 by TheTepidCoder
 Purpose: To show how to use std::binary_search,
 std::lower_bound, and std::upper_bound.
*/
#include <iostream>
#include <algorithm> //std::binary_search, std::upper_bound, std::lower_bound
#include <vector>

#define countof(a) (sizeof(a)/sizeof(a[0]))

int test1 ( void ) {
  const int i_find_me = 9;
  int iary_values [ 8 ] = {
    0, 1, 1, 2, 5, 7, 9, 11
  //0  1  2  3  4  5  6  7
  };
  int *lpi_sentinel = &iary_values [ countof ( iary_values ) ];

  std::cout << "Testing std::binary_search:\n";
  bool b_exists = std::binary_search (
    iary_values, //int[] -> int *
    lpi_sentinel,
    i_find_me
  );
  std::cout << "The value, " << i_find_me <<
    ", exists within the array: " <<
    std::boolalpha << b_exists << ".\n";

{
  std::cout << "\nTesting std::lower_bound:\n";
  //[ std::vector<int>::begin(), std::vector<int>::end() )
  //Finds the first value in the range of elements which
  //is not < the value specified; or it returns the sentinel,
  //if no such element can be found within the specified range.
  int *lpi_value = std::lower_bound (
    iary_values,
    lpi_sentinel,
    i_find_me
  );

  if ( lpi_value == lpi_sentinel ) {
    std::cout << "Neither " << i_find_me << " nor a greater value could be found in the array.\n";
    return 0;
  }
  std::cout << "The value, " << *lpi_value << ", was found in the array.\n";
}

{
  std::cout << "\nTesting std::upper_bound:\n";
  //This will return a reference to the first element which is > the value specified;
  //or it will return the sentinel, if no such element exists within the specified range.
  int *lpi_value = std::upper_bound (
    iary_values,
    lpi_sentinel,
    i_find_me
  );

  if ( lpi_value == lpi_sentinel ) {
    std::cout << "No value greater than " << i_find_me << " could be found in the specified range of elements.\n";
    return 0;
  }

  std::cout << "The value, " << *lpi_value << ", was found in the array.\n";
}

  return 1;
}

int main ( void ) {
  const int i_find_me = 10;
  std::vector<int> vec = {
    0, 1, 1, 2, 5, 7, 9, 11
  };
  std::vector<int>::iterator it =
    std::lower_bound (
      vec .begin (  ),
      vec .end (  ),
      i_find_me
    );

  if ( it == vec .end (  ) ) {
    std::cout << "Neither " << i_find_me << " nor a greater value exists in the vector.\n";
    vec .insert ( it, i_find_me );
  }
  else if ( *it != i_find_me ) {
    std::cout << i_find_me << " doesn't exist within the vector, however, a larger " <<
      "value, " << *it << ", does exist.\n";
    vec .insert ( it, i_find_me );
  }
  else {
    std::cout << i_find_me << " already exists within the vector.\n";
  }

  for ( const auto &i_value : vec ) {
    std::cout << "Value: \"" << i_value << "\"\n";
  }

  return 0;
}
