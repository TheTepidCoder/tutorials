/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To show how to make a reasonably future-proof
 code library.
*/
#include "future_proof_library.h"

#include <windows.h> //LoadLibraryA / FreeLibrary / GetProcAddress
#include <iostream> //std::cout
#include <stdexcept> //std::runtime_error / std::exception
#include <format> //std::vformat
#include <string> //std::string
#include <memory> //std::unqiue_ptr<T> (dll)

//This class exists to call FreeLibrary upon destruction so that
//the usage/lifetime of a DLL can be scoped with an object.
class dll {
  private:
    HMODULE m_h_library = nullptr;

  public:
    dll (  ) = delete;
    dll ( const char *lpsz_file_path );
    dll ( const dll & ) = delete;
    dll &operator= ( const dll & ) = delete;
    //!! Todo: Define move assignment and move constructor.
    dll ( dll && ) = delete;
    dll &operator= ( dll && ) = delete;

    ~dll (  );
    [[nodiscard]] FARPROC get_function ( const char *lpsz_function_name ) const;
};
[[nodiscard]] FARPROC dll::get_function ( const char *lpsz_function_name ) const {
  return GetProcAddress ( m_h_library, lpsz_function_name );
}
dll::dll ( const char *lpsz_file_path ) {
  m_h_library = LoadLibraryA ( lpsz_file_path );
  if ( ! m_h_library ) {
    throw std::runtime_error (
      std::vformat (
        "Error: Unable to load the library, \"{}\"",
        std::make_format_args ( lpsz_file_path )
      )
    );
  }
}
dll::~dll (  ) {
  if ( m_h_library ) {
    FreeLibrary ( m_h_library );
  }
}

class future_proof_api {
  private:
    API m_api = { 0 };
    dll m_dll;

  public:
    future_proof_api (  );
    ~future_proof_api (  );
    future_proof_api ( const future_proof_api & ) = delete; //can't copy this object by initialization.
    future_proof_api &operator= ( const future_proof_api & ) = delete; //can't copy this object by assignment.

    //!! Todo: Implement the move constructor and move assignment operator overload.
    future_proof_api ( future_proof_api && ) = delete; //can't move construct
    future_proof_api &operator= ( future_proof_api && ) = delete; //can't move assign

    [[nodiscard]] const API &api ( void ) const noexcept;
};
[[nodiscard]] const API &future_proof_api::api ( void ) const noexcept {
  return m_api;
}
future_proof_api::future_proof_api (
) :
  //If this throws, we'll just let our caller handle the exception.
  m_dll ( "future_proof_library.dll" )
{
  initialize_future_proof_library_t initialize_future_proof_library =
    (initialize_future_proof_library_t) m_dll .get_function (
      "initialize_future_proof_library"
    );
  if ( ! initialize_future_proof_library ) {
    throw std::runtime_error ( "Error: Couldn't find the initialization function in the library." );
  }

  //A structure which contains all of the API functions exposed by future_proof_library.
  tcbool b_success = initialize_future_proof_library (
    FUTURE_PROOF_LIBRARY_VERSION, //__IN__ requested library version
    sizeof ( API ),
    &m_api //__OUT__ a structure to fill with the API function references
  );
  if ( ! b_success ) {
    throw std::runtime_error ( "Error: Unable to initialize the library." );
  }
}

future_proof_api::~future_proof_api (  ) {
  std::cout << __FUNCTION__ << " called." << std::endl << std::flush;
  //If the function was properly loaded.
  if ( m_api .uninitialize_future_proof_library ) {
    m_api .uninitialize_future_proof_library (  );
  }
}

int main ( void ) {
  std::unique_ptr<future_proof_api> uptr_fp_api;

  try {
    uptr_fp_api = std::make_unique<future_proof_api>();
  } catch ( std::exception &e ) {
    std::cerr << "Exception: \"" << e .what (  ) << "\"" << std::endl << std::flush;
    return 0;
  }

  const API &api = uptr_fp_api ->api (  );

  api .do_something (  ); //v1 or v2 (same interface; different implementation)

  //Note: For this to exist in the header, FUTURE_PROOF_LIBRARY_VERSION must be
  //defined to a value 2 or greater.
  api .do_something_else (  ); //v2+

  return 0;
}
