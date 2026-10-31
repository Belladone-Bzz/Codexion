#include "codexion.h"

void	print_status(t_coder *coder, char *message)
{
	long			elapsed_time;
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_printer);
	elapsed_time = get_time_ms() - simulation->simulation_start;
	printf(YELLOW"%ld "BLUE"%d "GREEN"%s\n", elapsed_time, coder->id, message);
	fflush(stdout);
	pthread_mutex_unlock(&simulation->mutex_printer);
}

