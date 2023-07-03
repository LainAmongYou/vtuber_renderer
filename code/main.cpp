#ifdef _WIN64_IMPL_
#include <Windows.h>
#endif

import cstd;
import app;

static void load()
{
    VtuberApp app = VtuberApp();
    if (app.is_initialized())
    {
        app.run();
        app.deinit();
    }
}

#ifdef _WIN64_IMPL_
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{ // TODO(enlynn): pass command line arguments onwards
    load();
    return 0;
}
#else
#error Platform not currently supported.
int main(void)
{
    std::cout << "Hello world" << std::cout;
    return 0;
}
#endif
