//
// Created by pedro on 11-11-2016.
//

#include "MemoiProfiler.h"
#include "MemoiProfilerUtils.h"
#include "MemoiUtils.h"
#include "MemoiRecord.h"
#include "cJSON.h"
#include "zf_log.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdarg.h>


struct mp_t {

    // function config
    char *id;
    char *func_sig;

    // output config
    CType *output_types;
    unsigned int output_count;

    // input config
    CType *input_types;
    unsigned int input_count;
    unsigned int max_key_size;

    // table config
    GHashTable *table;

    // stats
    unsigned int calls;
    unsigned int hits;

    // call site config
    unsigned int call_site_count;
    const char **call_sites;

    // sampling config
    unsigned int sampling;
    unsigned int current_sample;

    // periodic reporting config
    char is_periodic;
    unsigned int period;
    char *periodic_filename;
    unsigned int periodic_part;

    // optimization config
    char remove_low_counts;
};

static cJSON *make_json_header(const MemoiProf *mp);

MemoiProf *mp_init(const char *func_sig, const char *id, unsigned int input_count, unsigned int output_count, ...) {

    ZF_LOGI("initializing '%s' for %s", id, func_sig);

    va_list ap;
    va_start(ap, output_count);

    MemoiProf *mp = malloc(sizeof *mp);

    // function signature
    size_t name_size = strlen(func_sig) + 1;
    mp->func_sig = calloc(name_size, sizeof *(mp->func_sig));
    strcpy(mp->func_sig, func_sig);

    // memoization id
    size_t id_size = strlen(id) + 1;
    mp->id = calloc(id_size, sizeof *(mp->id));
    strcpy(mp->id, id);

    mp->input_count = input_count;
    mp->output_count = output_count;


    const unsigned int key_sizes = 16 * input_count;
    const unsigned int sep_sizes = input_count - 1;
    mp->max_key_size = key_sizes + sep_sizes + 1; // + 1 is for the null terminating character

    mp->input_types = calloc(input_count, sizeof *(mp->input_types));
    for (unsigned int i = 0; i < input_count; ++i) {
        mp->input_types[i] = va_arg(ap, CType);
    }

    mp->output_types = calloc(output_count, sizeof *(mp->output_types));
    for (unsigned int o = 0; o < output_count; ++o) {
        mp->output_types[o] = va_arg(ap, CType);
    }

    mp->calls = 0;
    mp->hits = 0;

    mp->call_site_count = 0;
    mp->call_sites = NULL;

    mp->table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    mp_set_sampling(mp, 1u);

    mp_set_periodic_reporting(mp, 0, 1, NULL);

    mp_set_remove_low_counts(mp, 0);

    va_end(ap);

    return mp;
}

/**
 *  Controls the option to  periodically generate a partial report and write it to a partial file, whose name is based
 *  on the provided name. Whenever a partial report is generated and written, the internal table is reset and all
 *  profiling data is lost.
 *
 * @param mp
 * @param is_periodic
 * @param period The number of calls between writes of periodic reports. This number should be > 0. If it is <= 0,
 * periodic reporting will be disabled.
 * @param periodic_filename The name of split file where the results will be saved. This string will be duplicated and
 * stored, so you are responsible for its memory management.
 */
void mp_set_periodic_reporting(MemoiProf *mp, char is_periodic, int period, char *periodic_filename) {

    // periodic reporting is disabled if period <= 0
    if (period <= 0 || is_periodic == 0) {

        mp->is_periodic = 0;
        mp->period = 1;
        mp->periodic_filename = NULL;

    } else {

        mp->is_periodic = 1;
        mp->period = period;
        mp->periodic_filename = strdup(periodic_filename);
    }

    mp->periodic_part = 0;
}

/**
 * Controls whether low count entries (count == 1) are removed when printing the JSON file.
 *
 * @param mp
 * @param remove_low_counts
 */
