#include "user.h"

user::user (  ) {
}
user::user ( const char *lpsz_name, uint32_t ui32_ipv4_address ) {
  m_str_name = lpsz_name;
  m_ui32_ipv4_address = ui32_ipv4_address;
}
user::~user (  ) {
}
[[nodiscard]] const std::string &user::name ( void ) const noexcept {
  return m_str_name;
}

uint32_t user::ipv4_address ( void ) const noexcept {
  return m_ui32_ipv4_address;
}

