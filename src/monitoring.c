#include "codexion.h"

void	*monitoring(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_printer);
	pthread_cond_wait(&simulation->start_condition, NULL);
	pthread_mutex_unlock(&simulation->mutex_printer);
	return (NULL);
}