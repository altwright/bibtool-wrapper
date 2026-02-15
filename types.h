//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_INTS_H
#define BIBTOOL_WRAPPER_INTS_H

#include <stdint.h>
#include <assert.h>

typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#ifndef ARRAY
#define ARRAY(type) \
type *data; \
i64 len; \
i64 cap;
#endif

typedef struct {
    ARRAY(char)
} string;

typedef struct {
    const char *data;
    i64 len;
} string_view;

typedef struct {
    ARRAY(i64)
} i64s;

string str_make(const char *fmt, ...);

void str_free(string *str);

void str_append(string *str, const char *fmt, ...);

void str_to_lower(string *str);

void str_to_upper(string *str);

bool str_empty(const string *str);

string str_dup(const string *str);

#ifndef ARRAY_MAKE
#define ARRAY_MAKE(array_ptr) \
    do { \
        assert(array_ptr); \
        array_calloc( \
            (void**)(&(array_ptr)->data), \
            &((array_ptr)->len), \
            &((array_ptr)->cap), \
            sizeof(*((array_ptr)->data))); \
    } while(0)
#endif

#ifndef ARRAY_PUSH
#define ARRAY_PUSH(array_ptr, elem_ptr) \
    do { \
        assert(array_ptr); \
        assert(elem_ptr); \
        assert(sizeof(*(array_ptr)->data) == sizeof(*(elem_ptr))); \
        array_push( \
            (void**)(&((array_ptr)->data)), \
            &((array_ptr)->len), \
            &((array_ptr)->cap), \
            sizeof(*(elem_ptr)), \
            (const void*)(elem_ptr) \
        ); \
    } while (0)
#endif

#ifndef ARRAY_EMPTY
#define ARRAY_EMPTY(array_ptr) \
    (!(array_ptr)->data || (array_ptr)->len <= 0 || (array_ptr)->cap <= 0)
#endif

#ifndef ARRAY_FREE
#define ARRAY_FREE(array_ptr) \
    do { \
        if ((array_ptr)->data) \
        { \
            free((array_ptr)->data); \
        } \
        (array_ptr)->data = nullptr; \
        (array_ptr)->len = 0; \
        (array_ptr)->cap = 0; \
    } while(0)
#endif

void array_calloc(void **data_ptr, i64 *len, i64 *cap, i64 elem_size);

void array_push(void **data_ptr, i64 *len, i64 *cap, i64 elem_size, const void *new_elem);

#ifndef FOR_EACH
#define FOR_EACH(ptr_var, array_ptr) \
    for ( \
        typeof(*((array_ptr)->data)) ptr_var = (array_ptr)->data; \
        ptr_var < (array_ptr)->data + (array_ptr)->len; \
        ptr_var++ \
    )
#endif

#endif //BIBTOOL_WRAPPER_INTS_H
