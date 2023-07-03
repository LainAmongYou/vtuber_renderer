
export module app;

import cstd;
import platform;

export class VtuberApp 
{
public:
    explicit VtuberApp()
    {
        m_client_window = std::make_unique<PlatformWindow>();

        // register events 
        m_event_system.register_event(u16(EventCode::window_resized), this, on_app_resized_callback);
        m_event_system.register_event(u16(EventCode::key_pressed), this, on_app_key_press_callback);

        m_is_initialized = true;
    }

    void deinit()
    {
        m_client_window.reset();
        m_is_initialized = false;
    }

    void run()
    {
    }

    inline bool is_initialized() { return m_is_initialized; }

public: // Removed operators
    ~VtuberApp() = default;

    VtuberApp(const VtuberApp&)  = delete;
    VtuberApp(const VtuberApp&&) = delete;

    VtuberApp& operator=(const VtuberApp&  other) = delete;
    VtuberApp& operator=(const VtuberApp&& other) = delete;

private:
    bool                            m_is_initialized = false;
    bool                            m_is_running     = false;
    InputSystem                     m_input_system{};
    EventSystem                     m_event_system{};
    std::unique_ptr<PlatformWindow> m_client_window;

    void handle_resize(s32 width, s32 height)
    {
        //TODO
    }

    void handle_key_press(InputKey key)
    {
        if (key == InputKey::escape)
        {
            m_is_running = false;
        }
    }

    //
    // Event Callbacks
    //

    static bool on_app_resized_callback(u16 code, Event event, void* listener_inst, void* sender)
    {
        if (listener_inst)
        {
            VtuberApp* app = static_cast<VtuberApp*>(listener_inst);
            app->handle_resize(event.window_size[0], event.window_size[1]);
        }
        return false;
    }

    static bool on_app_key_press_callback(u16 code, Event event, void* listener_inst, void* sender)
    {
        if (listener_inst)
        {
            VtuberApp* app = static_cast<VtuberApp*>(listener_inst);
            app->handle_key_press(InputKey(event.key));
        }
        return false;
    }
};
