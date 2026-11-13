#include "codexion.h"

void	set_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->mutex_coder);
	coder->state = state;
	print_status(coder, state);
	pthread_mutex_unlock(&coder->mutex_coder);
}

bool	check_burnout(t_coder *coder)
{
	if (get_time_ms() - coder->last_compilation >=
		coder->simulation->config.time_to_burnout)
	{
		set_state(coder, BURNOUT);
		return (true);
	}
	return (false);
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
	while (coder->compilations_done < simulation->config.compiles_required &&
		simulation->state == RUNNING && check_burnout(coder) == false)
	{
		// Ajouter fonctions pour prendre et gerer dongles
		set_state(coder, COMPILING);
		if (sleep_ms(simulation->config.time_to_compile, coder) == false)
			return (NULL);
		coder->last_compilation = get_time_ms();
		coder->compilations_done++;
		set_state(coder, DEBUGGING);
		if (sleep_ms(simulation->config.time_to_debug, coder) == false)
			return (NULL);
		set_state(coder, REFACTORING);
		if (sleep_ms(simulation->config.time_to_refactor, coder) == false)
			return (NULL);
	}
	set_state(coder, DONE);
	return (NULL);
}
