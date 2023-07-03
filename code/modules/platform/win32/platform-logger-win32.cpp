module; 

#include <Windows.h>
#include <cstdio>
#include <cassert>

module platform:logger;

struct Win32StandardStream
{
    HANDLE handle;                          // Stream handle (STD_OUTPUT_HANDLE or STD_ERROR_HANDLE).
    bool   is_redirected;                   // True if redirected to file.
    [[maybe_unused]] bool is_wide;          // True if appending to a UTF-16 file.
    [[maybe_unused]] bool is_little_endian; // True if file is UTF-16 little endian.
};

#if 0 // From wincon.h, here for reference
#define FOREGROUND_BLUE            0x0001
#define FOREGROUND_GREEN           0x0002
#define FOREGROUND_RED             0x0004
#define FOREGROUND_INTENSITY       0x0008
#define BACKGROUND_BLUE            0x0010
#define BACKGROUND_GREEN           0x0020
#define BACKGROUND_RED             0x0040
#define BACKGROUND_INTENSITY       0x0080
#define COMMON_LVB_LEADING_BYTE    0x0100
#define COMMON_LVB_TRAILING_BYTE   0x0200
#define COMMON_LVB_GRID_HORIZONTAL 0x0400
#define COMMON_LVB_GRID_LVERTICAL  0x0800
#define COMMON_LVB_GRID_RVERTICAL  0x1000
#define COMMON_LVB_REVERSE_VIDEO   0x4000
#define COMMON_LVB_UNDERSCORE      0x8000
#endif

// Based on .NET ConsoleColor enumeration
// Foreground and Background colors are essentially the same bitfield, except the Background colors
// are left shifted 4 bits. Win32ConsoleColor can be composed by doing:
//     console_color = foreground | (background << 4)
enum class Win32ConsoleColor : int
{
    black        = 0,                                                   // color = black        | value = 0
    dark_blue    = FOREGROUND_BLUE,                                     // color = dark blue    | value = 1
    dark_green   = FOREGROUND_GREEN,                                    // color = dark green   | value = 2
    dark_cyan    = FOREGROUND_BLUE | FOREGROUND_GREEN,                  // color = dark cyan    | value = 3
    dark_red     = FOREGROUND_RED,                                      // color = dark red     | value = 3
    dark_magenta = FOREGROUND_BLUE | FOREGROUND_RED,                    // color = dark magenta | value = 5
    dark_yellow  = FOREGROUND_RED  | FOREGROUND_GREEN,                  // color = dark yellow  | value = 6
    grey         = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, // color = grey         | value = 7
    dark_grey    = FOREGROUND_INTENSITY,                                // color = dark grey    | value = 8
    blue         = FOREGROUND_INTENSITY | dark_blue,                    // color = blue         | value = 9
    green        = FOREGROUND_INTENSITY | dark_green,                   // color = green        | value = 10
    cyan         = FOREGROUND_INTENSITY | dark_cyan,                    // color = cyan         | value = 11
    red          = FOREGROUND_INTENSITY | dark_red,                     // color = red          | value = 12
    magenta      = FOREGROUND_INTENSITY | dark_magenta,                 // color = magenta      | value = 13
    yellow       = FOREGROUND_INTENSITY | dark_yellow,                  // color = yellow       | value = 14
    white        = FOREGROUND_INTENSITY | grey,                         // color = white        | value = 15
};

inline WORD win32_compose_console_color(Win32ConsoleColor foreground, Win32ConsoleColor background)
{
    return int(foreground) | (int(background) << 4);
}

inline WORD win32_get_console_color(HANDLE console_handle)
{
    WORD result = win32_compose_console_color(Win32ConsoleColor::white, Win32ConsoleColor::black);

    CONSOLE_SCREEN_BUFFER_INFO console_info{};
    BOOL info_result = GetConsoleScreenBufferInfo(console_handle, &console_info);
    if (info_result > 0)
    {
        result = console_info.wAttributes;
    }

    return result;
}

static Win32ConsoleColor log_color_to_win32_color(LogColor in_color)
{
    Win32ConsoleColor out_color = Win32ConsoleColor::black;
    switch (in_color)
    {
        case LogColor::black:        out_color = Win32ConsoleColor::black;        break; 
        case LogColor::dark_blue:    out_color = Win32ConsoleColor::dark_blue;    break; 
        case LogColor::dark_green:   out_color = Win32ConsoleColor::dark_green;   break; 
        case LogColor::dark_cyan:    out_color = Win32ConsoleColor::dark_cyan;    break; 
        case LogColor::dark_red:     out_color = Win32ConsoleColor::dark_red;     break; 
        case LogColor::dark_magenta: out_color = Win32ConsoleColor::dark_magenta; break; 
        case LogColor::dark_yellow:  out_color = Win32ConsoleColor::dark_yellow;  break; 
        case LogColor::grey:         out_color = Win32ConsoleColor::grey;         break; 
        case LogColor::dark_grey:    out_color = Win32ConsoleColor::dark_grey;    break; 
        case LogColor::blue:         out_color = Win32ConsoleColor::blue;         break; 
        case LogColor::green:        out_color = Win32ConsoleColor::green;        break; 
        case LogColor::cyan:         out_color = Win32ConsoleColor::cyan;         break; 
        case LogColor::red:          out_color = Win32ConsoleColor::red;          break; 
        case LogColor::magenta:      out_color = Win32ConsoleColor::magenta;      break; 
        case LogColor::yellow:       out_color = Win32ConsoleColor::yellow;       break; 
        case LogColor::white:        out_color = Win32ConsoleColor::white;        break; 
    }
    return out_color;
}

