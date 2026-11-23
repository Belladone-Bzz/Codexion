#include "codexion.h"

void	set_simulation_state(t_simulation *simulation, t_sim_state state)
{
	pthread_mutex_lock(&simulation->mutex_starter);
	simulation->state = state;
	pthread_mutex_unlock(&simulation->mutex_starter);
}

static t_sim_state	check_coders(t_simulation *simulation)
{
	int	index;
	int	coders_done;

	index = 0;
	coders_done = 0;
	while (index < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders[index].mutex_coder);
		if (simulation->coders[index].state == DONE)
			coders_done ++;
		pthread_mutex_unlock(&simulation->coders[index].mutex_coder);
		index ++;
	}
	if (coders_done == simulation->config.number_of_coders)
		set_simulation_state(simulation, FINISHED);
	return (check_simulation_state(simulation));
}

t_sim_state	check_simulation_state(t_simulation *simulation)
{
	t_sim_state	sim_state;

	pthread_mutex_lock(&simulation->mutex_starter);
	sim_state = simulation->state;
	pthread_mutex_unlock(&simulation->mutex_starter);
	return (sim_state);
}

void	*monitoring(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	while (check_coders(simulation) == RUNNING)
		continue ;
	return (NULL);
}
