/*
 Created on 2024-08-12 by TheTepidCoder
 Purpose: To show a practical example of how method pointers can
 be useful for C++ applications.
*/
#include <windows.h>

#include <string>
#include <format>

#ifndef elements_of
#define elements_of(n) (sizeof(n)/sizeof(n[0]))
#endif

HINSTANCE ghInst;
class window_manager {
  private:
    HWND m_h_window = nullptr;
    HDC m_hdc;
    PAINTSTRUCT m_ps;
    RECT m_client_rect = { 0 };
    size_t m_st_phase_index = 0; //determines which method to choose
    void (window_manager::*m_fnary_phases[4])(
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam
    ) = {
      &window_manager::phase1,
      &window_manager::phase2,
      &window_manager::phase3,
      &window_manager::phase4
    };
    void (window_manager::*m_fnary_phase_initializers[4])(void) = {
      &window_manager::start_phase1,
      &window_manager::start_phase2,
      &window_manager::start_phase3,
      &window_manager::start_phase4
    };

    //Shared setup phase state
    std::wstring m_wstr_username;
    std::wstring m_wstr_password;
    std::wstring m_wstr_server_address;

    //Setup phase elements.
    //Shared elements:
    HWND m_h_accept = nullptr;

    //phase1
    HWND m_h_username = nullptr; //edit window
    HWND m_h_password = nullptr; //edit window

    //phase2
    HWND m_h_server_address = nullptr; //edit window

  public:
    window_manager (  ); //constructor
    window_manager ( const window_manager &) = delete; //copy constructor
    window_manager &operator= ( const window_manager & ) = delete; //copy assignment operator overload
    window_manager ( window_manager && ) = delete; //move constructor
    window_manager &operator= ( window_manager && ) = delete; //move assignment operator overload
    ~window_manager (  ); //destructor

    //This was just a test method that is no longer used.
    void render ( void ) noexcept;

    //Set the current phase, hide all elements, and call
    //the appropriate start_phase# method.
    void set_phase ( size_t st_phase_index );

    //Call this between each phase transition so that
    //only the elements specific to a phase can be
    //unhidden by the corresponding start_phase method.
    void hide_everything ( void ) const noexcept;

    //These start_phase# methods will unhide the elements
    //specific to the corresponding phase.
    void start_phase1 ( void ) noexcept;
    void start_phase2 ( void ) noexcept;
    void start_phase3 ( void ) noexcept;
    void start_phase4 ( void ) noexcept;

    //Handle window messages uniquely for each phase.
    void phase1 ( UINT uMsg, WPARAM wParam, LPARAM lParam );
    void phase2 ( UINT uMsg, WPARAM wParam, LPARAM lParam );
    void phase3 ( UINT uMsg, WPARAM wParam, LPARAM lParam );
    void phase4 ( UINT uMsg, WPARAM wParam, LPARAM lParam );

    //Assign the window that we control.
    void initialize ( HWND h_window ) noexcept;

    //Adjust the sizes and locations of all of the UI components in our window.
    void adjust_common_elements ( void ) const noexcept;

    //Handle the window events (and delegate to phase specific methods).
    void update ( UINT uMsg, WPARAM wParam, LPARAM lParam );

    //Create the parent window and listen for its window messages.
    //Note: This will tie up the main thread.
    //To do other work in the background, this would need to be launched
    //within a thread (e.g. std::thread()).
    int create_window ( void );

    //Common window procedure which forwards messages to
    //phase specific window procedures. Must be static because
    //it needs to be a standard C function (not a method or
    //C++ lambda).
    static LRESULT CALLBACK common_window_procedure (
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam
    );
};

void window_manager::set_phase ( size_t st_phase_index ) {
  m_st_phase_index = st_phase_index;
  hide_everything (  );
  (this ->*m_fnary_phase_initializers[st_phase_index]) (  );
  InvalidateRect ( m_h_window, &m_client_rect, true );
  UpdateWindow ( m_h_window );
}

void window_manager::start_phase1 ( void ) noexcept {
  ShowWindow ( m_h_username, SW_SHOW );
  ShowWindow ( m_h_password, SW_SHOW );
}
void window_manager::start_phase2 ( void ) noexcept {
  ShowWindow ( m_h_server_address, SW_SHOW );
}
void window_manager::start_phase3 ( void ) noexcept {
}
void window_manager::start_phase4 ( void ) noexcept {
}
//Between each of the phases, we should hide all elements.
//The setup phase initializer methods can unhide/show what
//they require after this hide_everything method is called.
void window_manager::hide_everything ( void ) const noexcept {
  ShowWindow ( m_h_username, SW_HIDE );
  ShowWindow ( m_h_password, SW_HIDE );
  ShowWindow ( m_h_server_address, SW_HIDE );
}

