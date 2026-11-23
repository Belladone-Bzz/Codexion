#include "codexion.h"

void	print_status(t_coder *coder, t_coder_state state)
{
	long			elapsed_time;
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_printer);
	if (check_simulation_state(simulation) != RUNNING)
	{
		pthread_mutex_unlock(&simulation->mutex_printer);
		return ;
	}
	elapsed_time = get_time_ms() - simulation->simulation_start;
	printf(YELLOW"%ld "BLUE"%d ", elapsed_time, coder->id);
	if (state == TAKING_DONGLE)
		printf(GREEN"has taken a dongle\n");
	else if (state == COMPILING)
		printf(GREEN"is compiling\n");
	else if (state == DEBUGGING)
		printf(GREEN"is debugging\n");
	else if (state == REFACTORING)
		printf(GREEN"is refactoring\n");
	else if (state == BURNOUT)
		printf(GREEN"burned out\n");
	else if (state == DONE)
		printf(GREEN"has finished compiling\n");
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
			" must be between 1 and 200, and timing and compilation arguments"
			" must be >= 0.\n");
	else if (error == INVALID_ARGS_NUMBER)
		fprintf(stderr, "Error: invalid argument count.\n");
	else if (error == MALLOC_ERR)
		fprintf(stderr, "Error: memory allocation failed.\n");
	else if (error == MUTEX_INIT_ERR)
		fprintf(stderr, "Error: mutex initialization failed.\n");
	else if (error == THREAD_ERR)
		fprintf(stderr, "Error: thread initialization failed.\n");
	else
		fprintf(stderr, "Error: unknown error.\n");
}
