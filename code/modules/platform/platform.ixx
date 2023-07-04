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