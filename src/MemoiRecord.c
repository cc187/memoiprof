//
// Created by pedro on 12-03-2019.
//

#include "MemoiRecord.h"
#include "cJSON.h"

#include <stdlib.h>
#include <stdio.h>


struct mr_t {

    char*  input;
    unsigned int counter;
    uint64_t output;
};

MemoiRec *mr_init(char* input, unsigned int counter, uint64_t output) {

    MemoiRec *mr = malloc(sizeof(*mr));

    mr->input = input;
    mr->counter = counter;
    mr->output = output;

    return mr;
}


MemoiRec *mr_destroy(MemoiRec *mr) {

    if(mr != NULL) {
        free(mr->input);
    }

    free(mr);

    return NULL;
}

void mr_public_destroy(void* mr) {

    mr_destroy(mr);
}


void mr_simple_print(MemoiRec *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%s\n", mr->input);
    printf("\toutput  : 0x%lx\n", mr->output);
    printf("\tcounter : %u\n", mr->counter);
}


void mr_inc_counter(MemoiRec *mr) {

    mr->counter++;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void mr_make_json(void *key, void *mr, void *json_array) {

    const char* input_string = ((MemoiRec *) mr)->input;
    const uint64_t output_bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    struct cJSON *count = cJSON_CreateObject();

    char output_string[17];

    cJSON_AddStringToObject(count, "key", input_string);

    snprintf(output_string, 17, "%016lx", output_bits);
    cJSON_AddStringToObject(count, "output", output_string);

    cJSON_AddNumberToObject(count, "counter", counter);

//    cJSON_AddItemToArray(json_array, count);

    cJSON_InsertItemInArray(json_array, 0, count);
}
#pragma GCC diagnostic pop


void mr_print(void *key, void *mr, void *output_type) {

    float output_float;
    double output_double;
    int output_int;

    const uint64_t bits = ((MemoiRec *) mr)->output;
    const unsigned int counter = ((MemoiRec *) mr)->counter;

    switch (*(CType *) output_type) {

        case FLOAT:

            output_float = *(float *) &bits;
            printf("%s ---> %f (%ux)\n", (char *) key, output_float, counter);
            break;

        case DOUBLE:

            output_double = *(double *) &bits;
            printf("%s ---> %f (%ux)\n", (char *) key, output_double, counter);
            break;

        case INT:

            output_int = *(int *) &bits;
            printf("%s ---> %d (%ux)\n", (char *) key, output_int, counter);
            break;
    }
}