

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

    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);
    mp_inc(mp, &o1, &i1);

    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);
    mp_inc(mp, &o2, &i2);

    mp_print(mp);

    mp_to_json(mp, "mp.json");
    mp = mp_destroy(mp);


    MemoiProf *mi2 = mp_init("log", "tested", DOUBLE, 1, DOUBLE);

    double i12 = 3.14;
    double o12 = 345.127;

    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);
    mp_inc(mi2, &o12, &i12);

    mp_to_json(mi2, "mi2.json");
    mi2 = mp_destroy(mi2);


    MemoiProf *mp_pow = mp_init("pow(double,double)", "2 args", DOUBLE, 2, DOUBLE, DOUBLE);
    mp_set_call_sites(mp_pow, 1, "global");
    mp_set_sampling(mp_pow, 100);
    mp_set_periodic_reporting(mp_pow, 1, 100, "mp_pow.json");

    double base = 2.0;
    double power = 8.0;
    double output = pow(base, power);


    srand(time(NULL));   // Initialization, should only be called once.

    for (int i = 0; i < 100000; ++i) {

        const double b = i * 54 + output;
        const double e = rand() % 10;
        double o = pow(b, e);
        mp_inc(mp_pow, &o, &b, &e);
    }
//    mp_print(mp_pow);
    mp_to_json(mp_pow, "mp_pow.json");


    MemoiProf *mp_numbers = mp_init("cos(double)", "numbers", DOUBLE, 1, DOUBLE);
//    mp_set_periodic_reporting(mp_numbers, 1, 10, "mp_numbers.json");

    int keys[] = {15, 8, 14, 7, 7, 12, 3, 12, 22, 10, 18, 14, 25, 18, 26, 24, 29, 23, 20, 13, 1, 5, 3, 9, 21, 14, 29,
                  26, 24, 20, 15, 13, 10, 2, 28, 27, 22, 20, 23, 16, 21, 7, 20, 25, 15, 1, 15, 10, 21, 5, 4, 14, 6, 28,
                  21, 9, 29, 27, 22, 20, 28, 9, 29, 27, 17, 14, 9, 19, 18, 24, 26, 7, 6, 25, 10, 2, 6, 5, 29, 5, 1, 17,
                  29, 22, 19, 6, 5, 11, 24, 6, 1, 20, 13, 27, 15, 2, 5, 14, 25, 8};

    for (int index = 0; index < 100; ++index) {

        double input = keys[index];
        double output = cos(input);
        mp_inc(mp_numbers, &output, &input);
    }

    mp_to_json(mp_numbers, "mp_numbers.complete.json");


    mp_pow = mp_destroy(mp_pow);
}
