// optional #include or import statements. These only apply to this file
module;

#include <Windows.h>
#include <windowsx.h>

module platform:window;

import :logger;

static bool is_running = true; // TODO: This should be apart of user-data. 

static void win32_register_window_class(const char* window_class_name, WNDPROC window_proc)
{
    WNDCLASSEX window_class{};
    ZeroMemory(&window_class, sizeof(WNDCLASSEX));
    window_class.cbSize        = sizeof(window_class);
    window_class.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    window_class.lpfnWndProc   = window_proc;
    window_class.hInstance     = GetModuleHandle(0);
    window_class.hCursor       = (HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    window_class.hIcon         = (HICON)LoadImage(0, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    window_class.lpszClassName = window_class_name;
    RegisterClassEx(&window_class);
}

static HWND win32_create_window(const char* client_name, const char* window_class, const s32 requested_width, const s32 requested_height)
{
    // Get the current screen size, so we can center our window.
    int screen_width  = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    
    RECT rect = { 0, 0, requested_width, requested_height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    
    LONG width  = rect.right  - rect.left;
    LONG height = rect.bottom - rect.top;
    
    LONG window_x = (screen_width  - width)  / 2;
    LONG window_y = (screen_height - height) / 2;
    
    if (window_x < 0) window_x = CW_USEDEFAULT;
    if (window_y < 0) window_y = CW_USEDEFAULT;
    if (width < 0)    width    = CW_USEDEFAULT;
    if (height < 0)   height   = CW_USEDEFAULT;
    
    DWORD style    = WS_OVERLAPPEDWINDOW;
    DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;
    
    HWND result = CreateWindowEx(ex_style,
                                 window_class,
                                 client_name,
                                 style,
                                 window_x,
                                 window_y,
                                 width,
                                 height,
                                 0,
                                 0,
                                 GetModuleHandle(0),
                                 NULL);
    
    return result;
}

LRESULT CALLBACK
win32_window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
        case WM_CLOSE:   // TODO(enlynn): Close popup instead of immediate close
        {
            Event event{};
            // !!! event_fire(EVENT_CODE_APPLICATION_CLOSE, NULL, Event);
            is_running = false;
        } break;
        
        case WM_DESTROY: // TODO(enlynn): Reset program state?
        {
            Event event{};
            // !!! event_fire(EVENT_CODE_APPLICATION_DESTROY, NULL, Event);
            is_running = false;
        } break;

        case WM_QUIT:
        {
            is_running = false;
        } break;
        
        case WM_SIZE: {
            // Get the updated size.
            RECT r;
            GetClientRect(hwnd, &r);
            u32 width = r.right - r.left;
            u32 height = r.bottom - r.top;
            
            // Fire the event. The application layer should pick this up, but not handle it
            // as it shouldn be visible to other parts of the application.
            Event event{};
            event.window_size[0] = (u16)width;
            event.window_size[1] = (u16)height;
            // !!! event_fire(EVENT_CODE_RESIZED, 0, Event);
        } break;
        
        case WM_PAINT:
        {
            RECT WindowRect;
            GetClientRect(hwnd, &WindowRect);
            ValidateRect(hwnd, &WindowRect);
            return 0;
        } break;
    }
    
    return DefWindowProc(hwnd, message, wparam, lparam);
}

PlatformWindow::PlatformWindow(const char* client_window_name, const s32 window_width, const s32 window_height)
{
    win32_register_window_class(c_wnd_class, win32_window_proc);
    m_handle = win32_create_window(client_window_name, c_wnd_class, window_width, window_height);

    if (m_handle)
    {
        ShowWindow(static_cast<HWND>(m_handle), SW_SHOWNORMAL);
        m_is_running = true;
    }
    else
    {
        // LOG !!!
        m_is_running = false;
    }
}

PlatformWindow::~PlatformWindow()
{
    m_handle = nullptr;
}

bool PlatformWindow::pump_event_messages(
        std::optional<std::reference_wrapper<InputSystem>> input_system, 
        std::optional<std::reference_wrapper<EventSystem>> event_system)
{
    if (!is_running)
    {
        m_is_running = false;
        return m_is_running;
    }

    MSG msg{};
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        switch (msg.message)
        {
            case WM_CLOSE:   // TODO(enlynn): Close popup instead of immediate close
            {
                if (event_system)
                {
                    Event event{};
                    event_system.value().get().fire_event(u16(EventCode::app_close), event);    
                }
                m_is_running = false;
                return m_is_running;
            } break;
            
            case WM_DESTROY: // TODO(enlynn): Reset program state?
            {
                if (event_system)
                {
                    Event event{};
                    event_system.value().get().fire_event(u16(EventCode::app_destroy), event);    
                }
                m_is_running = false;
                return m_is_running;
            } break;
            
            case WM_QUIT:
            {
                if (event_system)
                {
                    Event event{};
                    event_system.value().get().fire_event(u16(EventCode::app_quit), event);
                }
                m_is_running = false;
                return m_is_running;
            } break;

            case WM_SIZE: {
                // Get the updated size.
                RECT r;
                GetClientRect(msg.hwnd, &r);
                u32 width  = r.right  - r.left;
                u32 height = r.bottom - r.top;
                
                if (event_system)
                {
                    Event event{};
                    event.window_size[0] = (u16)width;
                    event.window_size[1] = (u16)height;
                    event_system.value().get().fire_event(u16(EventCode::window_resized), event);
                }
            } break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                InputKey key  = (InputKey)msg.wParam;
                bool is_pressed  = (msg.message == WM_KEYDOWN || msg.message == WM_SYSKEYDOWN);
                bool is_extended = (HIWORD(msg.lParam) & KF_EXTENDED) == KF_EXTENDED;
                
                // Keypress only determines if _any_ alt/ctrl/shift key is pressed. Determine which one if so.
                if (msg.wParam == VK_MENU) 
                {
                    key = is_extended ? InputKey::ralt : InputKey::lalt;
                } 
                else if (msg.wParam == VK_SHIFT)
                {
                    // Annoyingly, KF_EXTENDED is not set for shift keys.
                    u32 lshift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                    u32 scancode = ((msg.lParam & (0xFF << 16)) >> 16);
                    key = (scancode == lshift) ? InputKey::lshift : InputKey::rshift;
                } 
                else if (msg.wParam == VK_CONTROL)
                {
                    key = is_extended ? InputKey::rcontrol : InputKey::lcontrol;
                }
                
                if (input_system)
                {
                    bool processed = input_system.value().get().process_key(key, is_pressed);
                    if (processed && event_system)
                    {
                        u16 event_code = is_pressed ? u16(EventCode::key_pressed) : u16(EventCode::key_released);
                        
                        Event event{};
                        event.key = key;

                        event_system.value().get().fire_event(event_code, event);
                    }
                }
            } break;

            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: 
            {
                bool pressed = 
                    msg.message == WM_LBUTTONDOWN || 
                    msg.message == WM_RBUTTONDOWN || 
                    msg.message == WM_MBUTTONDOWN;

                MouseButton button = MouseButton::max;
                switch (msg.message) 
                {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                    {
                        button = MouseButton::left;
                    } break;
                    
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                    {
                        button = MouseButton::middle;
                    } break;
                    
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                    {
                        button = MouseButton::right;
                    } break;
                }
                
                // Pass over to the input subsystem.
                if (button != MouseButton::max) 
                {
                    if (input_system)
                    {
                        bool processed = input_system.value().get().process_mouse_button(button, pressed);
                        if (processed && event_system)
                        {
                            u16 event_code = pressed ? u16(EventCode::button_pressed) : u16(EventCode::button_released);
                        
                            Event event{};
                            event.button = s16(button);

                            event_system.value().get().fire_event(event_code, event);
                        }
                    }
                }
            } break;

            case WM_MOUSEMOVE: {
                s32 screen_x = GET_X_LPARAM(msg.lParam);
                s32 screen_y = GET_Y_LPARAM(msg.lParam);
                
                if (input_system)
                {
                    bool processed = input_system.value().get().process_mouse_move(screen_x, screen_y);
                    if (processed && event_system)
                    {
                        u16 event_code = u16(EventCode::mouse_moved);
                        
                        Event event{};
                        event.mouse_position[0] = screen_x;
                        event.mouse_position[1] = screen_y;

                        event_system.value().get().fire_event(event_code, event);
                    }
                }
            } break;

            case WM_MOUSEWHEEL: {
                s32 mouse_delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
                if (mouse_delta != 0) {
                    // Flatten the input to an OS-independent (-1, 1)
                    mouse_delta = (mouse_delta < 0) ? -1 : 1;
                    if (event_system)
                    {
                        u16 event_code = u16(EventCode::mouse_wheel);
                        
                        Event event{};
                        event.mouse_delta = mouse_delta;

                        event_system.value().get().fire_event(event_code, event);
                    }
                }
            } break;

            default:
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            };
        }
    }

    return is_running;
}
