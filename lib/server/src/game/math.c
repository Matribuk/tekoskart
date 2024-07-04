/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** math
*/

#include <stdlib.h>
#include <math.h>
#include "map.h"

float euclidean_distance(point3D_t p1, point3D_t p2)
{
    return sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2) + powf(p2.z - p1.z, 2));
}

float angle_difference(angle3D_t a1, angle3D_t a2)
{
    float diff_x = fabsf(a2.x - a1.x);
    float diff_y = fabsf(a2.y - a1.y);
    float diff_z = fabsf(a2.z - a1.z);
    return diff_x + diff_y + diff_z;
}

int calculate_resolution(point3D_t p1, point3D_t p2, angle3D_t a1, angle3D_t a2, float k)
{
    float distance = euclidean_distance(p1, p2);
    float angle_diff = angle_difference(a1, a2);
    return (int)ceilf(k * (distance + angle_diff));
}

vector3D_t interpolate_vector(vector3D_t v1, vector3D_t v2, float t)
{
    vector3D_t result;
    result.x = v1.x + t * (v2.x - v1.x);
    result.y = v1.y + t * (v2.y - v1.y);
    result.z = v1.z + t * (v2.z - v1.z);
    return result;
}

vector_queue_t interpolate_point(vector_queue_t p1, vector_queue_t p2, float t)
{
    vector_queue_t result;
    result.point = malloc(sizeof(point3D_t));
    result.angle = malloc(sizeof(angle3D_t));
    result.primary_point = false;
    *(result.point) = interpolate_vector(*(p1.point), *(p2.point), t);
    *(result.angle) = interpolate_vector(*(p1.angle), *(p2.angle), t);
    return result;
}

void create_linear_trace(map_t *map, float k)
{
    struct vector_queue_head *head = &map->vector_head;
    if (head == NULL || TAILQ_EMPTY(head))
        return;
    struct vector_queue_head linear_head;
    TAILQ_INIT(&linear_head);

    vector_queue_t *current, *next;
    TAILQ_FOREACH(current, head, entries) {
        next = TAILQ_NEXT(current, entries);
        if (!next) break;

        int resolution = calculate_resolution(*(current->point), *(next->point), *(current->angle), *(next->angle), k);
        for (int i = 0; i < resolution; ++i) {
            float t = (float)i / resolution;
            vector_queue_t new_point = interpolate_point(*current, *next, t);

            vector_queue_t *new_node = malloc(sizeof(vector_queue_t));
            new_node->point = new_point.point;
            new_node->angle = new_point.angle;
            new_node->primary_point = (i == 0) ? true : false;
            TAILQ_INSERT_TAIL(&linear_head, new_node, entries);
        }
    }

    vector_queue_t *last_point = malloc(sizeof(vector_queue_t));
    last_point->point = malloc(sizeof(point3D_t));
    last_point->angle = malloc(sizeof(angle3D_t));
    *(last_point->point) = *(TAILQ_LAST(head, vector_queue_head)->point);
    *(last_point->angle) = *(TAILQ_LAST(head, vector_queue_head)->angle);
    last_point->primary_point = TAILQ_LAST(head, vector_queue_head)->primary_point;
    TAILQ_INSERT_TAIL(&linear_head, last_point, entries);

    while ((current = TAILQ_FIRST(head)) != NULL) {
        TAILQ_REMOVE(head, current, entries);
        free(current->point);
        free(current->angle);
        free(current);
    }
    TAILQ_CONCAT(head, &linear_head, entries);
}
