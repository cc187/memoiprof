//
// Created by pedro on 11-11-2016.
//

#ifndef MEMOIPROF_MEMOIPROFILER_H
#define MEMOIPROF_MEMOIPROFILER_H

#include "MemoiUtils.h"

struct mp_t;
typedef struct mp_t MemoiProf;

MemoiProf *mp_init(const char *name, CType type);

MemoiProf *mp_destroy(MemoiProf *mp);

void mp_inc(MemoiProf *mp, void *input, void *output);

void mp_print(MemoiProf *mi);

void mp_to_json(MemoiProf *mi, const char *filename);

// getters

CType mp_get_input_type(const MemoiProf* mp);

const char* mp_get_name(const MemoiProf* mp);

unsigned int mp_get_table_size(const MemoiProf* mp);

unsigned int mp_get_calls(const MemoiProf* mp);

unsigned int mp_get_hits(const MemoiProf* mp);

unsigned int mp_get_misses(const MemoiProf* mp);



#endif // MEMOIPROF_MEMOIPROFILER_H


