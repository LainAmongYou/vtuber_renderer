module;

export module renderer;

import gpu;
import cstd;

export class Renderer
{
public:
    Renderer()
    {
        gpu_instance = std::make_unique<GpuInstance>();
    }

    ~Renderer() {}

private: //internal state
    std::unique_ptr<GpuInstance> gpu_instance;
};