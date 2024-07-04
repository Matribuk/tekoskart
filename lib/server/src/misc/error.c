/*
** FIRST PERSONAL PROJECT, 2024
** TekosKart
** File description:
** error
*/

#include <stdio.h>
#include "misc.h"

void warn(const char *msg)
{
    perror(msg);
}

