/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** map
*/

#include <stdlib.h>
#include <stdio.h>
#include "map.h"

void push_front(struct vector_queue_head *head, point3D_t *point, vector3D_t *tangent, point3D_t *angles, bool primary)
{
    vector_queue_t *new_point = malloc(sizeof(vector_queue_t));
    new_point->point = point;
    new_point->tangent = tangent;
    new_point->primary_point = primary;
    new_point->angles = angles;
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
        if (vectors->tangent)
            free(vectors->tangent);
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

point3D_t *create_tangent(double a, double b, double c)
{
    point3D_t *new = malloc(sizeof(point3D_t));
    new->x = a;
    new->y = b;
    new->z = c;
    return new;
}

point3D_t *create_angle(double a, double b, double c)
{
    point3D_t *new = malloc(sizeof(point3D_t));
    new->x = a;
    new->y = b;
    new->z = c;
    return new;
}

static void default_map(map_t *map)
{
    TAILQ_INIT(&map->vector_head);
    push_front(&map->vector_head, create_point(13, 1, 15), create_tangent(0, 0, -10), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(18, 4, 10), create_tangent(10, 0, 0), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(24, 4, 15), create_tangent(0, 0, 10), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(18, 4, 20), create_tangent(-10, 0, 0), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(18, 7, 10), create_tangent(10, 0, 0), create_angle(0, 90, 0), true);
    push_front(&map->vector_head, create_point(24, 9, 15), create_tangent(5, 8, 0), create_angle(0, 0, 0), true);
    push_front(&map->vector_head, create_point(24, 18, 15), create_tangent(-5, 5, 0), create_angle(0, -90, 0), true);
    push_front(&map->vector_head, create_point(16, 23, 15), create_tangent(-10, 5, 0), create_angle(0, -90, 0), true);
    push_front(&map->vector_head, create_point(6, 19, 3), create_tangent(-15, -10, 0), create_angle(0, -90, 0), true);
    push_front(&map->vector_head, create_point(4, 4, 3), create_tangent(10, -19, 0), create_angle(0, -270, 0), true);
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
    double a2 = 0.0;
    double b2 = 0.0;
    double c2 = 0.0;


    free_points(&map->vector_head);
    TAILQ_INIT(&map->vector_head);
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#' && line[0] != '\n' &&
            sscanf(line, "%lf, %lf, %lf | %lf, %lf, %lf | %lf, %lf, %lf", &x, &y, &z, &a, &b, &c, &a2, &b2, &c2) == 9) {
            point3D_t *v1 = malloc(sizeof(point3D_t));
            angle3D_t *v2 = malloc(sizeof(angle3D_t));
            angle3D_t *v3 = malloc(sizeof(angle3D_t));
            if (!v1 || !v2 || !v3)
                continue;
            v1->x = x;
            v1->y = y;
            v1->z = z;
            v2->x = a;
            v2->y = b;
            v2->z = c;
            v3->x = a2;
            v3->y = b2;
            v3->z = c2;
            push_front(&map->vector_head, v1, v2, v3, true);
        }
    }
    fclose(file);
}

map_t *load_map(const char *filename, int resolution)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map)
        return NULL;
    default_map(map);
    FILE *file = fopen(filename, "r");
    if (!file)
        return map;

    parse_map_file(map, file);
    float k = 1;
    create_hermite_trace(map, resolution);
    return map;
}

void free_map(map_t *map)
{
    free_points(&map->vector_head);
    free(map);
}
