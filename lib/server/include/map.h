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
typedef vector3D_t vec3;

typedef struct vector_queue {
    point3D_t *point;
    point3D_t *tangent;
    point3D_t *angles;
    bool primary_point;
    TAILQ_ENTRY(vector_queue) entries;
} vector_queue_t;

TAILQ_HEAD(vector_queue_head, vector_queue);

typedef struct map {
    struct vector_queue_head vector_head;
} map_t;

float euclidean_distance(point3D_t p1, point3D_t p2);
float angle_difference(angle3D_t a1, angle3D_t a2);
int calculate_resolution(point3D_t p1, point3D_t p2, angle3D_t a1, angle3D_t a2, float k);
void create_hermite_trace(map_t *map);
void add_point_race(map_t *map);
map_t *load_map(const char *filename);
void free_map(map_t *map);