#include <stdio.h>

#include "MemoiProfiler.h"
#include "cJSON.h"

int main() {

    printf("-= Memoization Profiler Test =-\n");

    MemoiProf *mi = mp_init("test", FLOAT);

    float i1 = 3.14f;
    float o1 = 345.127f;
    float i2 = 1.0f;
    float o2 = 0.0f;

    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);
    mp_inc(mi, &i1, &o1);

    mp_inc(mi, &i2, &o2);
    mp_inc(mi, &i2, &o2);
    mp_inc(mi, &i2, &o2);
    mp_inc(mi, &i2, &o2);

    mp_print(mi);

    /*  */
    MemoiProf *mi2 = mp_init("tested", DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);

    mp_all_to_simple_json("/home/pedro/Documents/cJSON_output.txt", 2, mi, mi2);
    mp_to_json(mi, "/home/pedro/Documents/cJSON_output.txt");

    mi2 = mp_destroy(mi2);
    /*  */


    mi = mp_destroy(mi);

    return 0;
}
