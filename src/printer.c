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

void	print_error(t_error error)
{
	if (error == INVALID_NUMBER_ERR)
		fprintf(stderr, "Error: numeric arguments must contain digits only.\n");
	else if (error == NUMBER_OVERFLOW_ERR)
		fprintf(stderr, "Error: numeric argument exceeds INT_MAX.\n");
	else if (error == INVALID_SCHEDULER_ERR)
		fprintf(stderr, "Error: scheduler must be <fifo> or <edf>.\n");
	else if (error == INVALID_CONFIG_ERR)
		fprintf(stderr, "Error: invalid configuration. number_of_coders"
			" must be between 1 and 10000, and timing and compilation arguments"
			" must be >= 0.\n");
	else if (error == INVALID_ARGS_NUMBER)
		fprintf(stderr, "Error: invalid argument count.\n");
	else if (error == MALLOC_ERR)
		fprintf(stderr, "Error: memory allocation failed.\n");
	else if (error == MUTEX_INIT_ERR)
		fprintf(stderr, "Error: mutex initialization failed.\n");
	else
		fprintf(stderr, "Error: unknown error.\n");
}
