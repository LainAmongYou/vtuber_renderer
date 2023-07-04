module;

export module platform:timer;

import core;

export class Timer 
{
public:
    Timer()
    {
        m_start = 0.0f;
        m_elapsed = 0.0f;
        init_internal();
    }

    ~Timer()
    {
        m_perf_frequency = 0.0f;
        m_start = 0.0f;
        m_elapsed = 0.0f;
    }

    void start()
    {
        m_start = get_absolute_time();
        m_elapsed = 0.0;
    }

    void update()
    {
        m_elapsed = get_absolute_time() - m_start;
    }

    f64 get_seconds_elapsed() const 
    {
        return m_elapsed;
    }

    f64 get_miliseconds_elapsed() const
    {
        return m_elapsed * 1000.0;
    }

private: // Platform specific code
    void init_internal();
    f64  get_absolute_time(); // time in seconds

private: // internal state
    f64 m_perf_frequency;
    f64 m_start;
    f64 m_elapsed;
};