# include "codexion.h"

t_config	create_test_config(int number_of_coders)
{
	t_config	config;

	config.number_of_coders = number_of_coders;
	config.time_to_burnout = 800;
	config.time_to_compile = 200;
	config.time_to_debug = 100;
	config.time_to_refactor = 100;
	config.compiles_required = 3;
	config.dongle_cooldown = 50;
	config.scheduler = SCHED_POLICY_FIFO;
	return (config);
}

int	test_single_coder(void)
{
	t_config		config;
	t_simulation	sim;
	t_error			error;

	config = create_test_config(1);
	error = init_simulation(&sim, &config);
	if (error != NO_ERR)
	{
		printf("FAIL: init single coder\n");
		return (1);
	}
	if (sim.dongle_mutexes_initialized != 1)
	{
		printf("FAIL: initialized mutex count.\n");
		free_simulation(&sim);
		return (1);
	}
	if (pthread_mutex_lock(&sim.dongles[0].mutex) != 0)
	{
		printf("FAIL: mutex lock.\n");
		free_simulation(&sim);
		return (1);
	}
	else
	{
		if (pthread_mutex_unlock(&sim.dongles[0].mutex) != 0)
		{
			printf("FAIL: mutex unlock.\n");
			return (1);
		}
	}
	if (sim.coders == NULL || sim.dongles == NULL)
	{
		printf("FAIL: null arrays\n");
		free_simulation(&sim);
		return (1);
	}
	if (sim.coders[0].id != 1
		|| sim.coders[0].state != WAITING
		|| sim.coders[0].last_compilation_start != 0
		|| sim.coders[0].compilations_done != 0)
	{
		printf("FAIL: coder initial values\n");
		free_simulation(&sim);
		return (1);
	}
	if (sim.dongles[0].is_dispo != DONGLE_AVAILABLE
		|| sim.dongles[0].available_at_ms != 0)
	{
		printf("FAIL: dongle initial values\n");
		free_simulation(&sim);
		return (1);
	}
	if (sim.coders[0].left_dongle != &sim.dongles[0]
		|| sim.coders[0].right_dongle != &sim.dongles[0])
	{
		printf("FAIL: single coder dongle links\n");
		free_simulation(&sim);
		return (1);
	}
	free_simulation(&sim);
	if (sim.coders != NULL || sim.dongles != NULL
		|| sim.dongle_mutexes_initialized != 0)
	{
		printf("FAIL: cleanup pointers\n");
		return (1);
	}
	printf("PASS: single coder\n");
	return (0);
}

int	test_four_coders(void)
{
	t_config		config;
	t_simulation	sim;
	t_error			error;
	int				index;
	int				n;

	config = create_test_config(4);
	error = init_simulation(&sim, &config);
	if (error != NO_ERR)
	{
		printf("FAIL: init four coders\n");
		return (1);
	}
	if (sim.dongle_mutexes_initialized != 4)
	{
		printf("FAIL: initialized mutex count.\n");
		free_simulation(&sim);
		return (1);
	}
	n = config.number_of_coders;
	index = 0;
	while (index < n)
	{
		if (sim.coders[index].id != index + 1
			|| sim.coders[index].state != WAITING
			|| sim.coders[index].last_compilation_start != 0
			|| sim.coders[index].compilations_done != 0)
		{
			printf("FAIL: coder values\n");
			free_simulation(&sim);
			return (1);
		}
		if (sim.dongles[index].is_dispo != DONGLE_AVAILABLE
			|| sim.dongles[index].available_at_ms != 0)
		{
			printf("FAIL: dongle values\n");
			free_simulation(&sim);
			return (1);
		}
		if (sim.coders[index].right_dongle != &sim.dongles[index]
			|| sim.coders[index].left_dongle
			!= &sim.dongles[(index + n - 1) % n])
		{
			printf("FAIL: dongle links\n");
			free_simulation(&sim);
			return (1);
		}
		if (pthread_mutex_lock(&sim.dongles[index].mutex) != 0)
		{
			printf("FAIL: mutex lock.\n");
			free_simulation(&sim);
			return (1);
		}
		else
		{
			if (pthread_mutex_unlock(&sim.dongles[index].mutex) != 0)
			{
				printf("FAIL: mutex unlock.\n");
				return (1);
			}
		}
		index++;
	}
	free_simulation(&sim);
	if (sim.coders != NULL || sim.dongles != NULL
		|| sim.dongle_mutexes_initialized != 0)
	{
		printf("FAIL: cleanup pointers\n");
		return (1);
	}
	printf("PASS: four coders\n");
	return (0);
}

int	test_invalid_parameters(void)
{
	t_config		config;
	t_simulation	sim;

	config = create_test_config(1);
	if (init_simulation(NULL, &config) != INVALID_CONFIG_ERR)
	{
		printf("FAIL: null simulation\n");
		return (1);
	}
	if (init_simulation(&sim, NULL) != INVALID_CONFIG_ERR)
	{
		printf("FAIL: null config\n");
		return (1);
	}
	printf("PASS: invalid parameters\n");
	return (0);
}

int	main(void)
{
	int	failures;

	failures = 0;
	failures += test_single_coder();
	failures += test_four_coders();
	failures += test_invalid_parameters();
	if (failures != 0)
	{
		printf("%d initialization test(s) failed\n", failures);
		return (1);
	}
	printf("All initialization tests passed\n");
	return (0);
}




