/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To show how to make a reasonably future-proof
 code library.

 Rules:
 1.) All exposed API functions should be versioned by
 sticking them in their own namespaces.
 2.) The API functions must be returned by a structure
 that is filled with function references to functions
 from a specific version of the API.
 3.) Any functions newer than what would exist in the
 requested API must be inaccessible. (They will be
 defaulted to an invalid function pointer to force
 a reference to zero --and what should be a crash.)
 4.) Any structures used by the API must be passed
 and returned by reference. (This allows the structure
 size to change to support more potential members
 while not causing undefined behavior as older API
 versions work with the structures).
 5.) Structure members may not be reordered.
 6.) New structure members must be added after the
 current last member.
 7.) The header for the library must require a
 compile-time define to specifiy the API version
 that will be requested so that only the members
 of structures that existed for a given version
 of the library will be included.
 8.) All unused API function references in the
 structure will be initialized to zero.
 9.) Only return primitive types from the DLL
 or types that are defined and controlled by the
 DLL.
 10.) Pair any memory allocation functions with
 memory deallocation functions in the DLL which
 are exposed to the consumer.
*/
#include "future_proof_library.h"

//Define DEBUG_MODE before including debug.h to allow
//for debugging output to be compiled into the program.
#define DEBUG_MODE
#include "debug.h"

#include <cstdint>
#include <cinttypes>
#include <iostream>
#include <algorithm>
//#include <cstdlib>
#include <cstring> //memset to initialize the API structure
#include <stdexcept> //std::runtime_error / std::exception

/*
 ...I think what I'll actually do is just add a destructor
 to the API structure which checks to see whether the
 uninitialization function reference member has a valid address.
 If so, it would call it. This would allow for the API "structure"
 to be scoped, and allow it to behave like a true class.
 Its constructor should simply assign its own methods, depending
 upon the version of the library that exists.


//The actual api class defined in the DLL could be different than the
//one used by the consumer. What implications could this have on the
//ordering of members and methods in memory and what implications would
//that have on instantiation, class state, and destruction?
class future_proof_api {
  private:
    //...
  public:
    future_proof_api (  );
    ~future_proof_api (  );

    do_something (  );
#if API_VERSION > 1
    void do_something_else ( void );
#endif
#if API_VERSION > 2
//...
#endif
};

//Always exists in the newer DLL.
void future_proof_api::do_something_else (  ) {
}

#define version_mismatch_error(...) \
std::cerr << __FUNCTION__ << \
  " is not defined in API version " << \
  API_VERSION << std::endl; \
exit ( 1 )

//.so / .dll(with .lib import library) -- or just a static lib
class future_proof_api_interface {
  private:
  public:
    future_proof_api_interface (  );
    virtual ~future_proof_api_interface (  );

    //V1 methods
    virtual void do_something ( void );
    //V2 methods
    virtual void do_something_else ( void );
    //...
    //V3 methods
};
void future_proof_api_interface::do_something ( void ) {
  version_mismatch_error();
}
void future_proof_api_interface::do_something_else ( void ) {
  version_mismatch_error();
}

class __declspec(dllexport) future_proof_api :
  public future_proof_api_interface {
  private:
  public:
    future_proof_api (  );
    virtual ~future_proof_api (  );
    virtual void do_something ( void ) override;
#if API_VER > 1
    virtual void do_something_else ( void ) override;
#endif
}
void future_proof_api::do_something ( void ) {
  //...
}
#if API_VER > 1
void future_poof_api::do_something_else ( void ) {
  //..
}
#endif
*/

//Global state
INTERNAL_FUTURE_PROOF_LIBRARY_STATE g_future_proof_library_state;


//Content for version 1 of this library.
namespace ns_version1 {

//Actual API functions that are doing the work, internally.
void do_something ( void ) {
  std::cout << "[" << __FILE__ << ":" <<
            ((unsigned int) __LINE__) << "] " <<
            "(ns_version1) This is only a test." << std::endl;
}

} //namespace ns_version1



