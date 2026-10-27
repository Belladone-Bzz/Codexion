#include "codexion.h"

t_error	init_simulation(t_simulation *simulation, t_config *config)
{
	if (simulation == NULL || config == NULL)
		return (INVALID_CONFIG_ERR);
	simulation->simulation_start = 0;
	simulation->dongle_mutexes_initialized = 0;
	simulation->config = *config;
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->thread_coder = NULL;
	if (allocate_simulation_arrays(simulation) != NO_ERR)
	{
		free_simulation(simulation);
		return (MALLOC_ERR);
	}
	if (init_dongles(simulation) != NO_ERR)
	{
		free_simulation(simulation);
		return (MUTEX_INIT_ERR);
	}
	init_coders(simulation);
	return (NO_ERR);
}

t_error	init_dongles(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	while (index < nb_max)
	{
		simulation->dongles[index].is_dispo = DONGLE_AVAILABLE;
		simulation->dongles[index].available_at_ms = 0;
		if (pthread_mutex_init(
				&simulation->dongles[index].mutex, NULL) != 0)
			return (MUTEX_INIT_ERR);
		simulation->dongle_mutexes_initialized ++;
		index ++;
	}
	return (NO_ERR);
}

void	init_coders(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	while (index < nb_max)
	{
		simulation->coders[index].id = index + 1;
		simulation->coders[index].state = WAITING;
		simulation->coders[index].last_compilation_start = 0;
		simulation->coders[index].compilations_done = 0;
		simulation->coders[index].right_dongle = &simulation->dongles[index];
		simulation->coders[index].left_dongle = &simulation->dongles[(
				index + nb_max - 1) % nb_max];
		index ++;
	}
}

t_error	init_thread(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	while (index < nb_max)
	{
		if (pthread_create(&simulation->thread_coder[index],
				NULL, coder_routine, &simulation->coders[index]) != 0)
			return (THREAD_ERR);
		index ++;
	}
	return (NO_ERR);
}

void	free_simulation(t_simulation *simulation)
{
	int		index;

	if (simulation == NULL)
		return ;
	index = 0;
	while (simulation->dongles != NULL
		&& index < simulation->dongle_mutexes_initialized)
	{
		pthread_mutex_destroy(&simulation->dongles[index].mutex);
		index ++;
	}
	free(simulation->coders);
	free(simulation->dongles);
	free(simulation->thread_coder);
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->thread_coder = NULL;
	simulation->dongle_mutexes_initialized = 0;
}
