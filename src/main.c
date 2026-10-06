#include "codexion.h"

int	main(int argc, char **argv)
{
	t_config			config;
	t_error_parsing		error;

	error = parse_args(argc, argv, &config);
	if (error != NO_ERR)
	{
		print_error_parsing(error);
		return (1);
	}
	return (0);
}
