//
// Created by pedro on 12-03-2019.
//

#include "MemoiRecord.h"
#include "cJSON.h"

#include <stdlib.h>
#include <stdio.h>


struct mr_t {

    uint64_t input;
    unsigned int counter;
    uint64_t output;
};

MemoiRec *mr_init(uint64_t input, unsigned int counter, uint64_t output) {

    MemoiRec *mr = malloc(sizeof(*mr));

    mr->input = input;
    mr->counter = counter;
    mr->output = output;

    return mr;
}


MemoiRec *mr_destroy(MemoiRec *mr) {

    free(mr);

    return NULL;
}


void mr_simple_print(MemoiRec *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%lx\n", mr->input);
    printf("\toutput  : 0x%lx\n", mr->output);
    printf("\tcounter : %u\n", mr->counter);
}


void mr_inc_counter(MemoiRec *mr) {

    mr->counter++;
}


void mr_make_json(void *key, void *mr, void *json_array) {

    const uint64_t input_bits = ((MemoiRec *) mr)->input;
    const uint64_t output_bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    struct cJSON *count = cJSON_CreateObject();

    char hex_string[17];


    snprintf(hex_string, 17, "%016lx", input_bits);
    cJSON_AddStringToObject(count, "key", hex_string);

    snprintf(hex_string, 17, "%016lx", output_bits);
    cJSON_AddStringToObject(count, "output", hex_string);

    cJSON_AddNumberToObject(count, "counter", counter);

    cJSON_AddItemToArray(json_array, count);
}


void mr_print(void *key, void *mr, void *input_type) {

    float output_float;
    double output_double;
    int output_int;

    const uint64_t bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    switch (*(CType *) input_type) {

        case FLOAT:

            output_float = *(float *) &bits;
            printf("%f ---> %f (%ux)\n", *(float *) key, output_float, counter);
            break;

        case DOUBLE:

            output_double = *(double *) &bits;
            printf("%f ---> %f (%ux)\n", *(double *) key, output_double, counter);
            break;

        case INT:

            output_int = *(int *) &bits;
            printf("%d ---> %d (%ux)\n", *(int *) key, output_int, counter);
            break;
    }
}