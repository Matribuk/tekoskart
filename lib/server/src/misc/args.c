/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** args
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "misc.h"

static void default_config(args_t *config)
{
    config->port = 12345;
    config->password = strdup("1234");
    config->max_connection = 16;
    config->racers = 8;
    config->mode = grandprix;
    config->map_name = strdup("map.csv");
}

static void parse_config_file(args_t *config, FILE *file)
{
    char line[256];
    char key[128];
    char value[128];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#' && line[0] != '\n' &&
            sscanf(line, "%127s = %127[^\n]", key, value) == 2) {
            if (strcmp(key, "port") == 0)
                config->port = (size_t)atoi(value);
            else if (strcmp(key, "password") == 0) {
                free(config->password);
                config->password = strdup(value);
            } else if (strcmp(key, "map") == 0) {
                free(config->map_name);
                config->map_name = strdup(value);
            } else if (strcmp(key, "max_connection") == 0)
                config->max_connection = (size_t)atoi(value);
            else if (strcmp(key, "racers") == 0)
                config->racers = (size_t)atoi(value);
            else if (strcmp(key, "mode") == 0)
                config->mode = (gamemode)atoi(value);
            else if (strcmp(key, "resolution") == 0)
                config->resolution = (int)atoi(value);
        }
    }
    fclose(file);
}

args_t *load_config(const char *filename)
{
    args_t *config = malloc(sizeof(args_t));
    if (!config)
        return NULL;

    default_config(config);
    FILE *file = fopen(filename, "r");
    if (!file)
        return config;

    parse_config_file(config, file);
    return config;
}

void free_config(args_t *config)
{
    if (config) {
        free(config->password);
        free(config->map_name);
        free(config);
    }
}
