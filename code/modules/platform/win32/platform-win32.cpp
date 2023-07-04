module;

#include <Windows.h>
#include <timeapi.h>

module platform;

import <cassert>;

void platform_initialize()
{
    UINT desired_scheduler_ms = 1;
	bool sleep_is_granular = (timeBeginPeriod(desired_scheduler_ms) == TIMERR_NOERROR);
	assert(sleep_is_granular);
}

void platform_shutdown()
{
}

void platform_sleep(u32 time_in_ms)
{
    Sleep(time_in_ms);
}