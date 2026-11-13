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

bool	sleep_ms(long duration, t_coder *coder)
{
	long	duration_ms;
	
	duration_ms = duration*1000;
	while (duration_ms > 0)
	{
		if (coder->simulation->state != RUNNING
			|| check_burnout(coder) == true)
			return false;
		usleep(100);
		duration_ms -= 100;
	}
	return (true);
}
