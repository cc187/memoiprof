//
// Created by pedro on 12-03-2019.
//

#include "MemoiRecord.h"
#include <stdlib.h>
#include <stdio.h>





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


void mr_print(MemoiRec *mr) {

    printf("------------------------\n");
    printf("\tinput   : 0x%lx\n", mr->input);
    printf("\toutput  : 0x%lx\n", mr->output);
    printf("\tcounter : %u\n", mr->counter);
}


void mr_inc_counter(MemoiRec *mr) {

    mr->counter++;
}
