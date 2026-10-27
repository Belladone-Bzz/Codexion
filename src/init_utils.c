#include "codexion.h"

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



