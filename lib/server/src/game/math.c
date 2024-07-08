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

