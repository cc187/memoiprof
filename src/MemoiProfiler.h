//
// Created by pedro on 11-11-2016.
//

#ifndef _MEMOI_PROFILER_H
#define _MEMOI_PROFILER_H

#include <stdint.h>
#include <stdarg.h>

/**
 *
 *      The record that is stored inside the hash table and that maps an input to an output. More importantly, it also
 *  counts how many times that input appears.
 *
 */
typedef struct mr_t {
    uint64_t input;
    unsigned int counter;
    uint64_t output;
} MemoiRec;

/**
 *      Allocates a new MemoiRec.
 *
 * @param input The bits of the original input
 * @param counter The initial counter value
 * @param output The bits of the original output
 * @return A pointer to the newly allocated MemoiRec
 */
MemoiRec *mr_init(uint64_t input, unsigned int counter, uint64_t output);

/**
 *      Frees an allocated MemoiRec. Always returns NULL so you can assign it on destruction.
 *
 * @param mr The MemoiRec that will be freed
 * @return NULL
 */
MemoiRec *mr_destroy(MemoiRec *mr);

/**
 *      Pretty prints the contents of a MemoiRec.
 *
 * @param mr The MemoiRec that will be printed
 */
void mr_print(MemoiRec *mr);

/**
 *      Enumeration that represent the types this library deals with.
 *  @enum
 */
typedef enum ctype_t {
    FLOAT, DOUBLE, INT
} CType;

struct mp_t;
typedef struct mp_t MemoiProf;

MemoiProf *mp_init(const char *name, CType type);

MemoiProf *mp_destroy(MemoiProf *mp);

void mp_inc(MemoiProf *mp, void *input, void *output);

void mp_print(MemoiProf *mi);

void mp_to_json(MemoiProf *mi, const char *filename);

void mp_to_simple_json(MemoiProf *mi, const char *filename);

void mp_all_to_simple_json(const char *filename, unsigned int count, ...);

#endif // _MEMOI_PROFILER_H


