#include "codexion.h"

void	*coder_behavior(void *data)
{
	t_coder			*coder;
	t_simulation	*simulation;

	coder = (t_coder *)data;
	simulation = coder->simulation;
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	print_status(coder, "is compiling");
	sleep_ms(simulation->config.time_to_compile);
	print_status(coder, "is debugging");
	sleep_ms(simulation->config.time_to_debug);
	print_status(coder, "is refactoring");
	sleep_ms(simulation->config.time_to_refactor);
	return (NULL);
}
