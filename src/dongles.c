#include "codexion.h"

static void	check_queue(t_dongle *dongle)
{
	t_coder		*priority;
	if (dongle->queue[0] != NULL && dongle->queue[1] != NULL)
	{
		pthread_mutex_lock(&dongle->queue[0]->mutex_coder);
		pthread_mutex_lock(&dongle->queue[1]->mutex_coder);
		if (dongle->queue[1]->last_compilation - get_time_ms()
		< dongle->queue[0]->last_compilation - get_time_ms())
		{
			priority = dongle->queue[1];
			dongle->queue[1] = dongle->queue[0];
			dongle->queue[0] = priority;
		}
		pthread_mutex_unlock(&dongle->queue[1]->mutex_coder);
		pthread_mutex_unlock(&dongle->queue[0]->mutex_coder);
	}
}

static void	check_cooldown(t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->mutex_dongle);
	if (left->available_at_ms <= get_time_ms())
		left->is_dispo = AVAILABLE;
	pthread_mutex_unlock(&left->mutex_dongle);
	pthread_mutex_lock(&right->mutex_dongle);
	if (right->available_at_ms <= get_time_ms())
		right->is_dispo = AVAILABLE;
	pthread_mutex_unlock(&right->mutex_dongle);
}

static void	update_dongles_status(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex_dongle);
	pthread_mutex_lock(&coder->right_dongle->mutex_dongle);
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
}

bool	request_dongles(t_coder *coder)
{
	if (coder->simulation->config.scheduler == SCHED_POLICY_EDF)
	{
		check_queue(coder->left_dongle);
		check_queue(coder->right_dongle);
	}
	check_cooldown(coder->left_dongle, coder->right_dongle);
	if (coder->left_dongle->is_dispo == AVAILABLE
		&& coder->left_dongle->queue[0] == coder
		&& coder->right_dongle->is_dispo == AVAILABLE
		&& coder->right_dongle->queue[0] == coder)
	{
		update_dongles_status(coder);
		return (true);
	}
	return (false);
}
