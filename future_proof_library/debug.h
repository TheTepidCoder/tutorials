/*
 Created on 2024-08-10 by TheTepidCoder
 Purpose: To allow for filterable formatted strings
 that can dump to arbitrary output handles. Typically,
 this would be stdout/stderr or std::cout/std:cerr,
 but these could also be file handles.
 Currently this uses std::vformat and std::print,
 if CPP23_AVAILABLE is defined. It could be expanded
 to leverage fprintf, if C++ version 20+ isn't available.
*/
#pragma once
#ifndef DEBUG_HEADER
#define DEBUG_HEADER

#include <format>

//If you have c++23's std::print feature in your compiler,
//uncomment this line of code to enable modern versions
//of the debug_printf macros.
//#define CPP23_AVAILABLE

/*
//This must be defined in order to use these filters.
const uint8_t gui8_debugging_output_filters =
  (1 << ns_debugging_output::debugging_output::file_handling) |
  (1 << ns_debugging_output::debugging_output::encryption);
*/

#ifdef DEBUG_MODE

//Enable this, if your compiler supports C++23's std::print from <print>
#  ifdef CPP23_AVAILABLE
#    define debug_printf(out, sz_format_string, ...) \
       std::print ( out, "[{}:{}] ", __FILE__, __LINE__ ); \
       std::print ( out, sz_format_string, __VA_ARGS__ ); \
       std::cout << std::flush

#    define filter_debug_printf(filter_type, out, sz_format_string, ...) \
       if constexpr ( (1 << filter_type) & gui8_debugging_output_filters ) { \
         std::print ( out, "[{}:{}] ", __FILE__, __LINE__ ); \
         std::print ( out, sz_format_string, __VA_ARGS__ ); \
         std::cout << std::flush; \
       }
#  endif //CPP23_AVAILABLE

#  define cpp20_debug_printf(out, sz_format_string, ...) \
     std::vformat_to ( out, "[{}:{}] ", std::make_format_args ( __FILE__, __LINE__ ) ); \
     std::vformat_to ( out, sz_format_string, std::make_format_args ( __VA_ARGS__ ) ); \
     std::cout << std::flush
#else
#  ifdef CPP23_AVAILABLE
#    define debug_printf(...)
#    define filter_debug_printf(...)
#  endif
#  define cpp20_debug_printf(...)
#endif

#endif
