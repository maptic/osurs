/**
 * @brief IO helper and utils methods.
 * @file utils.c
 * @date: 2022-07-19
 * @author: Merlin Unterfinger
 */

#include <stdio.h>

#include "utils.h"

int compose_time(char *buffer, int time) {
    int h, m, s;
    h = (time / 3600);
    m = (time - (3600 * h)) / 60;
    s = (time - (3600 * h) - (m * 60));
    sprintf(buffer, "%.2d:%.2d:%.2d", h, m, s);
    return 0;
}

int parse_time(char *time) {
    int h, m, s;
    sscanf(time, "%d:%d:%d", &h, &m, &s);
    return h * 3600 + m * 60 + s;
}
