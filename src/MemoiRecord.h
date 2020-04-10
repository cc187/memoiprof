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

struct ji_t;
typedef struct ji_t json_info;

/**
 *      Allocates a new MemoiRec. Counter is set to 1.
 *
 *      MemoiRec frees: input, output
 *      MemoiRec does not free: output_types
 *
 * @param input The concatenated string of the inputs
 * @param output_count The number of outputs of this record
 * @param output The array of bits of the original outputs
 * @param output_types The array of types of the outputs
 *
 * @return A pointer to the newly allocated MemoiRec
 */
MemoiRec *mr_init(char *input, unsigned int output_count, uint64_t *output, CType *output_types);


/**
 *      Incremements the counter of this record.
 * @param mr The record to increment
 */
void mr_inc_counter(MemoiRec *mr);

/**
 *      Sets the counter of this record.
 * @param mr The record to increment
 * @param new_counter The new value to set
 */
void mr_set_counter(MemoiRec *mr, unsigned int new_counter);

/**
 *      Frees an allocated MemoiRec. Always returns NULL so you can assign it on destruction.
 *
 * @param mr The MemoiRec that will be freed
 * @return NULL
 */
MemoiRec *mr_destroy(MemoiRec *mr);

void mr_public_destroy(void *mr);

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
 * @param output_type The C type of the input
 */
void mr_print(void *key, void *mr, void *output_type);

json_info *ji_init(char opt, void *json_array);

json_info *ji_destroy(json_info *ji);

#endif //MEMOIPROF_MEMOIRECORD_H
