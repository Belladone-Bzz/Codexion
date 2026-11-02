#include "codexion.h"

t_error	init_simulation(t_simulation *simulation, t_config *config)
{
	if (simulation == NULL || config == NULL)
		return (INVALID_CONFIG_ERR);
	simulation->simulation_start = 0;
	simulation->thread_created = 0;
	simulation->config = *config;
	if (pthread_mutex_init(&simulation->mutex_printer, NULL) != 0)
		return (MUTEX_INIT_ERR);
	if (pthread_cond_init(&simulation->start_condition, NULL) != 0)
		return (COND_INIT_ERR);
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
				&simulation->dongles[index].mutex_dongle, NULL) != 0)
			return (MUTEX_INIT_ERR);
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
		simulation->coders[index].simulation = simulation;
		index ++;
	}
}

t_error	allocate_simulation_arrays(t_simulation *simulation)
{
	int		nb_max;

	nb_max = simulation->config.number_of_coders;
	simulation->coders = malloc(nb_max * sizeof(t_coder));
	simulation->dongles = malloc(nb_max * sizeof(t_dongle));
	simulation->thread_coder = malloc(nb_max * sizeof(pthread_t));
	if (simulation->coders == NULL || simulation->dongles == NULL
			|| simulation->thread_coder == NULL)
		return (MALLOC_ERR);
	return (NO_ERR);
}

void	free_simulation(t_simulation *simulation)
{
	int		index;

	if (simulation == NULL)
		return ;
	index = 0;
	while (simulation->dongles != NULL
		&& index < simulation->config.number_of_coders)
	{
		pthread_mutex_destroy(&simulation->dongles[index].mutex_dongle);
		index ++;
	}
	pthread_mutex_destroy(&simulation->mutex_printer);
	free(simulation->coders);
	free(simulation->dongles);
	free(simulation->thread_coder);
}