void window_manager::phase1 ( UINT uMsg, WPARAM wParam, LPARAM lParam ) {

  if ( uMsg == WM_PAINT ) {
    m_hdc = BeginPaint ( m_h_window, &m_ps );

    SetBkMode ( m_hdc, TRANSPARENT );
    SetBkColor ( m_hdc, RGB ( 0, 0, 0 ) );
    SetTextColor ( m_hdc, RGB ( 255, 0, 255 ) );
    TextOutA ( m_hdc, 0, 0, "Username:", 9 );
    TextOutA ( m_hdc, 0, 50, "Password:", 9 );

    EndPaint ( m_h_window, &m_ps );
  }

  else if ( uMsg == WM_WINDOWPOSCHANGED ) {
    MoveWindow (
      m_h_username,
      5, 20, m_client_rect .right - 5 - 5, 20,
      true
    );
    MoveWindow (
      m_h_password,
      5, 80, m_client_rect .right - 5 - 5, 20,
      true
    );
  }

  else if ( uMsg == WM_COMMAND ) {
    if ( (HWND) lParam == m_h_accept ) {
      wchar_t wsz_username [ 256 ] = {  };
      wchar_t wsz_password [ 256 ] = {  };
      GetWindowTextW (
        m_h_username,
        wsz_username,
        elements_of ( wsz_username )
      );
      GetWindowTextW (
        m_h_password,
        wsz_password,
        elements_of ( wsz_password )
      );
      m_wstr_username = wsz_username;
      m_wstr_password = wsz_password;

      MessageBoxW (
        NULL,
        std::vformat (
          L"Username: \"{}\"\r\nPassword: \"{}\"",
          std::make_wformat_args (
            wsz_username,
            wsz_password
          )
        ) .c_str (  ),
        L"User/Pass:",
        MB_OK
      );
    }
  }
}

void window_manager::phase2 ( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  if ( uMsg == WM_PAINT ) {
    m_hdc = BeginPaint ( m_h_window, &m_ps );

    SetBkMode ( m_hdc, TRANSPARENT );
    SetBkColor ( m_hdc, RGB ( 0, 0, 0 ) );
    SetTextColor ( m_hdc, RGB ( 255, 0, 255 ) );

    TextOutA ( m_hdc, 0, 0, "Server address:", 15 );
    EndPaint ( m_h_window, &m_ps );
  }
  else if ( uMsg == WM_COMMAND ) {
    if ( (HWND) lParam == m_h_accept ) {
      wchar_t wsz_server_address [ 16 ] = { 0 }; //^(\d{1,3}\.){3}\.\d{1,3}$
      GetWindowTextW (
        m_h_server_address,
        wsz_server_address,
        elements_of ( wsz_server_address )
      );
      m_wstr_server_address = wsz_server_address;

      MessageBoxW ( NULL, wsz_server_address, L"Server address:", MB_OK );
    }
  }
  else if ( uMsg == WM_WINDOWPOSCHANGED ) {
    MoveWindow (
      m_h_server_address,
      5, 20, m_client_rect .right - 5 - 5, 20,
      true
    );
  }
}
void window_manager::phase3 ( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
}
void window_manager::phase4 ( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
}
void window_manager::initialize ( HWND h_window ) noexcept {
  m_h_window = h_window;

  m_h_accept = CreateWindowExW (
    0,
    L"button",
    L"Accept",
    BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
    5,
    m_client_rect .bottom - 25,
    m_client_rect .right - 5 - 5,
    20,
    m_h_window,
    nullptr,
    ghInst,
    0
  );
  m_h_username = CreateWindowExW (
    0,
    L"edit",
    L"",
    WS_CHILD | ES_LEFT,
    5, 20, m_client_rect .right - 5 - 5, 20,
    m_h_window,
    nullptr,
    ghInst,
    0
  );
  m_h_password = CreateWindowExW (
    0,
    L"edit",
    L"",
    WS_CHILD | ES_LEFT | ES_PASSWORD,
    5, 50, m_client_rect .right - 5 - 5, 20,
    m_h_window,
    nullptr,
    ghInst,
    0
  );

  m_h_server_address = CreateWindowExW (
    0,
    L"edit",
    L"",
    WS_CHILD | ES_LEFT,
    5, 20, m_client_rect .right - 5 - 5, 20,
    m_h_window,
    nullptr,
    ghInst,
    0
  );

  //The phases indices are 0-based in the method pointer array.
  set_phase ( 0 );
}

