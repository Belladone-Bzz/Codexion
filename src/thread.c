#include "codexion.h"

t_error	init_thread(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	if (pthread_create(&simulation->monitoring,
			NULL, monitoring, simulation->coders) != 0)
		return (THREAD_ERR);
	simulation->simulation_start = get_time_ms();
	while (index < nb_max)
	{
		if (pthread_create(&simulation->thread_coder[index],
				NULL, coder_behavior, &simulation->coders[index]) != 0)
			return (THREAD_ERR);
		simulation->thread_created ++;
		index ++;
	}
	return (NO_ERR);
}

t_error	join_threads(t_simulation *simulation)
{
	int		index;

	if (pthread_join(simulation->monitoring, NULL) != 0)
		return (THREAD_ERR);
	index = 0;
	while (index < simulation->thread_created)
	{
		if (pthread_join(simulation->thread_coder[index], NULL) != 0)
		{
			simulation->thread_created = 0;
			return (THREAD_ERR);
		}
		index ++;
	}
	simulation->thread_created = 0;
	return (NO_ERR);
}
