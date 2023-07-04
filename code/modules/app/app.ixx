
export module app;

import cstd;
import platform;

export class VtuberApp 
{
public:
    explicit VtuberApp()
    {
        logger_initialize(
            LogLevel::trace,
            LogMode::console,
            { //foreground colors
                .trace = LogColor::grey,
                .debug = LogColor::blue,
                .info  = LogColor::white,
                .warn  = LogColor::yellow,
                .error = LogColor::dark_red,
                .fatal = LogColor::red,
            }
            // leaves background as black
        );

        platform_initialize();

        m_client_window = std::make_unique<PlatformWindow>("Vtuber Renderer", 1920, 1080);

        // register events 
        m_event_system.register_event(u16(EventCode::window_resized), this, on_app_resized_callback);
        m_event_system.register_event(u16(EventCode::key_pressed), this, on_app_key_press_callback);

        log({}, "VTuber App is initialized.");
        m_is_initialized = true;
    }

    void deinit()
    {
        m_client_window.reset();
        platform_shutdown();
        logger_shutdown();
        m_is_initialized = false;
    }

    void run()
    {
        f32 target_refresh_rate = m_client_window->get_refresh_rate();
        Timer frame_cap = Timer();

        m_is_running = true;
        while (m_is_running)
        {
            frame_cap.start();

            bool success = m_client_window->pump_event_messages(m_input_system, m_event_system);
            if (!success || !m_is_running) 
            {
                m_is_running = false;
                continue;
            }

            //TODO: everything

            // Cap the frame rate - TODO(enlynn): make this optional
            frame_cap.update();
            f32 work_seconds_elapsed = frame_cap.get_seconds_elapsed();
            f32 seconds_elapsed_for_frame = work_seconds_elapsed;
            if (seconds_elapsed_for_frame < target_refresh_rate)
            {
                u32 sleep_ms = static_cast<u32>(1000.0f * (target_refresh_rate - seconds_elapsed_for_frame));
                if (sleep_ms > 0)
                {
                    platform_sleep(sleep_ms);
                }
            }

#if 0 // log the framerate
            frame_cap.update();
            work_seconds_elapsed = frame_cap.get_seconds_elapsed();
            log({}, "Frame Rate {}s", work_seconds_elapsed);
#endif 
        }
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
    std::unique_ptr<PlatformWindow> m_client_window = nullptr;

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