static bool win32_redirect_console_io()
{
    bool result = true;
    FILE* fp;
    
    // Redirect STDIN if the console has an input handle
    if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
            result = false;
    }
    else
    {
        setvbuf(stdin, NULL, _IONBF, 0);
    }
    
    // Redirect STDOUT if the console has an output handle
    if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
            result = false;
    }
    else
    {
        setvbuf(stdout, NULL, _IONBF, 0);
    }
    
    // Redirect STDERR if the console has an error handle
    if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
            result = false;
    }
    else
    {
        setvbuf(stderr, NULL, _IONBF, 0);
    }
    
    return result;
}

// Sets up a standard stream (stdout or stderr).
static Win32StandardStream win32_get_standard_stream(DWORD stream_type)
{
    Win32StandardStream result{};
    
    // If we don't have our own stream and can't find a parent console, allocate a new console.
    result.handle = GetStdHandle(stream_type);
    if (!result.handle || result.handle == INVALID_HANDLE_VALUE)
    {
        if (!AttachConsole(ATTACH_PARENT_PROCESS))
        {
            AllocConsole();
            bool redirect_result = win32_redirect_console_io();
            assert(redirect_result);
        }

        result.handle = GetStdHandle(stream_type);
        assert(result.handle != INVALID_HANDLE_VALUE);
    }
    
    // Check if the stream is redirected to a file. If it does, check if the file already exists.
    if (result.handle != INVALID_HANDLE_VALUE)
    {
        DWORD dummy;
        DWORD type = GetFileType(result.handle) & (~FILE_TYPE_REMOTE);
        result.is_redirected = (type == FILE_TYPE_CHAR) ? !GetConsoleMode(result.handle, &dummy) : true;
        if (type == FILE_TYPE_DISK)
        {
            LARGE_INTEGER file_size;
            GetFileSizeEx(result.handle, &file_size);
            if (file_size.QuadPart > 1)
            {
                LARGE_INTEGER large = {0};
                u16 bom = 0;
                SetFilePointerEx(result.handle, large, 0, FILE_BEGIN);
                ReadFile(result.handle, &bom, 2, &dummy, 0);
                SetFilePointerEx(result.handle, large, 0, FILE_END);
                result.is_wide = (bom == (u16)0xfeff || bom == (u16)0xfffe);
                result.is_little_endian = (bom == (u16)0xfffe);
            }
        }
    }

    return result;
}

// Prints a message to a platform stream. If the stream is a console, uses supplied colors.
static void win32_print_to_stream(const char* message, s64 message_length, Win32StandardStream stream, WORD text_color)
{
    // If redirected, write to a file instead of console.
    DWORD dummy;
    if (stream.is_redirected)
    {
        WriteFile(stream.handle, message, (DWORD)message_length, &dummy, 0);
    }
    else
    {
        WORD previous_color = win32_get_console_color(stream.handle);

        SetConsoleTextAttribute(stream.handle, text_color);
        WriteConsole(stream.handle, message, (DWORD)message_length, &dummy, 0);
        
        // Restore console colors
        SetConsoleTextAttribute(stream.handle, previous_color);
    }
}

void log_internal(const char* message, s64 message_length, LogLevel level, LogColor foreground, LogColor background)
{
    Win32ConsoleColor win32_foreground = log_color_to_win32_color(foreground);
    Win32ConsoleColor win32_background = log_color_to_win32_color(background);
    WORD win32_console_color = win32_compose_console_color(win32_foreground, win32_background);

    if (level < LogLevel::error)
    { // log to stdout
        static Win32StandardStream stream = win32_get_standard_stream(STD_OUTPUT_HANDLE);
        win32_print_to_stream(message, message_length, stream, win32_console_color);
    }
    else
    { // log to stderr
        static Win32StandardStream stream = win32_get_standard_stream(STD_ERROR_HANDLE);
        win32_print_to_stream(message, message_length, stream, win32_console_color);
    }

    // NOTE(enlynn): Should this be either or? When debugging, will this just print the message twice?
#if DEBUG
    if (IsDebuggerPresent())
    {
        OutputDebugStringA(message);
    }
#endif
}