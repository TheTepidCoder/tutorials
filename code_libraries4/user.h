#pragma once
#ifndef USER_HEADER
#define USER_HEADER

#include <string>
#include <cstdint>

class user {
  private:
    std::string m_str_name;
    uint32_t m_ui32_ipv4_address = 0; //0.0.0.0

  public:
    user (  );
    user ( const char *lpsz_name, uint32_t ui32_ipv4_address );
    ~user (  );
    [[nodiscard]] const std::string &name ( void ) const noexcept;
    uint32_t ipv4_address ( void ) const noexcept;
};

#endif
