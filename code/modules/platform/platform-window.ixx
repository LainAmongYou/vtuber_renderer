module;

export module platform:window;

import :event;
import :input;
import cstd;

export using PlatformWindowHandle = void*;

export class PlatformWindow
{
public:
    PlatformWindow(const char* client_window_name, const s32 window_width, const s32 window_height);
    ~PlatformWindow();

    // std::optional<std::reference_wrapper<T>> == std::optional<T&>
    bool pump_event_messages(
        std::optional<std::reference_wrapper<InputSystem>> input_system = std::nullopt, 
        std::optional<std::reference_wrapper<EventSystem>> event_system = std::nullopt);

    f32 get_refresh_rate() const { return m_refresh_rate; }

private: // internal functions

private: // internal state
    static const inline char* c_wnd_class = "LYNN_WND_CLASS";

    bool                 m_is_running = false;
    PlatformWindowHandle m_handle     = nullptr;
    // TODO(enlynn): currently do not check if the window moves to a new monitor, so it is
    // currently possible for this value to become stale.
    f32                  m_refresh_rate = 0.0f; // in frames per second
};