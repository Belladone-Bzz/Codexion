#include "codexion.h"

int parse_number(const char *str, int *value)
{
    int       index;
    int       digit;
    int       result;

    if (str == NULL || *str == '\0' || value == NULL)
    {
        printf("Argument error: empty numeric argument.\n");
        return (1);
    }
    index = 0;
    result = 0;
    while (str[index] != '\0')
    {
        if (str[index] < '0' || str[index] > '9')
        {
            printf("Argument error: You must only provide digit characters.\n");
            return (1);
        }
        digit = str[index] - '0';
        if (result > INT_MAX / 10 
            || (result == INT_MAX / 10 && digit > INT_MAX % 10))
        {
            printf("Argument error: numeric argument exceeds INT_MAX.\n");
            return (1);
        }
        result = result * 10 + digit;
        index++;
    }
    *value = result;
    return (0);
}

int parse_scheduler(const char *str, t_scheduler *scheduler)
{
    if (str == NULL || *str == '\0' || scheduler == NULL)
    {
        printf("Argument error: empty scheduler argument.\n");
        return (1);
    }
    if (strcmp(str, "fifo") == 0)
    {
        *scheduler = SCHED_POLICY_FIFO;
        return (0);
    }
     if (strcmp(str, "edf") == 0)
    {
        *scheduler = SCHED_POLICY_EDF;
        return (0);
    }
    printf("Argument error. Your scheduler can only be <fifo> or <edf>.\n");
    return (1);
}

int validate_config(t_config *config)
{
    if (config->number_of_coders < 1 || config->number_of_coders > 10000)
    {
        printf("Validation error. number_of_coders must be between 1 and 10000.\n");
        return (1);
    }
    if (config->time_to_burnout < 0 || config->time_to_compile < 0
        || config->time_to_debug < 0 || config->time_to_refactor < 0
        || config->compiles_required < 0 || config->dongle_cooldown < 0)
    {    
        printf("Validation error. timing and compilation arguments must be >= 0.\n");
        return (1);
    }
    return (0);
}

int parse_args(int argc, char **argv, t_config *config)
{
    int     *values[7];
    int     index;

    if (argc != 9 || argv == NULL || config == NULL)
    {
        printf("Argument error: invalid argument count.\n");
        return (1);
    }
    values[0] = &config->number_of_coders;
    values[1] = &config->time_to_burnout;
    values[2] = &config->time_to_compile;
    values[3] = &config->time_to_debug;
    values[4] = &config->time_to_refactor;
    values[5] = &config->compiles_required;
    values[6] = &config->dongle_cooldown;
    index = 0;
    while (index < 7)
    {
        if (parse_number(argv[index + 1], values[index]) != 0)
            return (1);
        index++;
    }
    if (parse_scheduler(argv[8], &config->scheduler) != 0)
        return (1);
    return (0);
}
