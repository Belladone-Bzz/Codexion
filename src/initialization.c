#include "codexion.h"

t_error	init_simulation(t_simulation *simulation, t_config *config)
{
	if (simulation == NULL || config == NULL)
		return (INVALID_CONFIG_ERR);
	simulation->simulation_start = 0;
	simulation->config = *config;
	simulation->coders = NULL;
	simulation->dongles = NULL;
	simulation->coders = malloc(
		config->number_of_coders * sizeof(t_coder));
	simulation->dongles = malloc(
		config->number_of_coders * sizeof(t_dongle));
	if (simulation->coders == NULL || simulation->dongles == NULL)
	{
		free_simulation(simulation);
		return (MALLOC_ERR);
	}
	init_dongles(simulation);
	init_coders(simulation);
	return (NO_ERR);
}

void	init_dongles(t_simulation *simulation)
{
	int		index;
	int		nb_max;

	index = 0;
	nb_max = simulation->config.number_of_coders;
	while (index < nb_max)
	{
		simulation->dongles[index].is_dispo = DONGLE_AVAILABLE;
		simulation->dongles[index].available_at_ms = 0;
		index ++;
	}
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
		simulation->coders[index].right_dongle =
			&simulation->dongles[index];
		simulation->coders[index].left_dongle =
			&simulation->dongles[(index + nb_max - 1) % nb_max];
		index ++;
	}
}

void	free_simulation(t_simulation *simulation)
{
	if (simulation == NULL)
		return ;
	free(simulation->coders);
	free(simulation->dongles);
	simulation->coders = NULL;
	simulation->dongles = NULL;
}
