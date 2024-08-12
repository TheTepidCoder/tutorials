/*
 Created on 2024-08-11 by TheTepidCoder
 Purpose: To show a couple ways of defining
 and using class method pointers.
*/
#include <iostream>
#include <functional>

class cls {
  public:
    cls (  );
    ~cls (  );
    void foo ( void );
    int sub ( int, int );
};

cls::cls (  ) {}
cls::~cls (  ) {}
void cls::foo ( void ) {
  std::cout << "Testing." << std::endl;
}
int cls::sub ( int a, int b ) {
  return a - b;
}

int main (  ) {
  cls c;
  void(cls::*fn_foo)(void) = &cls::foo;
  //int(cls::*fn_sub)(int, int) = &cls::sub;
  cls *lpc = &c;
  (lpc ->*fn_foo) (  );

  std::function<int(cls&,int,int)> fn_sub = &cls::sub;
  int i_difference = fn_sub ( c, 1, 2 );
  std::cout << "Difference: " << i_difference << std::endl;

  return 0;
}
