#include "codexion.h"

t_error	init_thread(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	if (pthread_create(&simulation->monitoring,
				NULL, monitoring, &simulation->monitoring) != 0)
		return (THREAD_ERR);
	while (index < nb_max)
	{
		if (pthread_create(&simulation->thread_coder[index],
				NULL, coder_behavior, &simulation->coders[index]) != 0)
			return (THREAD_ERR);
		index ++;
	}
	sleep_ms(10);
	pthread_cond_broadcast(&simulation->start_condition);
	return (NO_ERR);
}

t_error	join_threads(t_simulation *simulation)
{
	int		index;

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
