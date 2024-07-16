#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"

#define DEG_TO_RAD(angle) ((angle) * M_PI / 180.0)

double h00(double t) { return 2*t*t*t - 3*t*t + 1; }
double h10(double t) { return t*t*t - 2*t*t + t; }
double h01(double t) { return -2*t*t*t + 3*t*t; }
double h11(double t) { return t*t*t - t*t; }

double dh00(double t) { return 6*t*t - 6*t; }
double dh10(double t) { return 3*t*t - 4*t + 1; }
double dh01(double t) { return -6*t*t + 6*t; }
double dh11(double t) { return 3*t*t - 2*t; }

double vector_norm(vec3 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}


static int is_almost_zero(double x)
{
    return fabs(x) < 1e-12;
}

static vec3 vector_difference(vec3 v1, vec3 v2)
{
    vec3 difference;
    difference.x = v1.x - v2.x;
    difference.y = v1.y - v2.y;
    difference.z = v1.z - v2.z;
    return difference;
}

static vec3 vector_add(vec3 v1, vec3 v2)
{
    vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

static vec3 vector_divide(vec3 v1, double n)
{
    vec3 result;
    result.x = v1.x / n;
    result.y = v1.y / n;
    result.z = v1.z / n;
    return result;
}

static vec3 vector_mul(vec3 v1, double n)
{
    vec3 result;
    result.x = v1.x * n;
    result.y = v1.y * n;
    result.z = v1.z * n;
    return result;
}

float adjustAngle(float angle) {
    return (angle + ((angle < 0 ? -1 : 1) * DEG_TO_RAD(-90)));
}

vec3 calculate_3d_angles(vec3 tangent)
{
    vec3 angles;
    //double correctorx = -1 * DEG_TO_RAD(-90);
    //double correctory = -1 * DEG_TO_RAD(-90);
    //double correctorz = -1 * DEG_TO_RAD(-90);

    //if (tangent.y < 0.0f && tangent.z < 0.0f)
    //    correctorx *= -1;
    //if (tangent.x < 0.0f && tangent.z < 0.0f)
    //    correctory *= -1;
    //if (tangent.y < 0.0f && tangent.x < 0.0f)
    //    correctorz *= -1;


    angles.x = atan2(tangent.y, tangent.z);
    angles.y = atan2(tangent.x, tangent.z);
    angles.z = atan2(tangent.y, tangent.x);


    //angles = vector_difference(angles, );
    return angles;
}

vec3 hermite_point(double t, vec3 P1, vec3 T1, vec3 P2, vec3 T2)
{
    vec3 point;
    point.x = h00(t) * P1.x + h10(t) * T1.x + h01(t) * P2.x + h11(t) * T2.x;
    point.y = h00(t) * P1.y + h10(t) * T1.y + h01(t) * P2.y + h11(t) * T2.y;
    point.z = h00(t) * P1.z + h10(t) * T1.z + h01(t) * P2.z + h11(t) * T2.z;
    return point;
}

vec3 hermite_tangent(double t, vec3 P1, vec3 T1, vec3 P2, vec3 T2)
{
    vec3 tangent;
    tangent.x = dh00(t) * P1.x + dh10(t) * T1.x + dh01(t) * P2.x + dh11(t) * T2.x;
    tangent.y = dh00(t) * P1.y + dh10(t) * T1.y + dh01(t) * P2.y + dh11(t) * T2.y;
    tangent.z = dh00(t) * P1.z + dh10(t) * T1.z + dh01(t) * P2.z + dh11(t) * T2.z;

    if (is_almost_zero(tangent.x)) tangent.x = 0.0;
    if (is_almost_zero(tangent.y)) tangent.y = 0.0;
    if (is_almost_zero(tangent.z)) tangent.z = 0.0;
    return tangent;
}

void insert_point_in_queue(struct vector_queue_head *head, point3D_t *point, angle3D_t *tangent, angle3D_t *angles, bool primary_point)
{
    vector_queue_t *new_node = malloc(sizeof(vector_queue_t));
    new_node->point = malloc(sizeof(point3D_t));
    *(new_node->point) = *point;
    new_node->tangent = malloc(sizeof(angle3D_t));
    *(new_node->tangent) = *tangent;
    new_node->angles = malloc(sizeof(angle3D_t));
    *(new_node->angles) = *angles;
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
        vec3 newAngles = calculate_3d_angles(newTangent);
        insert_point_in_queue(head, (point3D_t *)&newPoint, (angle3D_t *)&newTangent, (angle3D_t *)&newAngles, (t == 0.0f) ? true : false);
    }
}

void create_hermite_trace(map_t *map)
{
    struct vector_queue_head *head = &map->vector_head;
    if (head == NULL || TAILQ_EMPTY(head) || TAILQ_FIRST(head) == TAILQ_LAST(head, vector_queue_head))
        return;

    vector_queue_t *current;
    vector_queue_t *next;
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
