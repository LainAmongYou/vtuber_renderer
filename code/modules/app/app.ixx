
export module app;

import cstd;
import platform;

export class VtuberApp 
{
public:
    explicit VtuberApp()
    {
        m_client_window = std::make_unique<PlatformWindow>();
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
    std::unique_ptr<PlatformWindow> m_client_window;
};