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
