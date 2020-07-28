//
// Created by pedro on 28/07/20.
//

#include "MemoiUtils.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

static char* time_stamp() {

    size_t length = 16;
    char* stamp = malloc(length);

    time_t current_time = time(NULL);
    struct tm * tm = localtime(&current_time);

    snprintf(stamp, length, "%d%02d%02d_%02d%02d%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    return stamp;
}

/**
 * Utility method to generate the name of a report file with a time stamp.
 *
 * Note: the user should free the pointer returned by this function once it's no longer needed.
 *
 * @param dir
 * @param id
 * @return
 */
char* mp_make_report_path(const char *dir, const char *id) {

    char * stamp = time_stamp();
    char * ext = ".json";
    char * sep = "/";
    char * under = "_";

    size_t stamp_len = strlen(stamp);
    size_t ext_len = strlen(ext);
    size_t sep_len = strlen(sep);
    size_t under_len = strlen(under);
    size_t dir_len = strlen(dir);
    size_t id_len = strlen(id);

    size_t total_len = stamp_len + ext_len + sep_len + under_len + dir_len + id_len + 1; // 1 is for the \0

    char * path = malloc(total_len);

    snprintf(path, total_len, "%s%s%s%s%s%s", dir, sep, stamp, under, id, ext);

    free(stamp);

    return path;
}
