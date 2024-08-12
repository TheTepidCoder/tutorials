#include <print> //std::print
#include <iostream>

int main (  ) {
  srand ( static_cast<unsigned int> ( time(nullptr) ) );
  int r = 1 + rand() % 10;
  std::print (
    std::cout,
    "This is only {}\n",
    r
  );
  return 0;
}