/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** race
*/

#include "game.h"
#include "misc.h"

race_t *init_race(engine_t *engine)
{
    race_t *race = malloc(sizeof(race_t));
    race->len = engine->config->max_connection;
    race->entities = malloc(sizeof(entity_t) * engine->config->max_connection);
    for (size_t index = 0; index < engine->config->max_connection; index++) {
        race->entities[index] = malloc(sizeof(entity_t));
        race->entities[index]->status = NULL;
    }
    TAILQ_INIT(&race->dyn_obj_head);
    return race;
}

static void free_dyn_obj(struct entities_queue_head *dyn_obj_head)
{
    entities_queue_t *obj;
    while (!TAILQ_EMPTY(dyn_obj_head)) {
        obj = TAILQ_FIRST(dyn_obj_head);
        TAILQ_REMOVE(dyn_obj_head, obj, entries);
        if (obj)
            free(obj->entitie);
        free(obj);
    }
}

static void free_status(entity_t *entity)
{
    if (!entity->status) return;
    for (size_t index = 0; index < NUMBER_MAX_STATUS; index++)
        free(entity->status[index]);
}

static void free_entity(race_t *race)
{
    if (!race->entities) return;
    for (size_t index = 0; index < race->len; index++) {
        free_status(race->entities[index]);
        free(race->entities[index]);
    }
    free(race->entities);
    race->len = 0;
}

void clear_race(race_t *race)
{
    free_entity(race);
    free_dyn_obj(&race->dyn_obj_head);
    free(race);
}

