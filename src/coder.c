#include "codexion.h"

void	*coder_behavior(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_printer);
	pthread_cond_wait(&simulation->start_condition, NULL);
	pthread_mutex_unlock(&simulation->mutex_printer);
	while (coder->compilations_done < simulation->config.compiles_required)
	{
		coder->state = WAITING;
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
		print_status(coder, "is compiling");
		coder->state = COMPILING;
		sleep_ms(simulation->config.time_to_compile);
		print_status(coder, "is debugging");
		coder->state = DEBUGGING;
		sleep_ms(simulation->config.time_to_debug);
		print_status(coder, "is refactoring");
		coder->state = REFACTORING;
		sleep_ms(simulation->config.time_to_refactor);
	}
	coder->state = DONE;
	return (NULL);
}
