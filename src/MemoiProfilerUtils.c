//
// Created by pedro on 13-03-2019.
//

#include "MemoiProfilerUtils.h"
#include "zf_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


void write_json_and_cleanup(const char *filename, cJSON *json_root) {

    FILE *f = fopen(filename, "w");
    if (f == NULL) {

        ZF_LOGI("Error opening file: %s\n", filename);
        return;
    }

//    char *output = cJSON_Print(json_root);
    char *output = cJSON_PrintBuffered(json_root, 250 * 1024 * 1024, 0);
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
 *      WE ARE ASSUMING THIS IS EQUAL TO THE RESULT RETURNED BY THE HASH FUNCTION!
 * @param value
 * @return
 */
uint64_t mp_get_bits(void *value, CType type) {

    uint64_t bits64;
    uint32_t bits32;

    switch (type) {
        case MP_FLOAT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0x00000000FFFFFFFF;

        case MP_DOUBLE:

            bits64 = *(uint64_t *) value;
            return bits64;

        case MP_INT:

            bits32 = *(uint32_t *) value;
            bits64 = bits32;

            return bits64 & 0x00000000FFFFFFFF;
    }

    return 0u;
}

uint64_t mp_get_bits_from_va(va_list ap, CType type) {


    void *value = va_arg(ap, void *);
    return mp_get_bits(value, type);
}

void mp_concat_key_with_bits(uint64_t  bits, char *key) {

    char str[17];
    snprintf(str, 17, "%016lx", bits);

    strcat(key, str);
}

const char *mp_type_to_string(CType type) {

    switch (type) {

        case MP_FLOAT:
            return "float";
        case MP_DOUBLE:
            return "double";
        case MP_INT:
            return "int";
        default:
            return "";
    }
}

