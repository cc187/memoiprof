//
// Created by pedro on 13-03-2019.
//

#ifndef MEMOIPROF_MEMOIPROFILERUTILS_H
#define MEMOIPROF_MEMOIPROFILERUTILS_H

#include "MemoiProfiler.h"
#include "cJSON.h"

#include <stdint.h>
#include <stdio.h>

uint64_t mp_get_bits(void *value, CType type);

unsigned int memoi_float_hash(const void *key);

int memoi_float_equal(const void *a, const void *b);

void write_json_and_cleanup(const char *filename, cJSON *json_root);

cJSON *make_json_header(const MemoiProf *mp);

void mp_concat_key(va_list ap, char *key, CType type);

const char *mp_type_to_string(CType type);


#endif //MEMOIPROF_MEMOIPROFILERUTILS_H