//Move all of the window components for each of the phases to be
//proportional to the dimensions of the main window.
void window_manager::adjust_common_elements ( void ) const noexcept {
  MoveWindow (
    m_h_accept,
    5,
    m_client_rect .bottom - 25,
    m_client_rect .right - 5 - 5,
    20,
    true
  );
}
window_manager::window_manager (  ) {
}
window_manager::~window_manager (  ) {
}
void window_manager::update (
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  GetClientRect ( m_h_window, &m_client_rect );


  switch ( m_st_phase_index ) {
    case 0:
      phase1 ( uMsg, wParam, lParam );
    break;
    case 1:
      phase2 ( uMsg, wParam, lParam );
    break;
    case 2:
      phase3 ( uMsg, wParam, lParam );
    break;
    case 3:
      phase4 ( uMsg, wParam, lParam );
    break;
  }


/*
  //Call the window procedure (callback function for WINAPI messages)
  //that relates to the current setup phase.
  (this ->*m_fnary_phases [ m_st_phase_index ]) (
    uMsg,
    wParam,
    lParam
  );
*/

  if ( uMsg == WM_WINDOWPOSCHANGED ) {
    adjust_common_elements (  );
  }

  else if ( uMsg == WM_KEYDOWN ) {
    //0x30 = '0' VK_0
    //0x31 = '1' VK_1
    //...
    size_t st_phase_index = wParam - 0x30;
    if ( st_phase_index >= elements_of ( m_fnary_phases ) ) {
      return ;
    }

    set_phase ( wParam - 0x30 );
  }
  else if ( uMsg == WM_LBUTTONDOWN ) {
    SetFocus ( m_h_window );
  }

}
void window_manager::render ( void ) noexcept {
  m_hdc = BeginPaint ( m_h_window, &m_ps );

  //DrawTextA ( m_hdc, "Testing", -1, &m_client_rect, DT_LEFT | DT_TOP );
  SetBkMode ( m_hdc, TRANSPARENT );
  SetBkColor ( m_hdc, RGB ( 0, 0, 0 ) );
  SetTextColor ( m_hdc, RGB ( 255, 0, 255 ) );
  TextOutA ( m_hdc, 0, 0, "Testing", 7 );

  EndPaint ( m_h_window, &m_ps );
}

//static class method
LRESULT CALLBACK window_manager::common_window_procedure (
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  static window_manager *wm = nullptr;
  //wm = (window_manager *) GetWindowLongPtrW ( hWnd, GWLP_USERDATA );
  if ( wm ) {
    wm ->update ( uMsg, wParam, lParam );
  }

  switch ( uMsg ) {

    case WM_NCCREATE: {
      wm = (window_manager *) ((CREATESTRUCTW *) lParam) ->lpCreateParams;
      //wm = (window_manager *) GetWindowLongPtrW ( hWnd, GWLP_USERDATA );
      break; //Just return DefWindowProcW, here so that the window title and things can be set.
    }

    case WM_CREATE: {
      if ( wm ) {
        //MessageBoxA ( NULL, "We're able to create the elements because the window_manager class instance is valid in WM_CREATE.", "Note:", MB_OK );
        wm ->initialize ( hWnd );
      }

      return 0;
    }
/*
    case WM_PAINT: {
      if ( wm ) {
        wm ->render (  );
      }
      return 0;
    }
*/
    case WM_CLOSE: {
      DestroyWindow ( hWnd );
      return 0;
    }
    case WM_DESTROY: {
      //Free any resources.
      PostQuitMessage ( 0 );
      return 0;
    }
  }
  return DefWindowProcW ( hWnd, uMsg, wParam, lParam );
}

int window_manager::create_window ( void ) {
  HWND h_window;
  MSG msg;
  WNDCLASSEXW wc = { 0 };
  wc .cbSize = sizeof ( WNDCLASSEXW );
  wc .lpszClassName = L"method_pointer_main_window_class";
  wc .hInstance = ghInst;
  wc .hbrBackground = (HBRUSH) GetStockObject ( BLACK_BRUSH );
  wc .hCursor = LoadCursorW ( NULL, IDC_ARROW );
  wc .hIcon = LoadIconW ( NULL, IDI_APPLICATION );
  wc .hIconSm = wc .hIcon;
  wc .lpfnWndProc = window_manager::common_window_procedure;
  wc .lpszMenuName = nullptr;
  wc .style = CS_HREDRAW | CS_VREDRAW;
  RegisterClassExW ( &wc );

  h_window = CreateWindowExW (
    0,
    L"method_pointer_main_window_class",
    L"Method pointer example",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 400, 400,
    nullptr,
    nullptr,
    ghInst,
    (LPVOID) this
  );

  if ( ! h_window ) {
    MessageBoxA ( NULL, "We couldn't create the main window.", "Error:", MB_ICONEXCLAMATION );
    return false;
  }

  //Alternative to passing a CREATESTRUCT argument as the final argument of CreateWindowExA/W
  //SetWindowLongPtrW ( h_window, GWLP_USERDATA, (LONG_PTR) this );
  while ( GetMessageW ( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage ( &msg );
    DispatchMessageW ( &msg );
  }
  return static_cast<int> ( msg .wParam );
}

int WINAPI WinMain (
  HINSTANCE hInst,
  HINSTANCE hPrevInst,
  PSTR szCmdLine,
  int iCmdLine
) {
  ghInst = hInst;

  window_manager wm;
  return wm .create_window (  );
}