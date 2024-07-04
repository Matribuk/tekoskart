/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** server
*/

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <pthread.h>
#include <time.h>

typedef struct args args_t;
typedef void (*apply_func_t)(void);

typedef enum {
    OBJECT,
    PLAYER
} entity_type_t;

typedef enum {
    STUN,
    GHOST,
    STAR,
    BILL,
    CRUSH,
    MUSHROOM,
    BOOST,
    PLANE,
    AEROSLIDE,
    INVISIBLE,
    NUMBER_MAX_STATUS
} effect_t;

typedef struct vector {
    double x;
    double y;
    double z;
} vector_t;

typedef struct status {
    bool active;
    apply_func_t apply_func;
    effect_t effect;
    time_t start;
    size_t duration;
} status_t;

typedef struct entity {
    entity_type_t type;
    vector_t pos;
    bool hittable;
    status_t **status;
    int id;
} entity_t;

typedef struct entities_queue {
    entity_t *entitie;
    TAILQ_ENTRY(entities_queue) entries;
} entities_queue_t;

TAILQ_HEAD(entities_queue_head, entities_queue);

typedef struct race {
    size_t len;
    entity_t **entities;
    pthread_mutex_t locker;
    struct entities_queue_head dyn_obj_head;
} race_t;


typedef struct engine {
    race_t *race;
    args_t *config;
    int exit_code;
} engine_t;

typedef enum {
    grandprix = 0,
    versus = 1,
    arcade = 2
} gamemode;

race_t *init_race(engine_t *engine);
void run_engine(engine_t *engine);
void free_engine(engine_t *engine);
void clear_race(race_t *race);