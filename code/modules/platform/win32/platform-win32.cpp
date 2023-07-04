module;

#include <Windows.h>
#include <timeapi.h>

module platform;

import <cassert>;

void platform_initialize()
{
    UINT desired_scheduler_ms = 1;
	bool sleep_is_granular = (timeBeginPeriod(desired_scheduler_ms) == TIMERR_NOERROR);
	assert(sleep_is_granular);
}

void platform_shutdown()
{
}

void platform_sleep(u32 time_in_ms)
{
    Sleep(time_in_ms);
}

LibraryDLL::LibraryDLL(const char* library_name)
{
    m_handle = LoadLibraryA(library_name);
    if (m_handle == nullptr)
    {
        log({.level=LogLevel::error}, "Failed to load library: {}.", library_name);
    }
}

LibraryDLL::~LibraryDLL()
{
    if (m_handle != nullptr)
    {
        FreeLibrary((HMODULE)m_handle);
        m_handle = nullptr;
    }
}

FunctionHandle LibraryDLL::load_function(const char* function_name)
{
    if (m_handle != nullptr)
    {
        FARPROC proc = GetProcAddress((HMODULE)m_handle, function_name);
        if (proc == nullptr)
        {
            log({.level=LogLevel::error}, "Failed to load function: {}.", function_name);
        }
        return proc;
    }
    return nullptr;
}