void mp_set_remove_low_counts(MemoiProf *mp, char remove_low_counts) {

    mp->remove_low_counts = remove_low_counts;
}

char mp_get_remove_low_counts(MemoiProf *mp) {

    return mp->remove_low_counts;
}

MemoiProf *mp_destroy(MemoiProf *mp) {

    if (mp != NULL) {

        free(mp->input_types);
        free(mp->output_types);
        free(mp->id);
        free(mp->func_sig);
        g_hash_table_destroy(mp->table);
        free(mp->call_sites);
        free(mp->periodic_filename);
    }

    free(mp);

    return NULL;
}

static void mp_reset(MemoiProf *mp) {

    mp->calls = 0;
    mp->hits = 0;

    g_hash_table_remove_all(mp->table);
}

static void mp_periodic_report(MemoiProf *mp) {

    if (mp->is_periodic) {
        if (mp->calls >= mp->period) {

            // generate partial report
            mp->periodic_part = mp->periodic_part + 1;

            // 5 is for the part extension, 4 is to account for digits up to 9999, 1 is for the \0 terminating character
            unsigned int periodic_filename_size = strlen(mp->periodic_filename) + 5 + 4 + 1;
            char *periodic_filename = calloc(periodic_filename_size, sizeof *periodic_filename);
            snprintf(periodic_filename, periodic_filename_size, "%s.part%d", mp->periodic_filename, mp->periodic_part);

            mp_to_json(mp, periodic_filename);
            free(periodic_filename);

            // reset mp
            mp_reset(mp);
        }
    }
}

void mp_inc(MemoiProf *mp, ...) {

    // sampling test and skip
    if (mp->current_sample > 0) {
        mp->current_sample = mp->current_sample - 1;
        return;
    } else {
        mp->current_sample = mp->sampling;
    }

    // periodic test
    mp_periodic_report(mp);

    va_list ap;
    va_start(ap, mp);

    mp->calls++;

    char *key = calloc(mp->max_key_size, sizeof(*key));

    mp_concat_key(ap, key, mp->input_types[0]); // concat the first key
    for (unsigned int i = 1; i < mp->input_count; ++i) {

        // concat every other key separated by #
        strcat(key, "#");
        mp_concat_key(ap, key, mp->input_types[i]);
    }

    void *mr = g_hash_table_lookup(mp->table, key);

    if (mr != NULL) {

        mp->hits++;
        mr_inc_counter(mr);
    } else {

        char *new_key = strdup(key); // this key will be freed when the table is destroyed

        uint64_t *output_bits = calloc(mp->output_count, sizeof *output_bits);
        for (unsigned int o = 0; o < mp->output_count; ++o) {

            output_bits[o] = mp_get_bits(va_arg(ap, void*), mp->output_types[o]);
        }

        MemoiRec *new_mr = mr_init(new_key, mp->output_count, output_bits, mp->output_types);

        g_hash_table_insert(mp->table, new_key, new_mr);
    }

    free(key);

    va_end(ap);
}

void mp_print(MemoiProf *mp) {

    printf("==================================================\n");
    printf("Table '%s', function '%s', %u elements, %u calls (%uh, %um)\n\n", mp->id, mp->func_sig,
           g_hash_table_size(mp->table), mp->calls,
           mp->hits, mp->calls - mp->hits);


    g_hash_table_foreach(mp->table, mr_print, &(mp->output_types[0])); // FIXME

    printf("==================================================\n");
}


void mp_to_json(MemoiProf *mp, const char *filename) {

    cJSON *json_root = make_json_header(mp);

    /* counts array */
    cJSON *json_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "counts", json_array);
    json_info *info = ji_init(mp_get_remove_low_counts(mp), json_array);
    g_hash_table_foreach(mp->table, mr_make_json, info);
    info = ji_destroy(info);

    write_json_and_cleanup(filename, json_root);
}


