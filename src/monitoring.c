#include "codexion.h"

t_simulation_state	check_coders(t_simulation *simulation)
{
	int	index;
	int	coders_done;

	index = 0;
	coders_done = 0;
	while (index < simulation->config.number_of_coders)
	{
		pthread_mutex_lock(&simulation->coders->mutex_coder);
		if (simulation->coders[index].state == BURNOUT)
		{
			simulation->state = BURNING;
			pthread_mutex_unlock(&simulation->coders->mutex_coder);
			break ;
		}
		if (simulation->coders[index].state == DONE)
			coders_done ++;
		index ++;
		pthread_mutex_unlock(&simulation->coders->mutex_coder);
	}
	if (coders_done == simulation->config.number_of_coders)
		simulation->state = FINISHED;
	return (simulation->state);
}

void	*monitoring(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_starter);
	pthread_cond_wait(&simulation->start_condition,
		&simulation->mutex_starter);
	pthread_mutex_unlock(&simulation->mutex_starter);
	while (check_coders(simulation) == RUNNING)
		continue;
	return (NULL);
}