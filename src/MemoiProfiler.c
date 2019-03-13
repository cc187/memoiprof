//
// Created by pedro on 11-11-2016.
//

#include "MemoiProfiler.h"
#include "MemoiProfilerUtils.h"
#include "MemoiUtils.h"
#include "MemoiRecord.h"
#include "cJSON.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdarg.h>


struct mp_t {

    char *name;

    CType output_type;
    CType input_type;

    GHashTable *table;

    unsigned int calls;
    unsigned int hits;
};


MemoiProf *mp_init(const char *name, CType type) {

    MemoiProf *mp = malloc(sizeof *mp);

    size_t name_size = strlen(name) + 1;
    mp->name = calloc(name_size, sizeof *(mp->name));
    strcpy(mp->name, name);

    mp->input_type = type;
    mp->output_type = type;

    mp->calls = 0;
    mp->hits = 0;

    switch (type) {

        case FLOAT:
            mp->table = g_hash_table_new_full(memoi_float_hash, memoi_float_equal, g_free, g_free);
            break;
        case DOUBLE:
            mp->table = g_hash_table_new_full(g_double_hash, g_double_equal, g_free, g_free);
            break;
        case INT:
            mp->table = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_free);
            break;
    }

    return mp;
}


MemoiProf *mp_destroy(MemoiProf *mp) {

    if (mp != NULL) {

        free(mp->name);
        g_hash_table_destroy(mp->table);
        free(mp);
    }

    return NULL;
}


void mp_inc(MemoiProf *mp, void *input, void *output) {

    if (mp == NULL) {
        return;
    }

    mp->calls++;

    void *mr = g_hash_table_lookup(mp->table, input);

    if (mr != NULL) {

        mp->hits++;
        mr_inc_counter(mr);
    } else {

        uint64_t input_bits = mp_get_bits(mp, input);
        uint64_t output_bits = mp_get_bits(mp, output);
        MemoiRec *new_mr = mr_init(input_bits, 1, output_bits);

        void *key = mp_dup_input(mp, input);
        g_hash_table_insert(mp->table, key, new_mr);
    }
}


void mp_print(MemoiProf *mi) {

    printf("==================================================\n");
    printf("Table '%s', %u elements, %u calls (%uh, %um)\n\n", mi->name, g_hash_table_size(mi->table), mi->calls,
           mi->hits, mi->calls - mi->hits);


    g_hash_table_foreach(mi->table, mr_print, &(mi->input_type));

    printf("==================================================\n");
}


void mp_to_json(MemoiProf *mi, const char *filename) {

    cJSON* json_root = make_json_header(mi);

    /* counts array */
    cJSON *json_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "counts", json_array);
    g_hash_table_foreach(mi->table, mr_make_json, json_array);

    write_json_and_cleanup(filename, json_root);
}


CType mp_get_input_type(const MemoiProf *mp) {
    return mp->input_type;
}


const char* mp_get_name(const MemoiProf* mp) {
    return mp->name;
}


unsigned int mp_get_table_size(const MemoiProf* mp) {
    return g_hash_table_size(mp->table);
}


unsigned int mp_get_calls(const MemoiProf* mp) {
    return mp->calls;
}


unsigned int mp_get_hits(const MemoiProf* mp) {
    return mp->hits;
}


unsigned int mp_get_misses(const MemoiProf* mp) {
    return mp->calls - mp->hits;
}
