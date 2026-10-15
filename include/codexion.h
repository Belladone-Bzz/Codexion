#ifndef CODEXION_H
# define CODEXION_H

# include <stddef.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <time.h>
# include <unistd.h>

typedef enum e_error_parsing
{
	NO_ERR,
	INVALID_ARGS_NUMBER,
	INVALID_NUMBER_ERR,
	NUMBER_OVERFLOW_ERR,
	INVALID_SCHEDULER_ERR,
	INVALID_CONFIG_ERR
}	t_error_parsing;

typedef enum e_dongle_flag
{
	DONGLE_AVAILABLE,
	DONGLE_UNAVAILABLE,
}	t_dongle_flag;

typedef enum e_scheduler
{
	SCHED_POLICY_FIFO,
	SCHED_POLICY_EDF
}	t_scheduler;

typedef enum e_coder_state
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING
}	t_coder_state;

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

typedef struct s_dongle
{
	t_dongle_flag		is_dispo;
	long				available_at_ms;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	t_coder_state	state;
	long			last_compilation_start;
	int				compilations_done;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
}	t_coder;

typedef struct s_simulation
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	long			simulation_start;
}	t_simulation;


t_error_parsing		parse_number(const char *str, int *value);
t_error_parsing		parse_scheduler(const char *str, t_scheduler *scheduler);
t_error_parsing		validate_config(t_config *config);
t_error_parsing		parse_args(int argc, char **argv, t_config *config);
void				print_error_parsing(t_error_parsing error);
long				get_time_ms(void);
void				sleep_ms(long duration);

#endif
