//
// Created by wright on 2/14/26.
//

#include "types.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

DynamicString dyn_str_make(const char *fmt, ...) {
    va_list args_read, args_write;
    va_start(args_read);
    va_start(args_write);

    i32 len = vsnprintf(nullptr, 0, fmt, args_read);
    i64 cap = ((2 * (len + 1) + 3) >> 2) << 2;
    if (cap < 32) {
        cap = 32;
    }

    DynamicString str = {
        .len = len,
        .cap = cap,
    };

    str.data = calloc(cap, sizeof(char));
    assert(str.data);

    i32 bytes_written = vsnprintf(str.data, len + 1, fmt, args_write);
    assert(bytes_written == len);

    va_end(args_read);
    va_end(args_write);

    return str;
}

void dyn_str_append(DynamicString *str, const char *fmt, ...) {
    va_list args_read, args_write;
    va_start(args_read);
    va_start(args_write);

    i32 len = vsnprintf(nullptr, 0, fmt, args_read);

    while ((len + 1) > (str->cap - str->len)) {
        i64 new_cap = 2 * str->cap;
        char *new_data = realloc(str->data, new_cap * sizeof(char));
        assert(new_data);
        str->data = new_data;
        str->cap = new_cap;
    }

    i32 bytes_written = vsnprintf(str->data + str->len, len + 1, fmt, args_write);
    assert(bytes_written == len);

    str->len += len;

    va_end(args_read);
    va_end(args_write);
}

void dyn_str_to_lower(DynamicString *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) tolower(str->data[char_idx]);
    }
}

void dyn_str_to_upper(DynamicString *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) toupper(str->data[char_idx]);
    }
}

void dyn_str_free(DynamicString *str) {
    free(str->data);
    str->data = nullptr;
    str->cap = 0;
    str->len = 0;
}
