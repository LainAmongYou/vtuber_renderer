module;

export module gpu:instance;

import :vulkan;

import platform;
import <cassert>;

export class GpuInstance
{
public:
    GpuInstance() : m_vulkan_lib(load_vulkan_library())
    {
        bool load_result = load_exported_functions(m_vulkan_lib);
        assert(load_result);

        load_result = load_global_level_entry_points();
        assert(load_result);
    }

    ~GpuInstance()
    { // TODO
    }

public: //removed operators
    GpuInstance(const GpuInstance&)  = delete;
    GpuInstance(const GpuInstance&&) = delete;

    GpuInstance& operator=(const GpuInstance&  other) = delete;
    GpuInstance& operator=(const GpuInstance&& other) = delete;

private: //internal functions

private: //internal state
    VkInstance m_handle;
    LibraryDLL m_vulkan_lib;
};