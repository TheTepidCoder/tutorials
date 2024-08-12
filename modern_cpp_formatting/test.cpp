#include <format> //std::vformat_to
#include <iterator>
#include <iostream> //std::cout

int main ( void ) {
  //char cary_string [ 256 ];
  //snprintf ( cary_string, 255, "This is a %.2f\n", "test" );

  std::ostream_iterator<char> out ( std::cout );

  std::string str = std::vformat (
    "Testing {}",
    std::make_format_args ( "123" )
  );
  std::cout << str << std::endl;

  std::vformat_to (
    out,
    "This is a {:.2f}\n"
    "Here's a string: {:.4}\n"
    "Here's a pre-padded integer value: {:02}\n",
    std::make_format_args (
      1.225f,
      "test_GARBAGE",
      10
    )
  );

  std::print (
    std::cout,
    "This is a {:.2f}\n"
    "Here's a string: {:.4}\n"
    "Here's a pre-padded integer value: {:02}\n",
    std::make_format_args (
      1.225f,
      "test_GARBAGE",
      10
    )
  );

  return 0;
}
