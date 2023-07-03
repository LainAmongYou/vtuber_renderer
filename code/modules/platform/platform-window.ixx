module;

export module platform:window;

export using PlatformWindowHandle = void*;

export class PlatformWindow
{
public:
    PlatformWindow();
    ~PlatformWindow();

private:
    PlatformWindowHandle m_handle;
};