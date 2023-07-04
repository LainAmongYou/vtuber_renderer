module; 

#include <Windows.h>

module platform:timer;

void Timer::init_internal()
{
    LARGE_INTEGER perf_count_frequency_result;
	QueryPerformanceFrequency(&perf_count_frequency_result);
	m_perf_frequency = 1.0 / (f64)perf_count_frequency_result.QuadPart;
}

f64 Timer::get_absolute_time()
{  // time in seconds
    LARGE_INTEGER result{};
    QueryPerformanceCounter(&result);
    return result.QuadPart * m_perf_frequency;
}