#ifndef CODEXION_H
# define CODEXION_H

# include <stddef.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>

typedef enum e_scheduler
{
	SCHED_POLICY_FIFO,
	SCHED_POLICY_EDF
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

int		parse_number(const char *str, int *value);
int		parse_scheduler(const char *str, t_scheduler *scheduler);
int		validate_config(t_config *config);
int		parse_args(int argc, char **argv, t_config *config);

#endif
