/*
 Created on 2024-08-08 by TheTepidCoder
 Purpose: To show potential problems with returning
 objects from DLLs that you didn't create or
 reallocating/freeing memory returned from a DLL.
 To build:
   Use ./build.bat in Powershell (or Terminal).

 TODO:
 -Break the chatroom/date_time class into its own
 utility DLL.

 -Add a new dependency which requires the utility
 DLL. It should be something like the the chatroom
 class object, or perhaps a webserver object.

 -Also require the utility DLL in this project
 (perhaps just the date_time aspect).

 -Update the date_time object to use the extra
 structure members that I have commented out
 so that the structure grows.

 -Update the CONNETION_INFORMATION structure
 to include the date_time class so that it
 also grows.

 Maybe this application can use the date_time
 class to get the date & time that the server
 started listening for clients.

 Notes:
 -At run-time, the chatroom/webserver DLL
 dependency of this crash project should
 load the utility DLL.
 -At run-time, this crash project should also
 load the utility DLL.
 -Each will expect a different version of the
 same DLL.
 -One of them will get the incorrect version
 from the current directory (-L. -l:utility.dll).
 -This should cause undefined behavior (and
 hopefully show off an intentional crash for
 educational purposes).
*/
#include <iostream> //std::cout / std::cin
#include <vector> //std::vector<T>
#include <stdexcept> //std::exception/std::runtime_error
#include <cstdint> //u?int#_t
#include <ctime> //struct tm (time structure); time_t (timestamp)
#include <iterator> //std::ostream_iterator
#include <format> //std::format/vformat/format_to/vformat_to
#include <fstream>

/*
-L. -l:some_library.lib -l:lib1.lib -l:lib2.lib ...
lib1.lib hello_world -> "hello world"
lib2.lib hello_world -> "Hello, world."

  crash
   \   \
    \   _chatroom -> date_time_utility
     _date_time_utility

1.) Use the same compiler for all dependencies that you control.
2.) Use the same VERSION of the same compiler for all dependencies that you control.
3.) Free/resize any memory from the DLL using only functions/methods exposed by the DLL.
4.) Destroy any objects created by the DLL within the DLL or using methods/functions
exposed by the DLL.
5.) Don't return any objects that you down own from a DLL.
6.) Expose a function which intializes/uninitializes the DLL.

load dll
get function addresses to use
call date_time_utility_init();
call date_time_utility_uninit();
*/

#include "date_time_utility.h"
#include "chatroom.h"

int main ( void ) {
  std::ostream_iterator<char> cout { std::cout };

  date_time dt_now, dt_previous_date_time;
  if ( ! dt_previous_date_time .load ( "previous_date_time.dat" ) ) {
    std::cout << "Note: Previous date time doesn't exist. " <<
      "We will default to the current time." << std::endl;
  }
  std::vformat_to (
    cout,
    "Previous date time: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
    std::make_format_args (
      dt_previous_date_time .year (  ),
      dt_previous_date_time .month (  ),
      dt_previous_date_time .day (  ),
      dt_previous_date_time .hour (  ),
      dt_previous_date_time .minute (  ),
      dt_previous_date_time .second (  )
    )
  );
  std::vformat_to (
    cout,
    "Current date time: {:04}-{:02}-{:02} {:02}:{:02}:{:02}\n",
    std::make_format_args (
      dt_now .year (  ),
      dt_now .month (  ),
      dt_now .day (  ),
      dt_now .hour (  ),
      dt_now .minute (  ),
      dt_now .second (  )
    )
  );
  if ( ! dt_now .save ( "previous_date_time.dat" ) ) {
    std::cerr << "Error: We couldn't store the current date time." << std::endl;
  }

  chatroom cr_chatroom;
  CONNECTION_INFORMATION ci_connection_information;
  ci_connection_information .i_socket = 1;
  ci_connection_information .ui16_port = 16384;
  ci_connection_information .ui32_ipv4_address = 0x01020304; //should be big endian
  cr_chatroom .add_user (
    "TheTepidCoder",
    ci_connection_information
  );

  for ( const auto &usr : cr_chatroom .users (  ) ) {
    std::cout << "Name: \"" << usr .name (  ) << "\"" << std::endl;
    const CONNECTION_INFORMATION &ci = usr .get_connection_information (  );
    //std::cout << "ipv4 address: \"" << ci .ui32_ipv4_address << "\"" << std::endl;
    std::vformat_to (
      cout,
      "Ipv4 Address {}.{}.{}.{}\n", //or {:08x}
      std::make_format_args (
        (ci .ui32_ipv4_address & 0xff000000) >> 24,
        (ci .ui32_ipv4_address & 0xff0000) >> 16,
        (ci .ui32_ipv4_address & 0xff00) >> 8,
        (ci .ui32_ipv4_address & 0xff)
      )
    );
    std::cout << "Port: \"" << ci .ui16_port << "\"" << std::endl;
    std::cout << "Socket: \"" << ci .i_socket << "\"" << std::endl;
    std::cout << std::flush;
  }

  return 0;
}
