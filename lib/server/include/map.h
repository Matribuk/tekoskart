/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** map
*/

#pragma once

#include <sys/queue.h>

typedef struct vector3D {
    float x, y, z;
} vector3D_t;

typedef vector3D_t angle3D_t;
typedef vector3D_t point3D_t;

typedef struct vector_queue {
    point3D_t *point;
    angle3D_t *angle;
    TAILQ_ENTRY(vector_queue) entries;
} vector_queue_t;

TAILQ_HEAD(vector_queue_head, vector_queue);

typedef struct map {
    struct vector_queue_head vector_head;
} map_t;

map_t *load_map(const char *filename);
void free_map(map_t *map);