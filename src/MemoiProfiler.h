//
// Created by pedro on 11-11-2016.
//

#ifndef GLIBTEST_MEMOIINSTRUMENTER_H
#define GLIBTEST_MEMOIINSTRUMENTER_H

#include <stdint.h>
#include <stdarg.h>

/***********************************************************************************************************************
 *
 * MemoiRecord
 *
 ***********************************************************************************************************************
 *
 */

/**
 * blah
 */
typedef struct {
    uint64_t input;
    unsigned int counter;
    uint64_t output;
} MemoiRecord;

/**
 *      Allocates a new MemoiRecord.
 *
 * @param input The bits of the original input
 * @param counter The initial counter value
 * @param output The bits of the original output
 * @return A pointer to the newly allocated MemoiRecord
 */
MemoiRecord *memoi_rec_init(uint64_t input, unsigned int counter, uint64_t output);

/**
 *      Frees an allocated MemoiRecord. Always returns NULL so you can assign it on destruction.
 *
 * @param mr The MemoiRecord that will be freed
 * @return NULL
 */
MemoiRecord *memoi_rec_destroy(MemoiRecord *mr);

/**
 *      Pretty prints the contents of a MemoiRecord.
 *
 * @param mr The MemoiRecord that will be printed
 */
void memoi_rec_print(MemoiRecord *mr);


/***********************************************************************************************************************
 *
 * MemoiInstrumenter
 *
 ***********************************************************************************************************************
 *
 */

typedef enum {
    FLOAT, DOUBLE, INT
} CType;

struct mi_t;
typedef struct mi_t MemoiInstrumenter;


MemoiInstrumenter *memoi_inst_init(const char *name, CType type);

MemoiInstrumenter *memoi_inst_destroy(MemoiInstrumenter *mi);

void memoi_inst_inc(MemoiInstrumenter *mi, void *input, void *output);

void memoi_inst_print(MemoiInstrumenter *mi);

void memoi_inst_to_json(MemoiInstrumenter *mi, const char *filename);

void memoi_inst_to_simple_json(MemoiInstrumenter *mi, const char *filename);

void memoi_inst_all_to_simple_json(const char *filename, unsigned int count, ...);

#endif //GLIBTEST_MEMOIINSTRUMENTER_H


