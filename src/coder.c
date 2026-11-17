#include "codexion.h"

static void	set_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->state = state;
	print_status(coder, state);
	pthread_mutex_unlock(&coder->mutex_coder);
}

bool	check_burnout(t_coder *coder)
{
	if (get_time_ms() - coder->last_compilation
		>= coder->simulation->config.time_to_burnout)
	{
		set_state(coder, BURNOUT);
		return (true);
	}
	return (false);
}

static void	queue_attribution(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	if (coder->left_dongle->queue[0] == NULL
		|| coder->left_dongle->queue[0] == coder)
		coder->left_dongle->queue[0] = coder;
	else
		coder->left_dongle->queue[1] = coder;
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
	if (coder->right_dongle->queue[0] == NULL
		|| coder->right_dongle->queue[0] == coder)
		coder->right_dongle->queue[0] = coder;
	else
		coder->right_dongle->queue[1] = coder;
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
}

static int	coder_processing(t_coder *coder)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	queue_attribution(coder);
	while (request_dongles(coder) == false)
		if (check_simulation_state(simulation) != RUNNING
			|| check_burnout(coder) == true)
			return (1);
	set_state(coder, COMPILING);
	coder->last_compilation = get_time_ms();
	if (sleep_ms(simulation->config.time_to_compile, coder) == false)
		return (1);
	coder->compilations_done++;
	set_state(coder, DEBUGGING);
	if (sleep_ms(simulation->config.time_to_debug, coder) == false)
		return (1);
	set_state(coder, REFACTORING);
	if (sleep_ms(simulation->config.time_to_refactor, coder) == false)
		return (1);
	return (0);
}

void	*coder_behavior(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex_starter);
	pthread_cond_wait(&simulation->start_condition, &simulation->mutex_starter);
	pthread_mutex_unlock(&simulation->mutex_starter);
	coder->last_compilation = get_time_ms();
	if (coder->id % 2 == 0)
		usleep(100);
	while (coder->compilations_done < simulation->config.compiles_required
		&& check_simulation_state(simulation) == RUNNING
		&& check_burnout(coder) == false)
		if (simulation->config.number_of_coders != 1
			&& coder_processing(coder) != 0)
			return (NULL);
	if (coder->compilations_done == simulation->config.compiles_required)
		set_state(coder, DONE);
	return (NULL);
}
