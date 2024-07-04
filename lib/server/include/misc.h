/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** misc
*/

#pragma once

#include <stdlib.h>
#include "game.h"

typedef struct engine engine_t;
typedef struct args {
    size_t port;
    char *password;
    size_t max_connection;
    size_t racers;
    char *map_name;
    gamemode mode;
} args_t;

args_t *load_config(const char *filename);
void warn(const char *msg);
void free_config(args_t *config);