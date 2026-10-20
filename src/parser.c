#include "codexion.h"

t_error	parse_number(const char *str, int *value)
{
	int		index;
	int		digit;
	int		result;

	if (str == NULL || *str == '\0' || value == NULL)
		return (INVALID_NUMBER_ERR);
	index = 0;
	result = 0;
	while (str[index] != '\0')
	{
		if (str[index] < '0' || str[index] > '9')
			return (INVALID_NUMBER_ERR);
		digit = str[index] - '0';
		if (result > INT_MAX / 10
			|| (result == INT_MAX / 10 && digit > INT_MAX % 10))
			return (NUMBER_OVERFLOW_ERR);
		result = result * 10 + digit;
		index++;
	}
	*value = result;
	return (NO_ERR);
}

t_error	parse_scheduler(const char *str, t_scheduler *scheduler)
{
	if (str == NULL || *str == '\0' || scheduler == NULL)
		return (INVALID_SCHEDULER_ERR);
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler = SCHED_POLICY_FIFO;
		return (NO_ERR);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler = SCHED_POLICY_EDF;
		return (NO_ERR);
	}
	return (INVALID_SCHEDULER_ERR);
}

t_error	validate_config(t_config *config)
{
	if (config == NULL)
		return (INVALID_CONFIG_ERR);
	if (config->number_of_coders < 1 || config->number_of_coders > 10000)
		return (INVALID_CONFIG_ERR);
	if (config->time_to_burnout < 0 || config->time_to_compile < 0
		|| config->time_to_debug < 0 || config->time_to_refactor < 0
		|| config->compiles_required < 0 || config->dongle_cooldown < 0)
		return (INVALID_CONFIG_ERR);
	return (NO_ERR);
}

t_error	parse_args(int argc, char **argv, t_config *config)
{
	int				*values[7];
	int				index;
	t_error	error;

	if (argc != 9 || argv == NULL || config == NULL)
		return (INVALID_ARGS_NUMBER);
	values[0] = &config->number_of_coders;
	values[1] = &config->time_to_burnout;
	values[2] = &config->time_to_compile;
	values[3] = &config->time_to_debug;
	values[4] = &config->time_to_refactor;
	values[5] = &config->compiles_required;
	values[6] = &config->dongle_cooldown;
	index = 0;
	while (index < 7)
	{
		error = parse_number(argv[index + 1], values[index]);
		if (error != NO_ERR)
			return (error);
		index++;
	}
	error = parse_scheduler(argv[8], &config->scheduler);
	if (error != NO_ERR)
		return (error);
	return (validate_config(config));
}
