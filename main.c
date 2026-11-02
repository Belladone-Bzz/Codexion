#include "codexion.h"

t_error	run_simulation(t_config *config)
{
	t_simulation	simulation;
	t_error			error;

	error = init_simulation(&simulation, config);
	if (error != NO_ERR)
		return (error);
	simulation.simulation_start = get_time_ms();
	error = init_thread(&simulation);
	if (error != NO_ERR)
	{
		join_threads(&simulation);
		free_simulation(&simulation);
		return (error);
	}
	error = join_threads(&simulation);
	free_simulation(&simulation);
	return (error);
}

int	main(int argc, char **argv)
{
	t_config		config;
	t_error			error;

	error = parse_args(argc, argv, &config);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	error = run_simulation(&config);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	return (0);
}
