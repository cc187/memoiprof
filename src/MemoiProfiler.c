//
// Created by pedro on 11-11-2016.
//

#include "MemoiInstrumenter.h"
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


MemoiRecord *memoi_rec_init(uint64_t input, unsigned int counter, uint64_t output) {

    MemoiRecord *mr = malloc(sizeof(*mr));

    mr->input = input;
    mr->counter = counter;
    mr->output = output;

    return mr;
}


MemoiRecord *memoi_rec_destroy(MemoiRecord *mr) {

    free(mr);

    return NULL;
}

void memoi_rec_print(MemoiRecord *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%lx\n", mr->input);
    printf("\toutput  : 0x%lx\n", mr->output);
    printf("\tcounter : %u\n", mr->counter);
}


/***********************************************************************************************************************
 *
 * MemoiInstrumenter
 *
 ***********************************************************************************************************************
 *
 */

typedef struct mi_t {

    char *name;

    CType output_type;
    CType input_type;

    GHashTable *table;

    unsigned int calls;
    unsigned int hits;
};

#define NEW_AND_COPY(name, type, source) \
name = malloc(sizeof *name);\
*name = *((type *) source);

static cJSON *json_root;
static cJSON *json_array;

static void *memoi_inst_get_key(MemoiInstrumenter *mi, void *input);

static uint64_t memoi_inst_get_bits(MemoiInstrumenter *mi, void *value);

static void print_table(void *key, void *mr, void *input_type);

static void print_json(void *key, void *mr, void *input_type);

static unsigned int memoi_float_hash(const void *key);

static int memoi_float_equal(const void *a, const void *b);

static void write_json_and_cleanup(const char *filename);

static void make_simple_json(const MemoiInstrumenter *mi);

MemoiInstrumenter *memoi_inst_init(const char *name, CType type) {

    MemoiInstrumenter *mi = malloc(sizeof *mi);

    size_t name_size = strlen(name) + 1;
    mi->name = calloc(name_size, sizeof *(mi->name));
    strcpy(mi->name, name);

    mi->input_type = type;
    mi->output_type = type;

    mi->calls = 0;
    mi->hits = 0;

    switch (type) {

        case FLOAT:
            mi->table = g_hash_table_new_full(memoi_float_hash, memoi_float_equal, g_free, g_free);
            break;
        case DOUBLE:
            mi->table = g_hash_table_new_full(g_double_hash, g_double_equal, g_free, g_free);
            break;
        case INT:
            mi->table = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_free);
            break;
    }

    return mi;
}

static unsigned int memoi_float_hash(const void *key) {

    return *(const uint32_t *) key;
}

static int memoi_float_equal(const void *a, const void *b) {

    return *(const float *) a == *(const float *) b;
}

MemoiInstrumenter *memoi_inst_destroy(MemoiInstrumenter *mi) {

    if (mi != NULL) {

        free(mi->name);
        g_hash_table_destroy(mi->table);
        free(mi);
    }

    return NULL;
}


void memoi_inst_inc(MemoiInstrumenter *mi, void *input, void *output) {

    if (mi == NULL) {
        return;
    }

    mi->calls++;

    void *mr = g_hash_table_lookup(mi->table, input);

    if (mr != NULL) {

        mi->hits++;
        ((MemoiRecord *) mr)->counter++;
    } else {

        uint64_t input_bits = memoi_inst_get_bits(mi, input);
        uint64_t output_bits = memoi_inst_get_bits(mi, output);
        MemoiRecord *new_mr = memoi_rec_init(input_bits, 1, output_bits);

        void *key = memoi_inst_get_key(mi, input);
        g_hash_table_insert(mi->table, key, new_mr);
    }
}


void memoi_inst_print(MemoiInstrumenter *mi) {

    printf("==================================================\n");
    printf("Table '%s', %u elements, %u calls (%uh, %um)\n\n", mi->name, g_hash_table_size(mi->table), mi->calls,
           mi->hits, mi->calls - mi->hits);


    g_hash_table_foreach(mi->table, print_table, &(mi->input_type));

    printf("==================================================\n");
}

void memoi_inst_to_json(MemoiInstrumenter *mi, const char *filename) {

    make_simple_json(mi);

    /* counts array */
    json_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "counts", json_array);
    g_hash_table_foreach(mi->table, print_json, &(mi->input_type));

    write_json_and_cleanup(filename);

}

void memoi_inst_to_simple_json(MemoiInstrumenter *mi, const char *filename) {

    make_simple_json(mi);
    write_json_and_cleanup(filename);
}

void memoi_inst_all_to_simple_json(const char *filename, unsigned int count, ...) {

    va_list ap;
    va_start(ap, count);

    json_root = cJSON_CreateObject();

    for (int i = 0; i < count; ++i) {

        MemoiInstrumenter *mi = va_arg(ap, MemoiInstrumenter *);

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


static void make_simple_json(const MemoiInstrumenter *mi) {

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

    const uint64_t bits = ((MemoiRecord *) mr)->output;
    const unsigned int counter = ((MemoiRecord *) mr)->counter;

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

    const uint64_t input_bits = ((MemoiRecord *) mr)->input;
    const uint64_t output_bits = ((MemoiRecord *) mr)->output;
    const unsigned int counter = ((MemoiRecord *) mr)->counter;

    cJSON *count = cJSON_CreateObject();

    char hex_string[17];


    snprintf(hex_string, 17, "%016lx", input_bits);
    cJSON_AddStringToObject(count, "key", hex_string);

    snprintf(hex_string, 17, "%016lx", output_bits);
    cJSON_AddStringToObject(count, "output", hex_string);

    cJSON_AddNumberToObject(count, "counter", counter);

    cJSON_AddItemToArray(json_array, count);
}

static uint64_t memoi_inst_get_bits(MemoiInstrumenter *mi, void *value) {

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


static void *memoi_inst_get_key(MemoiInstrumenter *mi, void *input) {

    float *new_float;
    double *new_double;
    int *new_int;

    switch (mi->input_type) {
        case FLOAT:

        NEW_AND_COPY(new_float, float, input)
            return new_float;

        case DOUBLE:

        NEW_AND_COPY(new_double, double, input)
            return new_double;
        case INT:

        NEW_AND_COPY(new_int, int, input)
            return new_int;
    }
    return NULL;
}



