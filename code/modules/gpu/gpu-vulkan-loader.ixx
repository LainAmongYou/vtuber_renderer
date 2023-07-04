module;

export module gpu:vulkan.functions;

import :vulkan.vendor;

import platform;

#if defined(_WIN64_IMPL_)
#  define VULKAN_LIB_NAME "vulkan-1.dll"
#elif defined(_LINUX_IMPL_)
#  define VULKAN_LIB_NAME "libvulkan.so"
#else
#  define VULKAN_LIB_NAME ""
#endif

#define VK_EXPORTED_FUNCTION(fun)                                 export PFN_##fun fun = nullptr;
#define VK_GLOBAL_LEVEL_FUNCTION(fun)                             export PFN_##fun fun = nullptr;
#define VK_INSTANCE_LEVEL_FUNCTION(fun)                           export PFN_##fun fun = nullptr;
#define VK_INSTANCE_LEVEL_FUNCTION_FROM_EXTENSION(fun, extension) export PFN_##fun fun = nullptr;
#define VK_DEVICE_LEVEL_FUNCTION(fun)                             export PFN_##fun fun = nullptr;
#define VK_DEVICE_LEVEL_FUNCTION_FROM_EXTENSION(fun, extension)   export PFN_##fun fun = nullptr;

#include "function_list.inl"

export LibraryDLL load_vulkan_library()
{
    return LibraryDLL(VULKAN_LIB_NAME);
}

export bool load_exported_functions(LibraryDLL& library)
{
#define VK_EXPORTED_FUNCTION(fun)                                                      \
    if (!(fun = (PFN_##fun)library.load_function(#fun))) {                             \
        log({.level=LogLevel::error}, "Could not load exported function: %s\n", #fun); \
        return false;                                                                  \
    }
    
#include "function_list.inl"
    
    log({}, "Loaded vulkan exported functions");
    return true;
}

export bool load_global_level_entry_points()
{
#define VK_GLOBAL_LEVEL_FUNCTION(fun)                                                       \
    if (!(fun = (PFN_##fun)vkGetInstanceProcAddr(nullptr, #fun))) {                         \
        log({.level=LogLevel::error}, "Could not load global level function: %s!\n", #fun); \
        return false;                                                                       \
    }
    
#include "function_list.inl"
    
    log({}, "Loaded vulkan global level entry functions");
    return true;
}

