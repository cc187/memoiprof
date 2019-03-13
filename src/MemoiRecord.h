//
// Created by pedro on 12-03-2019.
//

#ifndef MEMOIPROF_MEMOIRECORD_H
#define MEMOIPROF_MEMOIRECORD_H

#include "MemoiUtils.h"
#include <stdint.h>


/**
 *
 *      The record that is stored inside the hash table and that maps an input to an output. More importantly, it also
 *  counts how many times that input appears.
 *
 */
struct mr_t;
typedef struct mr_t MemoiRec;


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
 *      Incremements the counter of this record.
 * @param mr The record to increment
 */
void mr_inc_counter(MemoiRec *mr);

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
void mr_simple_print(MemoiRec *mr);


/**
 *      Callback function used to transform a record into JSON.
 *
 * @param key not used
 * @param mr The record
 * @param json_array the array where we append the record information
 */
void mr_make_json(void *key, void *mr, void *user_data);


/**
 *      Callback function used to print the record to the standard output.
 *
 * @param key The key
 * @param mr The record
 * @param input_type The C type of the input
 */
void mr_print(void *key, void *mr, void *input_type);

#endif //MEMOIPROF_MEMOIRECORD_H
