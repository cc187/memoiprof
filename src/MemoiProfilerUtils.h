//
// Created by pedro on 13-03-2019.
//

#ifndef MEMOIPROF_MEMOIPROFILERUTILS_H
#define MEMOIPROF_MEMOIPROFILERUTILS_H

#include "MemoiProfiler.h"
#include "cJSON.h"

#include <stdint.h>

void *mp_dup_input(MemoiProf *mp, void *input);

uint64_t mp_get_bits(MemoiProf *mi, void *value);

unsigned int memoi_float_hash(const void *key);

int memoi_float_equal(const void *a, const void *b);

void write_json_and_cleanup(const char *filename, cJSON *json_root);

cJSON * make_json_header(const MemoiProf *mp);

#endif //MEMOIPROF_MEMOIPROFILERUTILS_H
