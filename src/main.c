#include <stdio.h>

#include "MemoiProfiler.h"
#include "cJSON.h"

int main() {

    printf("-= Memoization Profiler Test =-\n");

    MemoiProf *mp = mp_init("log", "test", FLOAT);

    float i1 = 3.14f;
    float o1 = 345.127f;
    float i2 = 1.0f;
    float o2 = 0.0f;

    mp_add_call_sites(mp, 2, "main.c:19", "main.c:22");

    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);
    mp_inc(mp, &i1, &o1);

    mp_inc(mp, &i2, &o2);
    mp_inc(mp, &i2, &o2);
    mp_inc(mp, &i2, &o2);
    mp_inc(mp, &i2, &o2);

    mp_print(mp);

    /*  */
    MemoiProf *mi2 = mp_init("log", "tested", DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);

    mp_to_json(mp, "mp.json");
    mp_to_json(mi2, "mi2.json");

    mi2 = mp_destroy(mi2);
    mp = mp_destroy(mp);

    return 0;
}
