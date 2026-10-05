#include "codexion.h"

int test_number(const char *str, int expected_value,
     int expected_result)
{
    int     value;
    int     result;

    value = -1;
    result = parse_number(str, &value);
    if (result != expected_result)
    {
        printf("FAIL: parse_number(\"%s\") returned %d, expected %d\n",
            str, result, expected_result);
        return (1);
    }
    if (expected_result == 0 && value != expected_value)
    {
        printf("FAIL: parse_number(\"%s\") gave %d, expected %d\n",
            str, value, expected_value);
        return (1);
    }
    return (0);
}

int test_scheduler(const char *str, t_scheduler expected,
        int expected_result)
{
    t_scheduler scheduler;
    int         result;

    scheduler = -1;
    result = parse_scheduler(str, &scheduler);
    if (result != expected_result)
    {
        printf("FAIL: parse_scheduler(\"%s\") returned %d, expected %d\n",
            str, result, expected_result);
        return (1);
    }
    if (expected_result == 0 && scheduler != expected)
    {
        printf("FAIL: wrong scheduler for \"%s\"\n", str);
        return (1);
    }
    return (0);
}

int main(void)
{
    int failures;

    failures = 0;

    printf("=== parse_number tests ===\n");
    failures += test_number("0", 0, 0);
    failures += test_number("1", 1, 0);
    failures += test_number("42", 42, 0);
    failures += test_number("123456789", 123456789, 0);
    failures += test_number("2147483646", INT_MAX - 1, 0);
    failures += test_number("2147483647", INT_MAX, 0);

    failures += test_number("", 0, 1);
    failures += test_number("-42", 0, 1);
    failures += test_number("+42", 0, 1);
    failures += test_number("42abc", 0, 1);
    failures += test_number("abc42", 0, 1);
    failures += test_number("12.5", 0, 1);
    failures += test_number(" 42", 0, 1);
    failures += test_number("42 ", 0, 1);
    failures += test_number("2147483648", 0, 1);
    failures += test_number("999999999999999999999", 0, 1);

    printf("=== parse_scheduler tests ===\n");
    failures += test_scheduler("fifo", SCHED_POLICY_FIFO, 0);
    failures += test_scheduler("edf", SCHED_POLICY_EDF, 0);

    failures += test_scheduler("", SCHED_POLICY_FIFO, 1);
    failures += test_scheduler("FIFO", SCHED_POLICY_FIFO, 1);
    failures += test_scheduler("EDF", SCHED_POLICY_EDF, 1);
    failures += test_scheduler("random", SCHED_POLICY_FIFO, 1);

    if (failures == 0)
        printf("ALL TESTS PASSED\n");
    else
        printf("%d TEST(S) FAILED\n", failures);
    return (failures != 0);
}