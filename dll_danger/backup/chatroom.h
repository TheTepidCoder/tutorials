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
