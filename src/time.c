#include "codexion.h"

long	get_time_ms(void)
{
	struct timespec		ts;
	long				time_ms;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
		return (1);
	time_ms = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
	return (time_ms);
}

void	sleep_ms(long duration)
{
	if (duration > 0)
		usleep(duration * 1000);
}
