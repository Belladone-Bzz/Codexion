#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config		config;
	t_simulation	simulation;
	t_error			error;

	error = parse_args(argc, argv, &config);
	if (error == NO_ERR)
		error = init_simulation(&simulation, &config);
	if (error != NO_ERR)
	{
		print_error(error);
		return (1);
	}
	free_simulation(&simulation);
	return (0);
}
