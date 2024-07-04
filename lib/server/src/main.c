/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** main
*/

#include <stdio.h>
#include "misc.h"
#include "game.h"
#include "map.h"

void print_config(args_t *config)
{
    printf("Port: %zu\n", config->port);
    printf("Password: %s\n", config->password);
    printf("Max Connections: %zu\n", config->max_connection);
    printf("Racers: %zu\n", config->racers);
    printf("Mode: %d\n", config->mode);
}

void print_coord(struct vector_queue_head *head) {
    vector_queue_t *current;
    size_t indexer = 1;

    TAILQ_FOREACH(current, head, entries) {
        if (current->primary_point) {
            printf("Coord / Angles P%zu :\n", indexer);
            printf(GREEN);
            indexer ++;
        } else
            printf(RED);
        printf("\tPoints: x = %.1lf, y = %.1lf, z = %.1lf\n",
               current->point->x,
               current->point->y,
               current->point->z);
        printf("\tAngle: a = %.1lf, b = %.1lf, c = %.1lf\n",
               current->angle->x,
               current->angle->y,
               current->angle->z);
        printf(RESET);
    }
}

engine_t *configure_engine(args_t *config)
{
    engine_t *engine = malloc(sizeof(engine_t));
    engine->config = config;
    engine->race = NULL;
    engine->exit_code = 0;
    return engine;
}

int main(int ac, char **av)
{
    engine_t *engine;
    args_t *config = load_config("server.cfg");
    int exit_code = 0;

    if (config) {
        map_t *map = load_map(config->map_name);
        if (!map) {
            free_config(config);
            return EXIT_FAILURE;
        }
        print_coord(&map->vector_head);
        engine = configure_engine(config);
        print_config(engine->config);
        run_engine(engine);
        exit_code = engine->exit_code;
        free_engine(engine);
        free_map(map);
    } else
        warn("Failed to load configuration.\n");
    return exit_code;
}
