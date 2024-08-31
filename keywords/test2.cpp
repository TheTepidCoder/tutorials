/*
std::atomic<T> (which should work for all trivially copiable types)
I suggest using it only for integral and floating-point types.
(I'm including types like these: char wchar_t char32_t.)
volatile
*/
int some_function (
  int *iary_buffer,
  size_t st_element_count
) {
  volatile int x = some_function (  );

  //as-if
  //[ 0, 1, 2, ..., n-1 ]
  int a = *iary_buffer; //iary_buffer[0]
  int b = iary_buffer [ 1 ];
  a += x;
  b -= x;
  *iary_buffer = a;
  iary_buffer [ 1 ] = b;
}

int get_num ( int a, bool b_add ) {
  volatile int x = a + 2;
  if ( b_add ) {
    x += 2;
  }
  else {
    x -= 2;
  }
  return x;
}

int get_num ( int a, bool b_add ) {
  return a + (int) b_add * 4;
}
