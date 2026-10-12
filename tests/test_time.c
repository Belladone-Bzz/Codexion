#include "codexion.h"

int	main(void)
{
	long	starting;
	long	ending;
	long	duration;
	long	previous;
	long	current;
	int		index;
	int		failures;

	failures = 0;
	printf("=== Duration test ===\n");
	starting = get_time_ms();
	sleep_ms(100);
	ending = get_time_ms();
	duration = ending - starting;
	printf("Requested: 100ms, measured: %ld ms.\n", duration);
	if (duration < 90 || duration > 110)
	{
		printf("FAIL: duration outside expected range.\n");
		failures ++;
	}
	else
		printf("OK: duration test.\n");
	previous = get_time_ms();
	index = 0;
	while (index < 1000)
	{
		current = get_time_ms();
		if (current < 0 || current < previous)
		{
			printf("FAIL: time went backwards or clock failed.\n");
			failures ++;
			break ;
		}
		previous = current;
		index ++;
	}
	if (index == 1000)
		printf("OK: monotonicity test.\n");
	if (failures == 0)
		printf("\nAll tests passed.\n");
	else
		printf("\n%d test(s) failed.\n", failures);
	return (0);
}
