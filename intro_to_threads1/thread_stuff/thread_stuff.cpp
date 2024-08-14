#include <windows.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <format>

HINSTANCE g_hinst;

void add_two ( uint64_t &ui64_value ) {
  ui64_value += 2;
}

LRESULT CALLBACK thread_stuff_main_window_procedure (
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  static HWND h_button = nullptr;
  static HDC hdc;
  static PAINTSTRUCT ps;

  switch ( uMsg ) {
    case WM_CREATE: {
      //initialize stuff.
      h_button = CreateWindowExW (
        0,
        L"button",
        L"Click me",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        0, //x
        20, //y
        150, //width
        20, //height
        hWnd,
        NULL,
        g_hinst,
        0
      );
      return 0;
    }
    case WM_PAINT: {
      hdc = BeginPaint ( hWnd, &ps );
      TextOutW ( hdc, 0, 0, L"Testing", 7 );
      EndPaint ( hWnd, &ps );
      return 0;
    }
    case WM_COMMAND: {
      if ( (HWND) lParam == h_button ) {
        //MessageBoxW ( NULL, L"You pressed the button.", L"Note:", MB_ICONEXCLAMATION );
        //Sleep ( 1000 );
        std::mutex mtx;
        uint64_t ui64_number = 0;
        std::vector<std::thread> v_threads;
        for (
          size_t st_thread_index = 0;
          st_thread_index < 10;
          st_thread_index ++
        ) {
          v_threads .emplace_back (
            std::thread (
              [&mtx, &ui64_number]() -> void {
                for (
                  size_t st_i = 0;
                  st_i < 100'000;
                  st_i ++
                ) {
                  std::lock_guard lg ( mtx );
//if ( mtx .try_lock() ) { ..., mtx .unlock(); }
//mtx.lock();
//mtx.unlock();
                  //std::unique_lock ul ( mtx );
                  add_two ( ui64_number );
                }
                //std::this_thread::sleep_for (
                //  std::chrono::milliseconds ( 1000 )
                //);
              }
            )
          );
        }
        for ( auto &th : v_threads ) {
          th .join (  );
        }
        std::string str_results = std::vformat (
          "The sum is {}",
          std::make_format_args ( ui64_number )
        );
        MessageBoxA ( NULL, str_results .c_str (  ), "Results:", MB_OK );
      }
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage ( 0 );
      return 0;
    }
    case WM_CLOSE: {
      DestroyWindow ( hWnd );
    }
  }
  return DefWindowProcW ( hWnd, uMsg, wParam, lParam );
}

int WINAPI WinMain (
  HINSTANCE hInst,
  HINSTANCE hPrevInst,
  PSTR szCmdLine,
  int iCmdShow
) {
  g_hinst = hInst;
  HWND h_window = 0;
  MSG msg = { 0 };
  WNDCLASSEXW wc = { 0 };
  wc .cbSize = sizeof ( WNDCLASSEX );
  wc .style = CS_HREDRAW | CS_VREDRAW;
  wc .lpfnWndProc = thread_stuff_main_window_procedure;
  //wc .cbClsExtra;
  //wc .cbWndExtra;
  wc .hInstance = hInst;
  wc .hIcon = LoadIconW ( hInst, IDI_APPLICATION );
  wc .hIconSm = wc .hIcon;
  wc .hCursor = (HCURSOR) LoadCursorW ( hInst, IDC_ARROW );
  wc .hbrBackground = (HBRUSH) GetStockObject ( BLACK_BRUSH );
  wc .lpszMenuName = nullptr;
  wc .lpszClassName = L"thread_stuff_main_window_class"; //(const wchar_t *)

  RegisterClassExW ( &wc );

  h_window = CreateWindowExW (
    WS_EX_TOPMOST,
    L"thread_stuff_main_window_class",
    L"Thread test window",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 400, 400,
    NULL,
    NULL,
    hInst,
    0
  );
  while ( GetMessageW ( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage ( &msg );
    DispatchMessageW ( &msg );
  }

  return static_cast<int> ( msg .wParam );
}