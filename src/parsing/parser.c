#include <stddef.h>
#include <limits.h>
#include <string.h>

int parse_number(const char *str, long long *value)
{
    int             index;
    int             digit;
    long long       result;

    if (str == NULL || *str == '\0' || value == NULL)
        return (1);
    index = 0;
    result = 0;
    while (str[index] != '\0')
    {
        if (str[index] < '0' || str[index] > '9')
            return (1);
        digit = str[index] - '0';
        if (result > LLONG_MAX / 10 
            || (result == LLONG_MAX / 10 && digit > LLONG_MAX % 10))
            return (1);
        result = result * 10 + digit;
        index++;
    }
    *value = result;
    return (0);
}

int parse_scheduler(const char *str, char *scheduler)
{
    if (strcmp(*str, "fifo") == 0 || strcmp(*str, "edf") == 0)
    {
        *scheduler = *str;
        return (0);
    }
    else
        return (1);
}