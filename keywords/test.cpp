#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

//typedef struct NUMBER {
//  std::atomic<int> x;
//} NUMBER;

typedef struct NUMBER {
  int x;
  //void operator=(const NUMBER &rhs) { x = rhs.x; }
} NUMBER;

int main ( void ) {

  bool b_success = std::is_trivially_copyable<NUMBER>::value &&
  std::is_copy_constructible<NUMBER>::value &&
  std::is_move_constructible<NUMBER>::value &&
  std::is_copy_assignable<NUMBER>::value &&
  std::is_move_assignable<NUMBER>::value;
  std::cout << std::boolalpha << b_success << std::endl;

//  return 0;

  volatile std::atomic<NUMBER> n;
  //n .x = 0; //bad because we're accessing std::atomic<T>::x, which doesn't exist

  NUMBER n2 = n;
  n2 .x = 0;
  n = n2;

  n2 = n;
  std::cout << "Initial n value: " << n2.x << std::endl;

//  NUMBER n;

  std::atomic<int> x = 0; //mutex
  //std::atomic<bool> b; //spinlock
  //std::mutex mtx;
  auto fn_proc = [&n, &x] () -> void {
    for ( size_t st_i = 0; st_i < 100'000; st_i ++ ) {
      //mtx.lock();
      x ++;

      //n.x ++;

      NUMBER tmp = n;
      tmp .x ++;
  //    std::cout << tmp.x << std::endl;
      n = tmp;

      //mtx.unlock();
    }
  };
  std::vector<std::thread> vec_threads;
  for ( size_t st_i = 0; st_i < 10; st_i ++ ) {
    vec_threads .emplace_back (
      fn_proc
    );
  }
  for ( auto &th : vec_threads ) {
    th .join (  );
  }

  n2 = n;
  std::cout << "n.x: " << n2.x << std::endl;

  //std::cout << "n.x: " << n.x << std::endl;

  std::cout << "x: " << x << std::endl;

  return 0;
}
