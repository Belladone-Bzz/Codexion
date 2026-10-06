#include "codexion.h"

int	test_number(const char *str, int expected_value,
	t_error_parsing expected_result)
{
	int					value;
	t_error_parsing		result;

	value = -1;
	result = parse_number(str, &value);
	if (result != (t_error_parsing)expected_result)
	{
		printf("FAIL: parse_number(\"%s\") returned %d, expected %d\n",
			str, result, expected_result);
		return (1);
	}
	if (result == NO_ERR && value != expected_value)
	{
		printf("FAIL: parse_number(\"%s\") gave %d, expected %d\n",
			str, value, expected_value);
		return (1);
	}
	return (0);
}

int	test_scheduler(const char *str, t_scheduler expected,
	t_error_parsing expected_result)
{
	t_scheduler			scheduler;
	t_error_parsing		result;

	scheduler = -1;
	result = parse_scheduler(str, &scheduler);
	if (result != expected_result)
	{
		printf("FAIL: parse_scheduler(\"%s\") returned %d, expected %d\n",
			str, result, expected_result);
		return (1);
	}
	if (result == NO_ERR && scheduler != expected)
	{
		printf("FAIL: wrong scheduler for \"%s\"\n", str);
		return (1);
	}
	return (0);
}

int	test_parse_args(int argc, char **argv, t_error_parsing expected_result)
{
	t_config			config;
	t_error_parsing		result;

	result = parse_args(argc, argv, &config);
	if (result != expected_result)
	{
		printf("FAIL: parse_args returned %d, expected %d.\n",
			result, expected_result);
		return (1);
	}
	return (0);
}

int	test_valid_args(void)
{
	char				*argv[] = {
		"codexion",
		"5",
		"1000",
		"200",
		"300",
		"400",
		"10",
		"50",
		"fifo"
	};
	t_config			config;
	t_error_parsing		result;

	result = parse_args(9, argv, &config);
	if (result != NO_ERR)
	{
		printf("FAIL: valid arguments returned %d.\n", result);
		return (1);
	}
	if (config.number_of_coders != 5
		|| config.time_to_burnout != 1000
		|| config.time_to_compile != 200
		|| config.time_to_debug != 300
		|| config.time_to_refactor != 400
		|| config.compiles_required != 10
		|| config.scheduler != SCHED_POLICY_FIFO)
	{
		printf("FAIL: config was not filled correctly.\n");
		return (1);
	}
	return (0);
}

int	main(void)
{
	int		failures;
	char	*argv_bad_count[] = {
		"codexion",
		"5",
		"1000"
	};
	char	*argv_bad_number[] = {
		"codexion",
		"-5",
		"1000",
		"200",
		"300",
		"400",
		"10",
		"50",
		"fifo"
	};
	char	*argv_bad_overflow[] = {
		"codexion",
		"5",
		"2147483648",
		"200",
		"300",
		"400",
		"10",
		"50",
		"fifo"
	};
	char	*argv_bad_scheduler[] = {
		"codexion",
		"5",
		"1000",
		"200",
		"300",
		"400",
		"10",
		"50",
		"random"
	};
	char	*argv_bad_config[] = {
		"codexion",
		"0",
		"1000",
		"200",
		"300",
		"400",
		"10",
		"50",
		"fifo"
	};

	failures = 0;
	printf("\n=== parse_number tests ===\n");
	failures += test_number("0", 0, NO_ERR);
	failures += test_number("1", 1, NO_ERR);
	failures += test_number("42", 42, NO_ERR);
	failures += test_number("123456789", 123456789, NO_ERR);
	failures += test_number("2147483646", INT_MAX - 1, NO_ERR);
	failures += test_number("2147483647", INT_MAX, NO_ERR);
	failures += test_number("", 0, INVALID_NUMBER_ERR);
	failures += test_number("-42", 0, INVALID_NUMBER_ERR);
	failures += test_number("+42", 0, INVALID_NUMBER_ERR);
	failures += test_number("42abc", 0, INVALID_NUMBER_ERR);
	failures += test_number("abc42", 0, INVALID_NUMBER_ERR);
	failures += test_number("12.5", 0, INVALID_NUMBER_ERR);
	failures += test_number(" 42", 0, INVALID_NUMBER_ERR);
	failures += test_number("42 ", 0, INVALID_NUMBER_ERR);
	failures += test_number("2147483648", 0, NUMBER_OVERFLOW_ERR);
	failures += test_number("999999999999999999999", 0, NUMBER_OVERFLOW_ERR);
	printf("\n=== parse_scheduler tests ===\n");
	failures += test_scheduler("fifo", SCHED_POLICY_FIFO, NO_ERR);
	failures += test_scheduler("edf", SCHED_POLICY_EDF, NO_ERR);
	failures += test_scheduler("", SCHED_POLICY_FIFO, INVALID_SCHEDULER_ERR);
	failures += test_scheduler("FIFO", SCHED_POLICY_FIFO, INVALID_SCHEDULER_ERR);
	failures += test_scheduler("EDF", SCHED_POLICY_EDF, INVALID_SCHEDULER_ERR);
	failures += test_scheduler("random", SCHED_POLICY_FIFO, INVALID_SCHEDULER_ERR);
	printf("\n=== parse_args tests ===\n");
	failures += test_valid_args();
	failures += test_parse_args(3, argv_bad_count, INVALID_ARGS_NUMBER);
	failures += test_parse_args(9, argv_bad_number, INVALID_NUMBER_ERR);
	failures += test_parse_args(9, argv_bad_overflow, NUMBER_OVERFLOW_ERR);
	failures += test_parse_args(9, argv_bad_scheduler, INVALID_SCHEDULER_ERR);
	failures += test_parse_args(9, argv_bad_config, INVALID_CONFIG_ERR);
	if (failures == 0)
		printf("ALL TESTS PASSED\n");
	else
		printf("%d TEST(S) FAILED\n", failures);
	return (failures != 0);
}
