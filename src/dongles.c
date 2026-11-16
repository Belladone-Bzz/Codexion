#include "codexion.h"

static void	check_queue(t_dongle *left, t_dongle *right)
{
	t_coder		*priority;
	if (left->queue[0] != NULL && left->queue[1] != NULL)
	{
		pthread_mutex_lock(&left->queue[0]->mutex_coder);
		pthread_mutex_lock(&left->queue[1]->mutex_coder);
		if (left->queue[1]->last_compilation - get_time_ms()
		< left->queue[0]->last_compilation - get_time_ms())
		{
			priority = left->queue[1];
			left->queue[1] = left->queue[0];
			left->queue[0] = priority;
		}
		pthread_mutex_unlock(&left->queue[1]->mutex_coder);
		pthread_mutex_unlock(&left->queue[0]->mutex_coder);
	}
	if (right->queue[0] != NULL && right->queue[1] != NULL)
	{
		pthread_mutex_lock(&right->queue[0]->mutex_coder);
		pthread_mutex_lock(&right->queue[1]->mutex_coder);
		if (right->queue[1]->last_compilation - get_time_ms()
		< right->queue[0]->last_compilation - get_time_ms())
		{
			priority = right->queue[1];
			right->queue[1] = right->queue[0];
			right->queue[0] = priority;
		}
		pthread_mutex_unlock(&right->queue[1]->mutex_coder);
		pthread_mutex_unlock(&right->queue[0]->mutex_coder);
	}
}

static void	check_cooldown(t_dongle *left, t_dongle *right)
{
	if (left->available_at_ms <= get_time_ms())
		left->is_dispo = AVAILABLE;
	if (right->available_at_ms <= get_time_ms())
		right->is_dispo = AVAILABLE;
}

bool	request_dongles(t_coder *coder)
{
	if (coder->simulation->config.scheduler == SCHED_POLICY_EDF)
		check_queue(coder->left_dongle, coder->right_dongle);
	pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
	check_cooldown(coder->left_dongle, coder->right_dongle);
	if (coder->left_dongle->is_dispo == AVAILABLE
		&& coder->left_dongle->queue[0] == coder
		&& coder->right_dongle->is_dispo == AVAILABLE
		&& coder->right_dongle->queue[0] == coder)
	{
		coder->left_dongle->is_dispo = UNAVAILABLE;
		coder->left_dongle->available_at_ms = (get_time_ms()
			+ coder->simulation->config.dongle_cooldown
			+ coder->simulation->config.time_to_compile);
		coder->left_dongle->queue[0] = coder->left_dongle->queue[1];
		coder->left_dongle->queue[1] = NULL;
		coder->right_dongle->is_dispo = UNAVAILABLE;
		coder->right_dongle->available_at_ms = (get_time_ms()
			+ coder->simulation->config.dongle_cooldown
			+ coder->simulation->config.time_to_compile);
		coder->right_dongle->queue[0] = coder->right_dongle->queue[1];
		coder->right_dongle->queue[1] = NULL;
		pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
		pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
		return (true);
	}
	pthread_mutex_unlock(&coder->right_dongle->mutex_dongle);
	pthread_mutex_unlock(&coder->left_dongle->mutex_dongle);
	return (false);
}
