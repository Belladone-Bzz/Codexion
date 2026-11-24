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
	long	end_time;

	end_time = get_time_ms() + duration;
	while (get_time_ms() < end_time)
	{
		if (check_simulation_state(coder->simulation) != RUNNING
			|| check_burnout(coder) == true)
			return (false);
	}
	return (true);
}
