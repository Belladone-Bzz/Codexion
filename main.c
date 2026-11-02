#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;
	t_error			error;

	error = parse_args(argc, argv, &config);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	error = init_simulation(&simulation, &config);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	simulation.simulation_start = get_time_ms();
	error = init_thread(&simulation);
	if (error != NO_ERR)
	{
		join_threads(&simulation);
		free_simulation(&simulation);
		print_error(error);
		return (1);
	}
	error = join_threads(&simulation);
	free_simulation(&simulation);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	return (0);
}
