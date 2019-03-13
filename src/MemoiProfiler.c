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

    char *id;
    char *func_name;

    CType output_type;
    CType input_type;

    GHashTable *table;

    unsigned int calls;
    unsigned int hits;

    unsigned int call_site_count;
    const char** call_sites;
};


MemoiProf *mp_init(const char *func_name, const char *id, CType type) {

    MemoiProf *mp = malloc(sizeof *mp);

    size_t name_size = strlen(func_name);
    mp->func_name = calloc(name_size, sizeof *(mp->func_name));
    strcpy(mp->func_name, func_name);

    size_t id_size = strlen(id) + 1;
    mp->id = calloc(id_size, sizeof *(mp->id));
    strcpy(mp->id, id);

    mp->input_type = type;
    mp->output_type = type;

    mp->calls = 0;
    mp->hits = 0;

    mp->call_site_count = 0;

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

        free(mp->id);
        free(mp->func_name);
        g_hash_table_destroy(mp->table);
        free(mp->call_sites);
        free(mp);
    }

    return NULL;
}


void mp_inc(MemoiProf *mp, void *input, void *output) {

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


void mp_print(MemoiProf *mp) {

    printf("==================================================\n");
    printf("Table '%s', function '%s', %u elements, %u calls (%uh, %um)\n\n", mp->id, mp->func_name, g_hash_table_size(mp->table), mp->calls,
           mp->hits, mp->calls - mp->hits);


    g_hash_table_foreach(mp->table, mr_print, &(mp->input_type));

    printf("==================================================\n");
}


void mp_to_json(MemoiProf *mp, const char *filename) {

    cJSON *json_root = make_json_header(mp);

    /* counts array */
    cJSON *json_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "counts", json_array);
    g_hash_table_foreach(mp->table, mr_make_json, json_array);

    write_json_and_cleanup(filename, json_root);
}


void mp_add_call_sites(MemoiProf *mp, unsigned int count, ...) {

    va_list ap;
    va_start(ap, count);

    mp->call_site_count = count;
    mp->call_sites = malloc(count * sizeof *(mp->call_sites));

    for (unsigned int i = 0; i < count; ++i) {

        mp->call_sites[i] = va_arg(ap, const char *);
    }

    va_end(ap);
}


CType mp_get_input_type(const MemoiProf *mp) {
    return mp->input_type;
}


const char *mp_get_id(const MemoiProf *mp) {
    return mp->id;
}


const char *mp_get_func_name(const MemoiProf *mp) {
    return mp->func_name;
}


unsigned int mp_get_table_size(const MemoiProf *mp) {
    return g_hash_table_size(mp->table);
}


unsigned int mp_get_calls(const MemoiProf *mp) {
    return mp->calls;
}


unsigned int mp_get_hits(const MemoiProf *mp) {
    return mp->hits;
}

unsigned int mp_get_misses(const MemoiProf *mp) {
    return mp->calls - mp->hits;
}

unsigned int mp_get_call_site_count(const MemoiProf* mp) {
    return mp->call_site_count;
}

const char** mp_get_call_sites(const MemoiProf* mp) {

    return mp->call_sites;
}