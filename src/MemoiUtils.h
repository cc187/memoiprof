//
// Created by pedro on 12-03-2019.
//

#ifndef MEMOIPROF_MEMOIUTILS_H
#define MEMOIPROF_MEMOIUTILS_H

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

/**
 *      Enumeration that represent the types this library deals with.
 *  @enum
 */
typedef enum ctype_t {
    MP_FLOAT, MP_DOUBLE, MP_INT
} CType;

EXTERN_C_BEGIN
char* mp_make_report_path(const char* dir, const char* id);
EXTERN_C_END

#endif //MEMOIPROF_MEMOIUTILS_H
