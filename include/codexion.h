#ifndef CODEXION_H
# define CODEXION_H

# include <stddef.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>

//Colors
# define YELLOW "\033[38;5;3m"
# define BLUE "\033[38;5;4m"
# define GREEN "\033[38;5;48m"

typedef enum e_error
{
	NO_ERR,
	MALLOC_ERR,
	INVALID_ARGS_NUMBER,
	INVALID_NUMBER_ERR,
	NUMBER_OVERFLOW_ERR,
	INVALID_SCHEDULER_ERR,
	INVALID_CONFIG_ERR,
	MUTEX_INIT_ERR,
	COND_INIT_ERR,
	THREAD_ERR
}	t_error;

typedef enum e_dongle_flag
{
	DONGLE_AVAILABLE,
	DONGLE_UNAVAILABLE
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
	REFACTORING,
	BURNOUT,
	DONE
}	t_coder_state;

typedef enum e_sim_state
{
	RUNNING,
	FINISHED,
	BURNING
}	t_sim_state;

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
	pthread_mutex_t		mutex_dongle;
}	t_dongle;

typedef struct s_simulation t_simulation;

typedef struct s_coder
{
	int					id;
	t_coder_state		state;
	long				last_compilation;
	int					compilations_done;
	pthread_mutex_t		mutex_coder;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_simulation		*simulation;
}	t_coder;

typedef struct s_simulation
{
	t_config			config;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_t			*thread_coder;
	pthread_t			monitoring;
	pthread_mutex_t		mutex_printer;
	pthread_mutex_t		mutex_starter;
	pthread_cond_t		start_condition;
	long				simulation_start;
	int					thread_created;
	t_sim_state			state;
}	t_simulation;

t_error			parse_number(const char *str, int *value);
t_error			parse_scheduler(const char *str, t_scheduler *scheduler);
t_error			validate_config(t_config *config);
t_error			parse_args(int argc, char **argv, t_config *config);
void			print_error(t_error error);
long			get_time_ms(void);
bool			sleep_ms(long duration, t_coder *coder);
t_error			init_simulation(t_simulation *simulation, t_config *config);
t_error			allocate_simulation_arrays(t_simulation *simulation);
t_error			init_dongles(t_simulation *simulation);
void			init_coders(t_simulation *simulation);
t_error			init_thread(t_simulation *simulation);
t_error			join_threads(t_simulation *simulation);
void			free_simulation(t_simulation *simulation);
void			*coder_behavior(void *data);
void			*monitoring(void *data);
void			print_status(t_coder *coder, t_coder_state state);
bool			check_burnout(t_coder *coder);
t_sim_state		check_simulation_state(t_simulation *simulation);

#endif
