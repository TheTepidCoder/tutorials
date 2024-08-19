#include <iostream>
#include <vector>
#include <algorithm>

/*
namespace std {
  template<typename T>
  class vector { ... };

  template<typename T>
  std::vector<T>::vector(){...}
  template<typename T>
  std::vector<T>::vector (
    size_t st_element_count,
    const T &t_value
  ) {...}

  template<typename T>
  std::vector<T>::vector (
    typename std::initializer_list<T> initializer_list
  );
};
*/

void test001 ( void ) {
  std::vector<int> vec { 10, 'x' };
  for ( const auto &i : vec ) {
    std::cout << "Value: " << i << std::endl;
  }
}

int main (  ) {
  std::vector<int> vec {
    0, 1, 1, 1, 2, 3
  };
/*
  while ( vec [ 1 ] == 1 ) {
    vec .erase ( vec.begin ( ) + 1 );
  }
  for ( const auto &i : vec ) {
    std::cout << "Remaining value: " << i << std::endl;
  }
*/

  for (
    std::vector<int>::iterator it = vec .begin (  );
    it != vec .end (  );
    //it ++
  ) {
    //std::cout << "Current offset: " << (it - vec.begin()) << std::endl;

    std::cout << "Current offset: " <<
      std::distance (vec.begin(), it) <<
      std::endl;
    if ( *it == 1 ) {
      it = vec .erase ( it );
      continue;
    }

    it ++;
  }

  for ( const auto &i : vec ) {
    std::cout << "Remaining value: " << i << std::endl;
  }

  return 0;
}
