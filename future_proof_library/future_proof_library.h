/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To show how to make a reasonably
 future-proof library that won't suffer from
 ABI-breaking changes.
*/
#pragma once
#ifndef FUTURE_PROOF_LIBRARY
#define FUTURE_PROOF_LIBRARY

#include <cstdint> //u?int#_t
#include <iostream> //std::cout / std::cin
#include <ostream> //std::ostream_iterator<t>
#include <iterator> //for std::osstream_iterator<T>
#include <format> //std::v?format(?:_to)?
#include <string> //used by std::v?format(?:_to)?
//#include <cinttypes>

#ifndef FUTURE_PROOF_LIBRARY_VERSION
#error "FUTURE_PROOF_LIBRARY_VERSION must be defined with an API version!"
#endif

#ifndef TCBOOL_DEFINED
#define TCBOOL_DEFINED
typedef uint8_t tcbool;
#endif

//Defines for true and false which guarantee compatibility with C,
//since C requires including the stdbool header and it's unclear
//exactly how C++'s bool might convert into an integer type.
//It could be the max integer value for the type of int used to
//store the bool for true, or simply one. False boolean values
//will undoubtedly be equal to zero.
#ifndef TCTRUE
#define TCTRUE ((uint8_t) 1)
#endif

#ifndef TCFALSE
#define TCFALSE ((uint8_t) 0)
#endif

//Preprocessor defines which help explain the intention of
//a pointer/reference parameter of a function.
#ifndef _IN_
#define _IN_
#endif

#ifndef _OUT_
#define _OUT_
#endif

#ifndef _IN_OUT_
#define _IN_OUT_
#endif

//Mutable global state variables
#ifndef INTERNAL_FUTURE_PROOF_LIBRARY_STATE_STRUCTURE
#define INTERNAL_FUTURE_PROOF_LIBRARY_STATE_STRUCTURE
typedef struct INTERNAL_FUTURE_PROOF_LIBRARY_STATE {
  //Global state variables
  uint64_t ui64_requested_library_version = 0;
  std::ostream_iterator<char> *cout = nullptr;
  std::ostream_iterator<char> *cerr = nullptr;
} INTERNAL_FUTURE_PROOF_LIBRARY_STATE;
#endif

#ifndef API_STRUCTURE
#define API_STRUCTURE
typedef struct API {
  tcbool (*initialize_future_proof_library)(
    _IN_ uint64_t ui64_desired_library_version,
    _IN_ size_t st_size_of_api_structure,
    _OUT_ API *lp_api
  );
  tcbool (*uninitialize_future_proof_library)(void);
//For all versions of the library.
  void (*do_something)(void);
//For library versions 2+
#if FUTURE_PROOF_LIBRARY_VERSION > 1
  void (*do_something_else)(void);
#endif

//For library versions 3+
#if FUTURE_PROOF_LIBRARY_VERSION > 2
#endif

} API;
#endif

//Define any consumer-facing structures, here.
#if FUTURE_PROOF_LIBRARY_VERSION == 1
//As an example, maybe the API takes some structure
//as an argument for one of its functions.
typedef struct SERVER_INFORMATION {
  //Note: On Windows w/ the Winsock API, this would be a SOCKET type instead of an int.
  int i_socket = 0; //Socket that we'll use to listen for new connections.
  const char *lpsz_server_name = nullptr; //Name of the server that we'll be hosting.
  uint32_t ui32_ipv4_address = 0; //The IP version 4 address (a.b.c.d) of this server.
  uint16_t ui16_local_port = 0; //listen on this port for remote connections
  //...
} SERVER_INFORMATION;
//For version two of the library, perhaps the structure definition needs to change.
//Here, we ensure that the correct version of the structure is being used by the
//consumer and that the version of the structure which we expect in the library is
//also correct, when building new versions of the library.
#elif FUTURE_PROOF_LIBRARY_VERSION == 2
typedef struct SERVER_INFORMATION {
  int i_socket = 0;
  const char *lpsz_server_name = nullptr;
  uint32_t ui32_ipv4_address = 0;
  uint16_t ui16_local_port = 0;

  //How many connections can be queued at once, before we start dropping them.
  uint32_t ui32_connection_queue_size = 0;
  //How many users can have established connections at one time?
  uint32_t ui32_max_active_connections = 0;
  //How long should the server wait before considering a connected user
  //inactive and terminating the connection to make room for other clients.
  uint32_t ui32_ping_timeout_in_milliseconds = 0;

  //Time to wait for reads before giving up.
  uint32_t ui32_socket_timeout_in_milliseconds = 0;

} SERVER_INFORMATION;
//Maybe this changed again in version 3 but hasn't changed again in the next
//15 versions, or something.
#elif FUTURE_PROOF_LIBRARY_VERSION >= 3
typedef struct SERVER_INFORMATION {
  //Version 1:
  int i_socket = 0;
  const char *lpsz_server_name = nullptr;
  uint32_t ui32_ipv4_address = 0;
  uint16_t ui16_local_port = 0;

  //Version 2:
  uint32_t ui32_connection_queue_size = 0;
  uint32_t ui32_max_active_connections = 0;
  uint32_t ui32_ping_timeout_in_milliseconds = 0;
  uint32_t ui32_socket_timeout_in_milliseconds = 0;

  //Version 3:
  //Added security features.
  uint16_t ui16_remote_port = 0; //The client must connect from a specific remote port, if this is non-zero.

  //Now we've added variables to track which IPv4 addresses/blocks should be
  //allowed to make (or blocked from making) connections.

  //A reference to uint32_t ui43ary_ipv4_addresses[];
  uint32_t *lpui32ary_ipv4_list = nullptr;
  uint32_t *lpui32ary_ipv4_list_mask = nullptr;
  //This is the number of elements in the array being referenced by
  //lpui32ary_ipv4_list.
  size_t st_ipv4_list_count = 0;

  //By default, we're allowing connections only from masked IPv4 addresses
  //that we specify to have extra security.
  tcbool b_allow_list = TCTRUE;
} SERVER_INFORMATION;
#endif

//Initialization function type definition so that
//when loading the library dynamically, one can
//do this:
//initialize_future_proof_library_t initialize_future_proof_library =
//(initialize_future_proof_library_t) GetProcAddress ( h_library, "initialize_future_proof_library_t" );
//instead of
//tcbool (*initialize_future_proof_library)(uint64_t, size_t, API*) =
//  (tcbool(*)(uint64_t, size_t, API*)) GetProcAddress ( h_library, "initialize_future_proof_library_t" );
//That should be a bit easier to read.
typedef tcbool (*initialize_future_proof_library_t) (
  _IN_ uint64_t,
  _IN_ size_t,
  _OUT_ API *
);

#endif
