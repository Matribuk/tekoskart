/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** server
*/

#include "game.h"
#include "misc.h"

void run_engine(engine_t *engine)
{
    race_t *race = init_race(engine);
    if (!race) return;
    //while (engine->exit_code != 84);
    clear_race(race);
    return;
}

void free_engine(engine_t *engine)
{
    free_config(engine->config);
    free(engine);
}