/*
 Created on 2024-08-03 by TheTepidCoder
 Purpose: To explain how to create abstract
 classes in C++ (like interfaces) and
 use them through derived classes
 (implementations).

 The use case I chose was varying types
 of encryption to use, depending upon
 a customer's payment tier.

 Apparently, a base class can't have
 a pure virtual destructor. Whoops.
*/
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "inttypes.h"
#include "string.h"

#include <vector>
#include <memory>

enum customer_tiers : uint8_t {
  FREE_TIER,
  PAID_TIER
};

class encryption {
  private:
    //...
  protected:
    //..can be viewed by classes which derive from this one.

  public:
    encryption (  );
    virtual ~encryption (  );
    virtual void encrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept = 0;
    virtual void decrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept = 0;
};
encryption::encryption (  ) {
}
encryption::~encryption (  ) {
  fprintf ( stdout, "encryption::~encryption called.\n" );
}

class xor_encryption : public encryption {
  public:
    xor_encryption (  );
    virtual ~xor_encryption (  ) override;

    virtual void encrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept override;
    virtual void decrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept override;
};

xor_encryption::xor_encryption (  ) {
}
xor_encryption::~xor_encryption (  ) {
  fprintf ( stdout, "xor_encryption destructor called.\n" );
}
void xor_encryption::encrypt (
  std::vector<uint8_t> &v,
  const char *lpsz_password
) const noexcept {
  size_t st_password_length = strlen ( lpsz_password );
  size_t st_vector_length = v .size (  );
//123456789a [0 -> length-1]
//0123456789
//30/10 -> 1 R 0
//test
//t -> e -> s -> t -> t -> e -> s -> t -> t
  for ( size_t st_i = 0; st_i < st_vector_length; st_i ++ ) {
    v [ st_i ] ^= lpsz_password [ st_i % st_password_length ];
  }
}
void xor_encryption::decrypt (
  std::vector<uint8_t> &v,
  const char *lpsz_password
) const noexcept {
  encrypt ( v, lpsz_password );
}
void print_vector (
  const std::vector<uint8_t> &v
) {
  for ( const auto &val : v ) {
    fprintf ( stdout, "Value: \"%" PRIu8 "\"\n", val );
  }
}


class caesar_encryption : public encryption {
  public:
    caesar_encryption (  );
    virtual ~caesar_encryption (  ) override;
    virtual void encrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept override;
    virtual void decrypt (
      std::vector<uint8_t> &v,
      const char *lpsz_password
    ) const noexcept override;

    void shift (
      std::vector<uint8_t> &v,
      const char *lpsz_password,
      bool b_decrypt //if decrypting, then shift left (subtract)
    ) const noexcept;

};

caesar_encryption::caesar_encryption (  ) {
}
caesar_encryption::~caesar_encryption (  ) {
  fprintf ( stdout, "caesar_encryption::~caesar_encryption called.\n" );
}

void caesar_encryption::shift (
   std::vector<uint8_t> &v,
   const char *lpsz_password,
   bool b_decrypt //if decrypting, then shift left (subtract)
) const noexcept {
  uint8_t *lpui8_p = &v [ 0 ];
  uint8_t *lpui8_e = &v [ v .size (  ) ]; //sentinel

  size_t st_index = 0;
  size_t st_password_length = strlen ( lpsz_password );
  char c_sign = b_decrypt ? -1 : 1;

  while ( lpui8_p < lpui8_e ) {
    *lpui8_p += c_sign * lpsz_password [ st_index % st_password_length ];
    lpui8_p ++;
    st_index ++;
  }
}

void caesar_encryption::encrypt (
   std::vector<uint8_t> &v,
   const char *lpsz_password
) const noexcept {
  shift ( v, lpsz_password, false );
}
void caesar_encryption::decrypt (
   std::vector<uint8_t> &v,
   const char *lpsz_password
) const noexcept {
  shift ( v, lpsz_password, true );
}

class customer {
  private:
    encryption *m_enc = nullptr;

  public:
    customer (  ) = delete;
    customer ( encryption *e );
    ~customer (  );
    void encrypt ( std::vector<uint8_t> &v, const char *lpsz_password ) const;
    void decrypt ( std::vector<uint8_t> &v, const char *lpsz_password ) const;
};

customer::customer ( encryption *enc ) {
  m_enc = enc;
}
customer::~customer (  ) {
  fprintf ( stdout, "customer destructor called.\n" );
  delete m_enc;
}
void customer::encrypt ( std::vector<uint8_t> &v, const char *lpsz_password ) const {
  m_enc ->encrypt ( v, lpsz_password );
}
void customer::decrypt ( std::vector<uint8_t> &v, const char *lpsz_password ) const {
  m_enc ->decrypt ( v, lpsz_password );
}

void test ( void ) {
  encryption *e = new xor_encryption (  );

  uint8_t ui8ary_values [ 4 ] = { 'T', 'e', 's', 't' };
  std::vector<uint8_t> v;
  v .insert (
    v .begin (  ),
    ui8ary_values, //starting element [S -> E)
    ui8ary_values + sizeof ( ui8ary_values ) //the sentinel element
  );

  print_vector ( v );

  e ->encrypt ( v, "abc" );

  print_vector ( v );

  e ->decrypt ( v, "abc" );

  print_vector ( v );

  delete e;

  encryption *e2 = new caesar_encryption ( );
  print_vector ( v );
  e2 ->encrypt ( v, "abc" );
  print_vector ( v );
  e2 ->decrypt ( v, "abc" );
  print_vector ( v );

  delete e2;
}

int main ( int argc, char *argv[] ) {

  //customer*
  std::unique_ptr<customer> uptr_cust;

  if ( argc < 2 ) {
    fprintf (
      stdout,
      "Usage: %s [customer payment tier] (e.g. 0 = free user; 1 = paid)\n",
      argv [ 0 ]
    );
    return 0;
  }

  //test.exe /^[01]$/
  switch ( atoi(argv [ 1 ]) ) {
    case FREE_TIER: {
      uptr_cust = std::make_unique<customer> ( new caesar_encryption() );
      break;
    }
    case PAID_TIER: {
      uptr_cust = std::make_unique<customer> ( new xor_encryption() );
      break;
    }
    default: {
      fprintf ( stderr, "Error: Invalid customer tier specified.\n" );
      return 0;
    }
  }


  std::vector<uint8_t> v;
  char sz_string [  ] = "This is a secret.";
  v .insert (
    v .end (  ),
    (uint8_t *) sz_string,
    ((uint8_t *) sz_string) + sizeof ( sz_string )
  );

  for ( const auto &c : v ) {
    fprintf ( stdout, "%c", c );
  }
  fprintf ( stdout, "\n" );

  uptr_cust ->encrypt ( v, "abc123" );

  for ( const auto &c : v ) {
    fprintf ( stdout, "%c", c );
  }
  fprintf ( stdout, "\n" );

  uptr_cust ->decrypt ( v, "abc123" );

  for ( const auto &c : v ) {
    fprintf ( stdout, "%c", c );
  }
  fprintf ( stdout, "\n" );

//  delete cust;

  return 0;
}