void mp_set_call_sites(MemoiProf *mp, unsigned int count, ...) {

    va_list ap;
    va_start(ap, count);

    mp->call_site_count = count;
    mp->call_sites = malloc(count * sizeof *(mp->call_sites));

    for (unsigned int i = 0; i < count; ++i) {

        mp->call_sites[i] = va_arg(ap, const char *);
    }

    va_end(ap);
}

CType *mp_get_input_type(const MemoiProf *mp) {
    return mp->input_types;
}


const char *mp_get_id(const MemoiProf *mp) {
    return mp->id;
}


const char *mp_get_func_sig(const MemoiProf *mp) {
    return mp->func_sig;
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

unsigned int mp_get_call_site_count(const MemoiProf *mp) {
    return mp->call_site_count;
}

const char **mp_get_call_sites(const MemoiProf *mp) {

    return mp->call_sites;
}

unsigned int mp_get_input_count(const MemoiProf *mp) {
    return mp->input_count;
}

unsigned int mp_get_output_count(const MemoiProf *mp) {
    return mp->output_count;
}

CType *mp_get_input_types(const MemoiProf *mp) {

    return mp->input_types;
}

CType *mp_get_output_types(const MemoiProf *mp) {

    return mp->output_types;
}

/**
 * Sets the sampling rate. If you want to sample 1/x of the calls, sampling needs to be x. The value of sampling should
 * be >= 1, meaning that every call is accounted for.
 *
 * @param mp the MemoiProf instance
 * @param sampling x, where the sampling rate is 1/x
 */
void mp_set_sampling(MemoiProf *mp, int sampling) {

    if (sampling <= 0) {
        sampling = 1;
    }

    mp->sampling = sampling - 1;
    mp->current_sample = 0u;
}

cJSON *make_json_header(const MemoiProf *mp) {

    cJSON *json_root = cJSON_CreateObject();

    const unsigned int input_count = mp_get_input_count(mp);
    const unsigned int output_count = mp_get_output_count(mp);

    /* function information */
    cJSON_AddStringToObject(json_root, "id", mp_get_id(mp));
    cJSON_AddStringToObject(json_root, "funcSig", mp_get_func_sig(mp));
    cJSON_AddNumberToObject(json_root, "inputCount", input_count);
    cJSON_AddNumberToObject(json_root, "outputCount", output_count);

    // input information
    cJSON *input_types_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "inputTypes", input_types_array);

    CType *input_types = mp_get_input_types(mp);
    for (unsigned int i = 0; i < input_count; ++i) {

        // NEEDS TO BE IN ORDER
        cJSON_AddItemToArray(
                input_types_array,
                cJSON_CreateString(
                        mp_type_to_string(
                                input_types[i]
                        )
                )
        );
    }

    // output information
    cJSON *output_types_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "outputTypes", output_types_array);

    CType *output_types = mp_get_output_types(mp);
    for (unsigned int o = 0; o < output_count; ++o) {

        cJSON_AddItemToArray(
                output_types_array,
                cJSON_CreateString(
                        mp_type_to_string(output_types[o])
                )
        );
    }

    /* profiling information */
    cJSON_AddNumberToObject(json_root, "elements", mp_get_table_size(mp));
    cJSON_AddNumberToObject(json_root, "calls", mp_get_calls(mp));
    cJSON_AddNumberToObject(json_root, "hits", mp_get_hits(mp));
    cJSON_AddNumberToObject(json_root, "misses", mp_get_misses(mp));

    /* call site information */
    cJSON *call_sites_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "call_sites", call_sites_array);

    unsigned int call_site_count = mp_get_call_site_count(mp);
    const char **call_sites = mp_get_call_sites(mp);
    for (unsigned int i = 0; i < call_site_count; ++i) {

        cJSON_InsertItemInArray(call_sites_array, 0, cJSON_CreateString(call_sites[i]));
//        cJSON_AddItemToArray(call_sites_array, cJSON_CreateString(call_sites[i]));
    }

    return json_root;
}