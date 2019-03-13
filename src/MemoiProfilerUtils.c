//
// Created by pedro on 13-03-2019.
//

#include "MemoiProfilerUtils.h"

#include <stdio.h>
#include <stdlib.h>

/**
 *      Duplicates the given input. Allocates memory for a new variable of the same type, copies the content and
 *  returns the new value.
 *
 * @param mp
 * @param input
 * @return a pointer with a copy of the input
 */
void *mp_dup_input(MemoiProf *mp, void *input) {

    float *new_float;
    double *new_double;
    int *new_int;

    switch (mp_get_input_type(mp)) {

        case FLOAT:
            new_float = malloc(sizeof *new_float);
            *new_float = *((float *) input);
            return new_float;

        case DOUBLE:
            new_double = malloc(sizeof *new_double);\
            *new_double = *((double *) input);
            return new_double;

        case INT:
            new_int = malloc(sizeof *new_int);\
            *new_int = *((int *) input);
            return new_int;
    }
    return NULL;
}


cJSON * make_json_header(const MemoiProf *mp) {

    cJSON *json_root = cJSON_CreateObject();

    /* table information */
    cJSON_AddStringToObject(json_root, "id", mp_get_id(mp));
    cJSON_AddStringToObject(json_root, "func_name", mp_get_func_name(mp));
    cJSON_AddNumberToObject(json_root, "elements", mp_get_table_size(mp));
    cJSON_AddNumberToObject(json_root, "calls", mp_get_calls(mp));
    cJSON_AddNumberToObject(json_root, "hits", mp_get_hits(mp));
    cJSON_AddNumberToObject(json_root, "misses", mp_get_misses(mp));

    cJSON *call_sites_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "call_sites", call_sites_array);

    unsigned int call_site_count = mp_get_call_site_count(mp);
    const char** call_sites = mp_get_call_sites(mp);
    for (unsigned int i = 0; i < call_site_count; ++i) {

        cJSON_AddItemToArray(call_sites_array, cJSON_CreateString(call_sites[i]));
    }

    return json_root;
}


void write_json_and_cleanup(const char *filename, cJSON *json_root) {

    FILE *f = fopen(filename, "w");
    if (f == NULL) {

        printf("Error opening file: %s\n", filename);
        return;
    }

    char *output = cJSON_Print(json_root);
    fprintf(f, "%s", output);

    free(output);
    cJSON_Delete(json_root);
    fclose(f);
}

unsigned int memoi_float_hash(const void *key) {

    return *(const uint32_t *) key;
}

int memoi_float_equal(const void *a, const void *b) {

    return *(const float *) a == *(const float *) b;
}


/**
 *      WE ARE ASSUMING THIS IS EQUAL TO THE RESULT RETURND BY THE HASH FUNCTION!
 * @param mp
 * @param value
 * @return
 */
uint64_t mp_get_bits(MemoiProf *mp, void *value) {

    uint64_t bits64;
    uint32_t bits32;

    switch (mp_get_input_type(mp)) {
        case FLOAT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0xFFFFFFFF;

        case DOUBLE:

            bits64 = *(uint64_t *) value;
            return bits64;

        case INT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0xFFFFFFFF;
    }

    return 0u;
}