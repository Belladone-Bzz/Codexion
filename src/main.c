#include "codexion.h"

int main(int argc, char **argv)
{
    t_config config;

    if (parse_args(argc, argv, &config) != 0)
        return (1);
    if (validate_config(&config) != 0)
        return (1);
    return (0);
}