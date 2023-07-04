module;

export module platform;

export import :window;
export import :input;
export import :event;
export import :logger;
export import :timer;

export  void platform_initialize();
export  void platform_shutdown();

export void platform_sleep(u32 time_in_ms);

export using LibraryHandle  = void*;
export using FunctionHandle = void*;

export class LibraryDLL
{
public:
    LibraryDLL(const char* library_name);
    ~LibraryDLL();

    // Copies are not allowed - we don't want there to be multiple owners of a library floating about
    // this will cause complications when releasing. If this is needed, use a shared_ptr or unique_ptr.
    LibraryDLL(const LibraryDLL&)            = delete;
    LibraryDLL& operator=(const LibraryDLL&) = delete;

    // Moves are ok - there is still only a single owner.
    LibraryDLL(LibraryDLL&& other)
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
    }

    LibraryDLL& operator=(LibraryDLL&& other)
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        return *this;
    }

    FunctionHandle load_function(const char* function_name);
    void unload_function(FunctionHandle function);

private:
    LibraryHandle               m_handle{nullptr};
};