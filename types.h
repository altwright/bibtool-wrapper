//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_INTS_H
#define BIBTOOL_WRAPPER_INTS_H

#include <stdint.h>

typedef int32_t i32;
typedef int64_t i64;

typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
    char *data;
    i64 len;
    i64 cap;
} DynamicString;

DynamicString dyn_str_make(const char *fmt, ...);
void dyn_str_free(DynamicString *str);
void dyn_str_append(DynamicString *str, const char *fmt, ...);
void dyn_str_to_lower(DynamicString *str);
void dyn_str_to_upper(DynamicString *str);

#endif //BIBTOOL_WRAPPER_INTS_H