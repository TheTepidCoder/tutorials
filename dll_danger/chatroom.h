/*
 Created on 2024-08-08 by TheTepidCoder
 Purpose: To be a dependency of our crash
 project which shares a common dependency
 (date_time_utility.dll) and which may
 expect to be using a different version
 of that date_time_utility DLL. This
 should be a decent explanation of
 undefined behavior in projects using
 DLL files.

 Just imagine that:
 1.) You're using a third-party DLL
 (let's call that dep1.dll).
 2.) dep1 requires dep2.dll.
 3.) Both dep1.dll and dep2.dll
 are third-party DLLs for which you
 don't have the source code.
 4.) You're project depends upon
 both dep1.dll and dep2.dll, directly.

 Possible outcomes:
 -Your project may be linked against
 the same version of dep2.dll which
 dep1.dll expects and everything will
 be fine.
 -Your project depends upon a version
 of dep2.dll which is newer or older
 than the one depended upon by dep1.dll
 and the behavior will be undefined if
 the ABI (Application Binary Interface)
 has changed (e.g. sizes of classes and
 structures; or function/method signatures).

 It's possible that things changed in
 the DLL but that structures/classes
 retain their previous sizes and none
 of the function signatures have changed.
 This may mean that there are no breaking
 changes between versions of the DLL.
*/
#pragma once
#ifndef CHATROOM_HEADER
#define CHATROOM_HEADER

#ifndef CHAT_ROOM_IMPORT_OR_EXPORT
#  ifdef BUILDING_CHATROOM
#    define CHAT_ROOM_IMPORT_OR_EXPORT __declspec(dllexport)
#  else
#    define CHAT_ROOM_IMPORT_OR_EXPORT __declspec(dllimport)
#  endif
#endif

#include <iostream> //std::cout / std::cin
#include <vector> //std::vector<T>
#include <stdexcept> //std::exception/std::runtime_error
#include <cstdint> //u?int#_t
#include <ctime> //struct tm (time structure); time_t (timestamp)
#include <iterator> //std::ostream_iterator
#include <format> //std::format/vformat/format_to/vformat_to

#include "date_time_utility.h"

#ifndef CONNECTION_INFORMATION_STRUCTURE
#define CONNECTION_INFORMATION_STRUCTURE
typedef struct CONNECTION_INFORMATION {
  int i_socket = 0;
  uint16_t ui16_port = 0;
  uint32_t ui32_ipv4_address = 0;
  date_time dt_connection_start;
} CONNECTION_INFORMATION;
#endif

class CHAT_ROOM_IMPORT_OR_EXPORT user {
  private:
    std::string m_str_name;
    CONNECTION_INFORMATION m_connection_information;

  public:
    user (  );
    user (
      const char *lpsz_name,
      const CONNECTION_INFORMATION &connection_information
    );
    ~user (  );
    bool name ( const char *lpsz_name );
    const char *name ( void ) const noexcept;
    [[nodiscard]] const CONNECTION_INFORMATION &get_connection_information ( void ) const noexcept;
};

class CHAT_ROOM_IMPORT_OR_EXPORT chatroom {
  private:
    std::vector<user> m_vec_users;

  public:
    chatroom (  );
    ~chatroom (  );
    bool add_user (
      const char *lpsz_name,
      const CONNECTION_INFORMATION &connection_information
    );
    const std::vector<user> &users ( void ) const noexcept;
};

#endif
