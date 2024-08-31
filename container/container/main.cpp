#include <iostream>
#include <format>
#include <print>

#include <chrono>

#include <vector>
#include <list>
#include <string>
#include <algorithm>


#include <cfloat>

typedef struct NODE {
  std::string str;
  double dbl = 15.5;
  int64_t i64_number = 7;
} NODE;

static inline void list_test ( void ) {
  std::chrono::system_clock::time_point tp_start = std::chrono::system_clock::now (  );
  std::list<NODE> lst;
  NODE node;

  for ( size_t st_i = 0; st_i < 1'000'000; st_i ++ ) {
    node .i64_number += 7;
    node .dbl -= (double) st_i /  DBL_EPSILON;
    //node .str = std::to_string ( st_i );
    lst .push_back ( node );
  }
  for ( size_t st_i = 0; st_i < 40'000; st_i ++ ) {

/*
    std::list<NODE>::iterator it = lst .begin (  );
    std::advance ( it, rand (  )%50'000 );
    lst .erase ( it );
*/
    lst .pop_back (  );
/*
    std::list<NODE>::iterator it = lst .begin (  );
    std::advance ( it, lst .size (  ) - 1 );
    lst .erase ( it );
*/
  }

  std::chrono::system_clock::time_point tp_end = std::chrono::system_clock::now (  );
  std::chrono::duration<double, std::milli> dur_elapsed_ms =
    tp_end - tp_start;

  std::print (
    "Elapsed milliseconds: {:f}\n",
    dur_elapsed_ms .count (  )
  );
}

static inline void vector_test ( void ) {
  std::chrono::high_resolution_clock::time_point tp_start = std::chrono::high_resolution_clock::now (  );
  std::vector<NODE> vec;
  vec .reserve ( 1'000'000 );
  NODE node;
  NODE *lp_previous_node = 0;

  //How many times did we have to move the entire block of memory to a new address
  //in order to make its elements contiguous.
  size_t st_vector_copies = 0;
  for ( size_t st_i = 0; st_i < 1'000'000; st_i ++ ) {
    node .i64_number += 7;
    node .dbl -= (double) st_i /  DBL_EPSILON;
    node .str = std::to_string ( st_i );
    vec .push_back ( node );
    if ( lp_previous_node ) {
      if ( lp_previous_node != &vec [ 0 ] ) {
        st_vector_copies ++;
      }
    }

    lp_previous_node = &vec [ 0 ];
  }

  for ( size_t st_i = 0; st_i < 40'000; st_i ++ ) {
    //vec .erase ( vec.begin() + rand()%50'000 );
    vec .pop_back (  );
    //vec .erase ( vec.begin() );
  }

  std::chrono::high_resolution_clock::time_point tp_end = std::chrono::high_resolution_clock::now (  );
  std::chrono::duration<double, std::milli> dur_elapsed_ms =
    tp_end - tp_start;

  std::print (
    "Elapsed milliseconds: {}\n",
    dur_elapsed_ms .count (  )
  );
  std::print (
    "The vector elements' base address changed {} times.\n",
    st_vector_copies
  );
}

int main ( void ) {

  //list_test (  );
  //vector_test (  );

  size_t st_nodes_to_traverse;
  size_t st_total_nodes_traversed = 0;
  for ( size_t st_i = 0; st_i < 40'000; st_i ++ ) {
    st_nodes_to_traverse = 1'000'000 - 40'000 - st_i - 1;
    st_total_nodes_traversed += st_nodes_to_traverse;
  }
  std::cout << st_total_nodes_traversed << " nodes were traversed.\n";
  double dbl_elapsed_minutes = 23.09893635;
  double dbl_nodes_traversed_per_minute = (double) 37'600'020'000 / dbl_elapsed_minutes;
  std::print (
    "{:f} nodes were travsed per minute.\n",
    dbl_nodes_traversed_per_minute
  );
  //1,627,781,445.443050 1.6 billion nodes per minute can be traversed on my i7 8700k CPU.


  return 0;
}