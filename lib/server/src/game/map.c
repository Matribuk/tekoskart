/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** map
*/

#include <stdlib.h>
#include <stdio.h>
#include "map.h"

void push_front(struct vector_queue_head *head, point3D_t *point, vector3D_t *angle, bool primary)
{
    vector_queue_t *new_point = malloc(sizeof(vector_queue_t));
    new_point->point = point;
    new_point->angle = angle;
    new_point->primary_point = primary;
    TAILQ_INSERT_TAIL(head, new_point, entries);
}

void free_points(struct vector_queue_head *vector_head)
{
    vector_queue_t *vectors;
    while (!TAILQ_EMPTY(vector_head)) {
        vectors = TAILQ_FIRST(vector_head);
        TAILQ_REMOVE(vector_head, vectors, entries);
        if (vectors->point)
            free(vectors->point);
        if (vectors->angle)
            free(vectors->angle);
        free(vectors);
    }
}

point3D_t *create_point(double x, double y, double z)
{
    point3D_t *new = malloc(sizeof(point3D_t));
    new->x = x;
    new->y = y;
    new->z = z;
    return new;
}

angle3D_t *create_angle(double a, double b, double c)
{
    angle3D_t *new = malloc(sizeof(point3D_t));
    new->x = a;
    new->y = b;
    new->z = c;
    return new;
}

static void default_map(map_t *map)
{
    TAILQ_INIT(&map->vector_head);
    push_front(&map->vector_head, create_point(13, 1, 3), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(24, 9, 3), create_angle(0, 90, 80), true);
    push_front(&map->vector_head, create_point(24, 18, 3), create_angle(0, 90, 100), true);
    push_front(&map->vector_head, create_point(16, 23, 3), create_angle(0, 90, 195), true);
    push_front(&map->vector_head, create_point(6, 19, 3), create_angle(0, 90, 250), true);
    push_front(&map->vector_head, create_point(4, 4, 3), create_angle(0, 90, 280), true);
}

static void parse_map_file(map_t *map, FILE *file)
{
    char line[768];
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;

    free_points(&map->vector_head);
    TAILQ_INIT(&map->vector_head);
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#' && line[0] != '\n' &&
            sscanf(line, "%lf, %lf, %lf | %lf, %lf, %lf", &x, &y, &z, &a, &b, &c) == 6) {
            point3D_t *v1 = malloc(sizeof(point3D_t));
            angle3D_t *v2 = malloc(sizeof(angle3D_t));
            if (!v1 || !v2)
                continue;
            v1->x = x;
            v1->y = y;
            v1->z = z;
            v2->x = a;
            v2->y = b;
            v2->z = c;
            push_front(&map->vector_head, v1, v2, true);
        }
    }
    fclose(file);
}

map_t *load_map(const char *filename)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map)
        return NULL;
    default_map(map);
    FILE *file = fopen(filename, "r");
    if (!file)
        return map;

    parse_map_file(map, file);
    float k = 0.1;
    create_linear_trace(map, k);
    return map;
}

void free_map(map_t *map)
{
    free_points(&map->vector_head);
    free(map);
}
