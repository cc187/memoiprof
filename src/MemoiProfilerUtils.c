//
// Created by pedro on 13-03-2019.
//

#include "MemoiProfilerUtils.h"
#include "zf_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

cJSON *make_json_header(const MemoiProf *mp) {

    cJSON *json_root = cJSON_CreateObject();

    /* function information */
    cJSON_AddStringToObject(json_root, "id", mp_get_id(mp));
    cJSON_AddStringToObject(json_root, "funcSig", mp_get_func_sig(mp));
    cJSON_AddStringToObject(json_root, "outputType", mp_get_output_type_str(mp));
    cJSON_AddNumberToObject(json_root, "inputCount", mp_get_input_count(mp));

    cJSON *input_types_array = cJSON_CreateArray();
    cJSON_AddItemToObject(json_root, "inputTypes", input_types_array);

    unsigned int input_count = mp_get_input_count(mp);
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


void write_json_and_cleanup(const char *filename, cJSON *json_root) {

    FILE *f = fopen(filename, "w");
    if (f == NULL) {

        printf("Error opening file: %s\n", filename);
        return;
    }

//    char *output = cJSON_Print(json_root);
    char *output = cJSON_PrintBuffered(json_root, 50 * 1024 * 1024, 0);
//    fprintf(f, "%s", output);
    int res = fputs(output, f);
    if (res == EOF) {
        ZF_LOGI("writing of report '%s' failed", filename);
    }

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
 * @param value
 * @return
 */
uint64_t mp_get_bits(void *value, CType type) {

    uint64_t bits64;
    uint32_t bits32;

    switch (type) {
        case FLOAT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0x00000000FFFFFFFF;

        case DOUBLE:

            bits64 = *(uint64_t *) value;
            return bits64;

        case INT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0x00000000FFFFFFFF;
    }

    return 0u;
}

void mp_concat_key(va_list ap, char *key, CType type) {


    void *value = va_arg(ap, void *);
    uint64_t bits = mp_get_bits(value, type);

//    ZF_LOGD("bits: 0x%016lx", bits);

    char str[17];
    snprintf(str, 17, "%016lx", bits);

    strcat(key, str);
}

const char *mp_type_to_string(CType type) {

    switch (type) {

        case FLOAT:
            return "float";
        case DOUBLE:
            return "double";
        case INT:
            return "int";
        default:
            return "";
    }
}

