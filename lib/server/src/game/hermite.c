/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** hermite
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"

double h00(double t) { return 2*t*t*t - 3*t*t + 1; }
double h10(double t) { return t*t*t - 2*t*t + t; }
double h01(double t) { return -2*t*t*t + 3*t*t; }
double h11(double t) { return t*t*t - t*t; }

double dh00(double t) { return 6*t*t - 6*t; }
double dh10(double t) { return 3*t*t - 4*t + 1; }
double dh01(double t) { return -6*t*t + 6*t; }
double dh11(double t) { return 3*t*t - 2*t; }

vec3 hermite_point(double t, vec3 P1, vec3 T1, vec3 P2, vec3 T2) {
    vec3 point;
    point.x = h00(t) * P1.x + h10(t) * T1.x + h01(t) * P2.x + h11(t) * T2.x;
    point.y = h00(t) * P1.y + h10(t) * T1.y + h01(t) * P2.y + h11(t) * T2.y;
    point.z = h00(t) * P1.z + h10(t) * T1.z + h01(t) * P2.z + h11(t) * T2.z;
    return point;
}

vec3 hermite_tangent(double t, vec3 P1, vec3 T1, vec3 P2, vec3 T2) {
    vec3 tangent;
    tangent.x = dh00(t) * P1.x + dh10(t) * T1.x + dh01(t) * P2.x + dh11(t) * T2.x;
    tangent.y = dh00(t) * P1.y + dh10(t) * T1.y + dh01(t) * P2.y + dh11(t) * T2.y;
    tangent.z = dh00(t) * P1.z + dh10(t) * T1.z + dh01(t) * P2.z + dh11(t) * T2.z;
    return tangent;
}

void insert_point_in_queue(struct vector_queue_head *head, point3D_t *point, angle3D_t *tangent, bool primary_point)
{
    vector_queue_t *new_node = malloc(sizeof(vector_queue_t));
    new_node->point = malloc(sizeof(point3D_t));
    *(new_node->point) = *point;
    new_node->tangent = malloc(sizeof(angle3D_t));
    *(new_node->tangent) = *tangent;
    new_node->primary_point = primary_point;
    TAILQ_INSERT_TAIL(head, new_node, entries);
}

void hermiteCurve(vec3 P1, vec3 P2, vec3 T1, vec3 T2, struct vector_queue_head *head)
{
    int num_points = 100;

    for (int i = 0; i < num_points; ++i) {
        float t = (float)i / (num_points - 1);
        vec3 newPoint = hermite_point(t, P1, T1, P2, T2);
        vec3 newTangent = hermite_tangent(t, P1, T1, P2, T2);
        insert_point_in_queue(head, (point3D_t *)&newPoint, (angle3D_t *)&newTangent, (t == 0.0f) ? true : false);
    }
}

void create_hermite_trace(map_t *map)
{
    struct vector_queue_head *head = &map->vector_head;
    if (head == NULL || TAILQ_EMPTY(head) || TAILQ_FIRST(head) == TAILQ_LAST(head, vector_queue_head))
        return;

    vector_queue_t *first, *second, *current, *next, *second_next;
    first = TAILQ_FIRST(head);
    second = TAILQ_NEXT(first, entries);
    if (!second || TAILQ_NEXT(second, entries) == NULL) {
        printf("La liste doit contenir au moins 3 points.\n");
        return;
    }
    struct vector_queue_head hermite_head;
    TAILQ_INIT(&hermite_head);

    TAILQ_FOREACH(current, head, entries) {
        next = TAILQ_NEXT(current, entries);
        if (!next)
            next = TAILQ_FIRST(head);
        vec3 P0 = *(vec3 *)current->point;
        vec3 T0 = *(vec3 *)current->tangent;
        vec3 P1 = *(vec3 *)next->point;
        vec3 T1 = *(vec3 *)next->tangent;
        hermiteCurve(P0, P1, T0, T1, &hermite_head);
    }

    while ((current = TAILQ_FIRST(head)) != NULL) {
        TAILQ_REMOVE(head, current, entries);
        free(current->point);
        free(current->tangent);
        free(current);
    }
    TAILQ_CONCAT(head, &hermite_head, entries);
}
