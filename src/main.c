

#include "MemoiProfiler.h"
#include "cJSON.h"
#include "zf_log.h"

#include <glib.h>
#include <stdio.h>
#include <math.h>

void profiler_test();

void hash_table_test();

int main() {

//    hash_table_test();

    profiler_test();

    return 0;
}

void hash_table_test() {

    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    char *str = "hello";
    char *key = strdup(str);

    printf("start: %s\n", key);

    g_hash_table_insert(table, key, "world");
    printf("after 1st insert: %s\n", key);

    g_hash_table_insert(table, key, "world");
    printf("after 2nd insert: %s\n", key);

    g_hash_table_destroy(table);

    printf("end: %s\n", key);
}

void profiler_test() {


    printf("-= Memoization Profiler Test =-\n");

    MemoiProf *mp = mp_init("log", "test", FLOAT, 1, FLOAT);

    float i1 = 3.14f;
    float o1 = 345.127f;
    float i2 = 1.0f;
    float o2 = 0.0f;

    mp_set_call_sites(mp, 2, "main.c:19", "main.c:22");

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

    mp_to_json(mp, "mp.json");
    mp = mp_destroy(mp);


    MemoiProf *mi2 = mp_init("log", "tested", DOUBLE, 1, DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);
    mp_inc(mi2, &i12, &o12);

    mp_to_json(mi2, "mi2.json");
    mi2 = mp_destroy(mi2);


    MemoiProf *mp_pow = mp_init("pow(double,double)", "2 args", DOUBLE, 2, DOUBLE, FLOAT);
    mp_set_call_sites(mp_pow, 1, "global");

    double base = 2.0;
    float power = 8.0f;
    double output = pow(base, power);

    mp_inc(mp_pow, &output, &base, &power);

    mp_print(mp_pow);

    mp_to_json(mp_pow, "mp_pow.json");

    mp_pow = mp_destroy(mp_pow);}
