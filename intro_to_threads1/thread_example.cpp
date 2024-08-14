#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

int main () {
  uint64_t ui64_number = 0;
  std::vector<std::thread> v_threads;
  std::mutex mtx;

  //auto fn_thread_function = [&mtx] ( uint64_t &ui64_number ) -> void {
  auto fn_thread_function = [&ui64_number, &mtx] ( void ) -> void {
    for ( size_t st_i = 0; st_i < 100'000; st_i ++ ) {
      //mtx .lock (  );
      std::lock_guard lg ( mtx );
      //std::unique_lock ul ( mtx ); //constructor calls mtx.lock() --> internally it has std::mutex &

      ui64_number += 2;
      //mtx .unlock (  );
    }
  };

  for (
    size_t st_thread_index = 0;
    st_thread_index < 10;
    st_thread_index ++
  ) {
    v_threads .emplace_back (
      fn_thread_function //,
      //std::ref ( ui64_number ) //T&
      //std::reference_wrapper<uint64_t> ( ui64_number ) //uint64_t &
    );
  }

  //std::vector<std::thread> for each of those (std::thread) elements
  for ( auto &th : v_threads ) {
    th .join (  );
  }
  std::cout << (unsigned int) ui64_number << std::endl;

  return 0;
}
