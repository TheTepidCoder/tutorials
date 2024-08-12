#include "user.h"

#include "stdio.h"
#include <iostream>

int main ( void ) {

  //1.2.3.4
  user u ( "TheTepidCoder", 0x01020304 );
  std::cout << "Name: \"" << u .name (  ) << "\"" << std::endl;
  std::cout << "IPv4 Address: \"" << u.ipv4_address (  ) << "\"" << std::endl;
  return 0;
}
