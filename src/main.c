#include <stdio.h>

#include "MemoiInstrumenter.h"
#include "cJSON.h"

int main() {

    printf("-= GLib Hash Table Test =-\n");

    MemoiInstrumenter *mi = memoi_inst_init("test", FLOAT);

    float i1 = 3.14f;
    float o1 = 345.127f;
    float i2 = 1.0f;
    float o2 = 0.0f;

    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);
    memoi_inst_inc(mi, &i1, &o1);

    memoi_inst_inc(mi, &i2, &o2);
    memoi_inst_inc(mi, &i2, &o2);
    memoi_inst_inc(mi, &i2, &o2);
    memoi_inst_inc(mi, &i2, &o2);

    memoi_inst_print(mi);

    /*  */
    MemoiInstrumenter *mi2 = memoi_inst_init("tested", DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    memoi_inst_inc(mi2, &i12, &o12);
    memoi_inst_inc(mi2, &i12, &o12);
    memoi_inst_inc(mi2, &i12, &o12);
    memoi_inst_inc(mi2, &i12, &o12);

    memoi_inst_all_to_simple_json("/home/pedro/Documents/cJSON_output.txt", 2, mi, mi2);
    memoi_inst_to_json(mi, "/home/pedro/Documents/cJSON_output.txt");

    mi2 = memoi_inst_destroy(mi2);
    /*  */


    mi = memoi_inst_destroy(mi);

    return 0;
}
