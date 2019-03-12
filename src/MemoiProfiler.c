//
// Created by pedro on 11-11-2016.
//

#include "MemoiProfiler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "cJSON.h"


/***********************************************************************************************************************
 *
 * MemoiRecord
 *
 ***********************************************************************************************************************
 *
 */


MemoiRec *mr_init(uint64_t input, unsigned int counter, uint64_t output) {

    MemoiRec *mr = malloc(sizeof(*mr));

    mr->input = input;
    mr->counter = counter;
    mr->output = output;

    return mr;
}


MemoiRec *mr_destroy(MemoiRec *mr) {

    free(mr);

    return NULL;
}

void mr_print(MemoiRec *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%lx\n", mr->input);
    printf("\toutput  : 0x%lx\n", mr->output);
    printf("\tcounter : %u\n", mr->counter);
}


/***********************************************************************************************************************
 *
 * MemoiProfiler
 *
 ***********************************************************************************************************************
 *
 */

struct mp_t {

    char *name;

    CType output_type;
    CType input_type;

    GHashTable *table;

    unsigned int calls;
    unsigned int hits;
};

static cJSON *json_root;
static cJSON *json_array;

static void *mp_dup_input(MemoiProf *mi, void *input);

static uint64_t mp_get_bits(MemoiProf *mi, void *value);

static void print_table(void *key, void *mr, void *input_type);

static void print_json(void *key, void *mr, void *input_type);

static unsigned int memoi_float_hash(const void *key);

static int memoi_float_equal(const void *a, const void *b);

static void write_json_and_cleanup(const char *filename);

static void make_simple_json(const MemoiProf *mi);

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

static unsigned int memoi_float_hash(const void *key) {

    return *(const uint32_t *) key;
}

static int memoi_float_equal(const void *a, const void *b) {

    return *(const float *) a == *(const float *) b;
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
        ((MemoiRec *) mr)->counter++;
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


    g_hash_table_foreach(mi->table, print_table, &(mi->input_type));

    printf("==================================================\n");
}

void mp_to_json(MemoiProf *mi, const char *filename) {

    make_simple_json(mi);

    /* counts array */
    json_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "counts", json_array);
    g_hash_table_foreach(mi->table, print_json, &(mi->input_type));

    write_json_and_cleanup(filename);

}

void mp_to_simple_json(MemoiProf *mi, const char *filename) {

    make_simple_json(mi);
    write_json_and_cleanup(filename);
}

void mp_all_to_simple_json(const char *filename, unsigned int count, ...) {

    va_list ap;
    va_start(ap, count);

    json_root = cJSON_CreateObject();

    for (unsigned int i = 0; i < count; ++i) {

        MemoiProf *mi = va_arg(ap, MemoiProf *);

        if (mi == NULL) {
            continue;
        }

        cJSON *object = cJSON_CreateObject();

        cJSON_AddNumberToObject(object, "elements", g_hash_table_size(mi->table));
        cJSON_AddNumberToObject(object, "calls", mi->calls);
        cJSON_AddNumberToObject(object, "hits", mi->hits);
        cJSON_AddNumberToObject(object, "misses", mi->calls - mi->hits);

        cJSON_AddItemToObject(json_root, mi->name, object);
    }

    write_json_and_cleanup(filename);

    va_end(ap);
}


static void make_simple_json(const MemoiProf *mi) {

    json_root = cJSON_CreateObject();

    /* table information */
    cJSON_AddStringToObject(json_root, "name", mi->name);
    cJSON_AddNumberToObject(json_root, "elements", g_hash_table_size(mi->table));
    cJSON_AddNumberToObject(json_root, "calls", mi->calls);
    cJSON_AddNumberToObject(json_root, "hits", mi->hits);
    cJSON_AddNumberToObject(json_root, "misses", mi->calls - mi->hits);
}

static void write_json_and_cleanup(const char *filename) {

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


/**
 *      Callback function used to print the table to the standard output.
 *
 * @param key The key
 * @param mr The record
 * @param input_type The C type of the input
 */
static void print_table(void *key, void *mr, void *input_type) {

    float output_float;
    double output_double;
    int output_int;

    const uint64_t bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    switch (*(CType *) input_type) {

        case FLOAT:

            output_float = *(float *) &bits;
            printf("%f ---> %f (%ux)\n", *(float *) key, output_float, counter);
            break;

        case DOUBLE:

            output_double = *(double *) &bits;
            printf("%f ---> %f (%ux)\n", *(double *) key, output_double, counter);
            break;

        case INT:

            output_int = *(int *) &bits;
            printf("%d ---> %d (%ux)\n", *(int *) key, output_int, counter);
            break;

    }

}

/**
 *      Callback function used to print the table to a JSON file.
 * @param key The key
 * @param mr The record
 * @param input_type The C type of the input
 */
void print_json(void *key, void *mr, void *input_type) {

    const uint64_t input_bits = ((MemoiRec *) mr)->input;
    const uint64_t output_bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    cJSON *count = cJSON_CreateObject();

    char hex_string[17];


    snprintf(hex_string, 17, "%016lx", input_bits);
    cJSON_AddStringToObject(count, "key", hex_string);

    snprintf(hex_string, 17, "%016lx", output_bits);
    cJSON_AddStringToObject(count, "output", hex_string);

    cJSON_AddNumberToObject(count, "counter", counter);

    cJSON_AddItemToArray(json_array, count);
}

static uint64_t mp_get_bits(MemoiProf *mi, void *value) {

    uint64_t bits64;
    uint32_t bits32;

    switch (mi->input_type) {
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

/**
 *      Duplicates the given input. Allocates memory for a new variable of the same type, copies the content and
 *  returns the new value.
 * @param mi
 * @param input
 * @return
 */
static void *mp_dup_input(MemoiProf *mi, void *input) {

    float *new_float;
    double *new_double;
    int *new_int;

    switch (mi->input_type) {

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
