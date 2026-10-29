#include "codexion.h"

void	printer_dongle(t_coder *coder, t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->mutex_printer);
	printf(YELLOW"");
	pthread_mutex_unlock(&simulation->mutex_printer);
}