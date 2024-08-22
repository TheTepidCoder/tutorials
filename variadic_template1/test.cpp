#include <iostream>
#include <type_traits>

template<typename T, typename ...Targs>
void show_types (
  T t_value,
  Targs ...fargs
) {
  std::cout << "Type of argument: \"" << typeid ( T ) .name (  ) << "\"\n";

  //If float/double
  if constexpr ( std::is_floating_point<T>::value ) {
    std::cout << "This is a floating-point type.\n";
  }
  else if constexpr ( std::is_integral<T>::value ) {
    std::cout << "This is an integer type.\n";
  }
  else if constexpr ( std::is_same<T, const char *>::value ) {
    std::cout << "This is a const char *.\n";
  }

  if constexpr ( sizeof...(fargs) ) {
    std::cout << (unsigned int) sizeof...(fargs) <<
      " variadic argument" <<
      ((sizeof...(fargs) != 1) ? "s" : "") <<
      " were passed to the function." << std::endl;

    show_types ( fargs... ); //--> -3, 5.6f, 3.14 -> 5.6f, 3.14 -> 3.14
  }
}

int main ( void ) {
  show_types (
    "test", //const char *
    -3, //int
    5.6f, //float
    3.14 //double
  );

  return 0;
}
