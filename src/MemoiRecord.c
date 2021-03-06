//
// Created by pedro on 12-03-2019.
//

#include "MemoiRecord.h"
#include "cJSON.h"
#include "MemoiProfilerUtils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ji_t {
    cJSON *counts_object;
    int culling_on;
    float culling_ratio;
    unsigned int total_calls;
};

struct mr_t {

    char *input;
    unsigned int counter;
    unsigned int output_count;
    uint64_t *output;
    CType *output_types;
};

json_info *ji_init(int culling_on, float culling_ratio, void *counts_object, unsigned int total_calls) {

    json_info *ji = malloc(sizeof(*ji));

    ji->culling_on = culling_on;
    ji->culling_ratio = culling_ratio;
    ji->counts_object = counts_object;
    ji->total_calls = total_calls;

    return ji;
}

json_info *ji_destroy(json_info *ji) {

    free(ji);
    return NULL;
}

MemoiRec *mr_init(char *input, unsigned int output_count, uint64_t *output, CType *output_types) {

    MemoiRec *mr = malloc(sizeof(*mr));

    mr->input = input;
    mr->counter = 1;
    mr->output = output;
    mr->output_count = output_count;
    mr->output_types = output_types;

    return mr;
}


MemoiRec *mr_destroy(MemoiRec *mr) {

    if (mr != NULL) {
        free(mr->input);
        free(mr->output);
    }

    free(mr);

    return NULL;
}

void mr_public_destroy(void *mr) {

    mr_destroy(mr);
}


void mr_simple_print(MemoiRec *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%s\n", mr->input);
    for (unsigned int i = 0; i < mr->output_count; ++i) {

        printf("\toutput %d  : 0x%lx\n", i, mr->output[i]);
    }
    printf("\tcounter : %u\n", mr->counter);
}


void mr_inc_counter(MemoiRec *mr) {

    mr->counter++;
}

void mr_set_counter(MemoiRec *mr, unsigned int new_counter) {

    mr->counter = new_counter;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void mr_make_json(void *key, void *mr, void *info) {

    json_info *json_info = info;

    // get the total calls counter and cull if needed
    const unsigned int counter = ((MemoiRec *) mr)->counter;
    if (json_info->culling_on) {
        if (counter / json_info->total_calls < json_info->culling_ratio) {
            return;
        }
    }

    // create the "count" JSON object
    struct cJSON *count = cJSON_CreateObject();

    // add the total calls counter
    cJSON_AddNumberToObject(count, "counter", counter);

    // add the key / concatenated inputs
    const char *input_string = ((MemoiRec *) mr)->input;
    cJSON_AddStringToObject(count, "key", input_string);

    // add the output
    const uint64_t *output_bits = ((MemoiRec *) mr)->output;
    unsigned int output_count = ((MemoiRec *) mr)->output_count;

    const unsigned int outputs_size = 16 * output_count;
    const unsigned int separators_size = output_count - 1;
    const unsigned int output_string_size =
            outputs_size + separators_size + 1; // + 1 is for the null terminating character

    char *output_string = calloc(output_string_size, sizeof(*key));

    mp_concat_key_with_bits(output_bits[0], output_string); // concat the first key

    for (unsigned int i = 1; i < output_count; ++i) {

        // concat every other key separated by #
        strcat(output_string, "#");
        mp_concat_key_with_bits(output_bits[i], output_string);
    }

    cJSON_AddStringToObject(count, "output", output_string);

    // add the "count" JSON object to the "counts" array
    cJSON_AddItemToObject(json_info->counts_object, input_string, count);
}

#pragma GCC diagnostic pop


void mr_print(void *key, void *mr, void *output_type) {

    float output_float;
    double output_double;
    int output_int;

    // FIXME: this function does not work for multiple outputs
    if (((MemoiRec *) mr)->output_count > 1) {
        printf("mr_print is not implemented for multiple outputs");
        return;
    }

    const uint64_t bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    switch (*(CType *) output_type) {

        case MP_FLOAT:

            output_float = *(float *) &bits;
            printf("%s ---> %f (%ux)\n", (char *) key, output_float, counter);
            break;

        case MP_DOUBLE:

            output_double = *(double *) &bits;
            printf("%s ---> %f (%ux)\n", (char *) key, output_double, counter);
            break;

        case MP_INT:

            output_int = *(int *) &bits;
            printf("%s ---> %d (%ux)\n", (char *) key, output_int, counter);
            break;
    }
}