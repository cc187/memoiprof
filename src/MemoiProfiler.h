//
// Created by pedro on 11-11-2016.
//

#ifndef MEMOIPROF_MEMOIPROFILER_H
#define MEMOIPROF_MEMOIPROFILER_H

#include "MemoiUtils.h"

struct mp_t;
typedef struct mp_t MemoiProf;

MemoiProf *mp_init(const char *func_sig, const char *id, CType output_type, unsigned int input_count, ...);

MemoiProf *mp_destroy(MemoiProf *mp);

void mp_inc(MemoiProf *mp, void *output, ...);

void mp_print(MemoiProf *mp);

void mp_to_json(MemoiProf *mp, const char *filename);

void mp_set_call_sites(MemoiProf *mp, unsigned int count, ...);

void mp_set_sampling(MemoiProf *mp, int sampling);

void mp_set_periodic_reporting(MemoiProf *mp, char is_periodic, int period, char *periodic_filename);

void mp_set_remove_low_counts(MemoiProf *mp, char remove_low_counts);

// getters

const char *mp_get_output_type_str(const MemoiProf *mp);

CType *mp_get_input_type(const MemoiProf *mp);

const char *mp_get_id(const MemoiProf *mp);

const char *mp_get_func_sig(const MemoiProf *mp);

unsigned int mp_get_table_size(const MemoiProf *mp);

unsigned int mp_get_calls(const MemoiProf *mp);

unsigned int mp_get_hits(const MemoiProf *mp);

unsigned int mp_get_misses(const MemoiProf *mp);

unsigned int mp_get_call_site_count(const MemoiProf *mp);

const char **mp_get_call_sites(const MemoiProf *mp);

unsigned int mp_get_input_count(const MemoiProf *mp);

CType *mp_get_input_types(const MemoiProf *mp);

char mp_get_remove_low_counts(MemoiProf *mp);

#endif // MEMOIPROF_MEMOIPROFILER_H


