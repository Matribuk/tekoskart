/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** map
*/

#pragma once

#include <sys/queue.h>
#include <stdbool.h>

typedef struct vector3D {
    float x, y, z;
} vector3D_t;

typedef vector3D_t angle3D_t;
typedef vector3D_t point3D_t;

typedef struct vector_queue {
    point3D_t *point;
    angle3D_t *angle;
    bool primary_point;
    TAILQ_ENTRY(vector_queue) entries;
} vector_queue_t;

TAILQ_HEAD(vector_queue_head, vector_queue);

typedef struct map {
    struct vector_queue_head vector_head;
} map_t;

void create_linear_trace(map_t *map, float k);
map_t *load_map(const char *filename);
void free_map(map_t *map);