//Content for versioon two of this library.
namespace ns_version2 {

//Actual API functions that are doing the work, internally.
void do_something ( void ) {
  std::cout << "[" << __FILE__ << ":" <<
            ((unsigned int) __LINE__) << "] " <<
            "(ns_version2) This is only a test." << std::endl;
}

//This is a new API function added in library version 2.
void do_something_else ( void ) {
  std::cout << "[" << __FILE__ << ":" <<
            ((unsigned int) __LINE__) << "] " <<
            "(ns_version2) This is a API function that was introduced in version 2 of the library." << std::endl;
}

} //namespace ns_version2




//Exposed API functions:
extern "C" {

tcbool uninitialize_future_proof_library ( void ) {
  //Free all resources/reset global state to its initial value.
  cpp20_debug_printf (
    *g_future_proof_library_state .cout,
    "Freeing resources and resetting to default state.\n"
  );

  //Delete the osstream_iterator objects used for stdout, stderr output.
  if ( g_future_proof_library_state .cout ) {
    delete g_future_proof_library_state .cout;
  }
  if ( g_future_proof_library_state .cerr ) {
    delete g_future_proof_library_state .cerr;
  }

  memset (
    &g_future_proof_library_state,
    0,
    sizeof ( INTERNAL_FUTURE_PROOF_LIBRARY_STATE )
  );

  return TCTRUE;
}


//Note: This function must be the last one defined in the
//list of exported C API functions so that all previous
//ones will be referencable by the body (or functions
//defined below this function will need a forward
//declaration).
//Note: This is the only function that we export for the API.
__declspec(dllexport) tcbool initialize_future_proof_library (
  _IN_ uint64_t ui64_desired_library_version,
  _IN_ size_t st_size_of_api_structure, //This may be different for the caller, if they're requesting an older version of the API.
  _OUT_ API *lp_api
) {
  //std::cout << "[" << __FILE__ << ":" << __LINE__ << "] " <<
  //  "Requested library version: " << ui64_desired_library_version << std::endl <<
  //  "API structure size:" << st_size_of_api_structure << std::endl << std::flush;

  //Track the requested version of the library for debugging.
  g_future_proof_library_state .ui64_requested_library_version = ui64_desired_library_version;
  try {
    g_future_proof_library_state .cout = new std::ostream_iterator<char> { std::cout };
    g_future_proof_library_state .cerr = new std::ostream_iterator<char> { std::cerr };
  } catch ( std::exception &e ) {
    std::cerr << "Error: Unable to create an osstream_iterator for cout/cerr to use std::vformat." << std::endl << std::flush;
    return TCFALSE;
  }

  //We have to zero all of the function pointers using the structure size that
  //we were provided, because the size of the structure that they have will be
  //smaller than the size of the structure that we have, if the caller is using
  //an older version of the API than what is current.
  memset ( lp_api, 0, st_size_of_api_structure );

  //All versions will use these initialization and uninitialization functions.
  lp_api ->initialize_future_proof_library = initialize_future_proof_library;
  lp_api ->uninitialize_future_proof_library = uninitialize_future_proof_library;

  switch ( ui64_desired_library_version ) {
    case 1: {
      lp_api ->do_something = ns_version1::do_something;
      break;
    }
    case 2: {
      lp_api ->do_something = ns_version2::do_something;
      lp_api ->do_something_else = ns_version2::do_something_else;
      break;
    }
    default: {
      return TCFALSE;
    }
  }

  //Treat the structure of function pointers as an array of arbitrary typeless pointers
  //to test what addresses they reference. If any of them reference zero, then we've
  //forgotten to assign that function reference.
  void **lpv_p = (void **) lp_api;
  void **lpv_sentinel = lpv_p + (st_size_of_api_structure / sizeof(void*));
  std::cout << "lpv_p: " << lpv_p << "; lpv_sentinel: " << lpv_sentinel << std::endl << std::flush;
  while ( lpv_p < lpv_sentinel ) {
    cpp20_debug_printf (
      *g_future_proof_library_state .cout,
      "Checking address: {}\n",
      *lpv_p
    );
    if ( ! *lpv_p ) {
      std::cerr <<
        "Internal library error. One or more API functions references were not set for requested API version " <<
        ui64_desired_library_version << "." << std::endl << std::flush;
      return TCFALSE;
    }
    lpv_p ++;
  }

  return TCTRUE;
}

} //extern "C"
