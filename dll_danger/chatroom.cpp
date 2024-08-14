/*
 Created on 2024-08-08 by TheTepidCoder
 Purpose: To be a fake "chatroom" class
 which would handle some messages between
 connected users by forwarding a queue
 of messages to propogate to all connected
 clients' sockets with source information.

 In reality, this exists only to depend
 upon the date_time class object and store
 its contents in a std::vector<date_time>
 (or something that encapsulates the
 date_time object -- the user class defined
 hereinbelow). This will cause undefined
 behavior when this is compiled with an
 old version of the date_time class, if
 the class header file has a date_time
 class which differs. E.g. if it's
 compiled and linked against a version
 of date_time_utilities.dll, and a newer
 version of that dll is what is loaded
 at run-time by the consumer of this
 chatroom.dll, then that is an error.
*/
#include "date_time_utility.h"
#include "chatroom.h"

user::user (  ) {  }

user::user (
  const char *lpsz_name,
  const CONNECTION_INFORMATION &connection_information
) :
  m_str_name ( lpsz_name ),
  m_connection_information ( connection_information )
{
}

user::~user (  ) {
}

bool user::name ( const char *lpsz_name ) {
  try {
    m_str_name = lpsz_name ? lpsz_name : "";
  } catch ( std::exception &e ) {
    std::cerr << "Exception: \"" << e .what (  ) << "\"" << std::endl;
    return false;
  }
  return true;
}

const char *user::name ( void ) const noexcept {
  return m_str_name .c_str (  );
}

[[nodiscard]] const CONNECTION_INFORMATION &user::get_connection_information ( void ) const noexcept {
  return m_connection_information;
}




chatroom::chatroom(){}
chatroom::~chatroom(){}
bool chatroom::add_user (
  const char *lpsz_name,
  const CONNECTION_INFORMATION &connection_information
) {
  try {
    //number of elements * size of a single element = total size in bytes
    m_vec_users .emplace_back ( lpsz_name, connection_information );
  }
  catch ( std::exception &e ) {
    std::cerr << "Exception: \"" << e .what (  ) << "\"\n" << std::endl;
    return false;
  }

  return true;
}
const std::vector<user> &chatroom::users ( void ) const noexcept {
  return m_vec_users;
}
