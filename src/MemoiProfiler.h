//
// Created by pedro on 11-11-2016.
//

#ifndef MEMOIPROF_MEMOIPROFILER_H
#define MEMOIPROF_MEMOIPROFILER_H

#include "MemoiUtils.h"

struct mp_t;
typedef struct mp_t MemoiProf;

typedef enum sampling_kind_t {
    MP_SAMPLING_RANDOM, MP_SAMPLING_FIXED, MP_SAMPLING_OFF
} SamplingKind;

typedef enum periodic_kind_t {
    MP_PERIODIC_OFF, MP_PERIODIC_ON
} PeriodicKind;

typedef enum culling_kind_t {
    MP_CULLING_OFF, MP_CULLING_ON
} CullingKind;

typedef enum approx_kind_t {
    MP_APPROX_OFF, MP_APPROX_ON
} ApproxKind;

EXTERN_C_BEGIN

/**
 *
 * @param func_sig
 * @param id
 * @param filename the path to the file where the report will be writen (this is duplicated internally, so manage the passed pointer)
 * @param input_count
 * @param output_count
 * @param ...
 * @return
 */
MemoiProf *mp_init(const char *func_sig, const char *id, const char* filename, unsigned int input_count, unsigned int output_count, ...);

MemoiProf *mp_destroy(MemoiProf *mp);

/**
 *
 * @param mp
 * @param ... First the inputs, then the outputs
 */
void mp_inc(MemoiProf *mp, ...);

void mp_print(MemoiProf *mp);

void mp_to_json(MemoiProf *mp);

void mp_set_call_sites(MemoiProf *mp, unsigned int count, ...);

void mp_set_sampling(MemoiProf *mp, SamplingKind sampling, int sampling_rate);

void mp_set_periodic_reporting(MemoiProf *mp, PeriodicKind periodic_kind, int period);

void mp_set_culling(MemoiProf *mp, CullingKind culling_kind, float culling_ratio);

void mp_set_approx(MemoiProf *mp, ApproxKind approx_kind, unsigned int approx_bits);

// getters

CType *mp_get_input_type(const MemoiProf *mp);

const char *mp_get_id(const MemoiProf *mp);

const char *mp_get_uuid(const MemoiProf *mp);

const char *mp_get_func_sig(const MemoiProf *mp);

unsigned int mp_get_table_size(const MemoiProf *mp);

unsigned int mp_get_calls(const MemoiProf *mp);

unsigned int mp_get_hits(const MemoiProf *mp);

unsigned int mp_get_misses(const MemoiProf *mp);

unsigned int mp_get_call_site_count(const MemoiProf *mp);

const char **mp_get_call_sites(const MemoiProf *mp);

unsigned int mp_get_input_count(const MemoiProf *mp);

unsigned int mp_get_output_count(const MemoiProf *mp);

CType *mp_get_input_types(const MemoiProf *mp);

CType *mp_get_output_types(const MemoiProf *mp);

EXTERN_C_END

#endif // MEMOIPROF_MEMOIPROFILER_H


