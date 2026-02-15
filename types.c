//
// Created by wright on 2/14/26.
//

#include "types.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void array_calloc(void **data_ptr, i64 *len, i64 *cap, i64 elem_size) {
    if (*len < 0) {
        *len = 0;
    }

    if (*len > *cap) {
        *cap = *len;
    }

    if (*cap <= 0) {
        *cap = 8;
    }

    *data_ptr = calloc(*cap, elem_size);
    assert(*data_ptr);
}

void array_push(void **data_ptr, i64 *len, i64 *cap, i64 elem_size, const void *new_elem) {
    assert(*len >= 0);
    assert(*len <= *cap);

    if (*len == *cap) {
        i64 new_cap = 2 * (*cap);
        void *new_data = realloc(*data_ptr, new_cap * elem_size);
        assert(new_data);
        *data_ptr = new_data;
        *cap = new_cap;
    }

    u8 *data_bytes = *data_ptr;
    u8 *new_elem_bytes = data_bytes + (elem_size * (*len));
    memcpy(new_elem_bytes, new_elem, elem_size);

    (*len)++;
}

string str_make(const char *fmt, ...) {
    va_list args_read, args_write;
    va_start(args_read);
    va_start(args_write);

    i32 len = vsnprintf(nullptr, 0, fmt, args_read);
    i64 cap = ((2 * (len + 1) + 3) >> 2) << 2;
    if (cap < 32) {
        cap = 32;
    }

    string str = {
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

void str_append(string *str, const char *fmt, ...) {
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

void str_to_lower(string *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) tolower(str->data[char_idx]);
    }
}

void str_to_upper(string *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) toupper(str->data[char_idx]);
    }
}

void str_free(string *str) {
    if (str->data) {
        free(str->data);
    }
    str->data = nullptr;
    str->cap = 0;
    str->len = 0;
}

bool str_empty(const string *str) {
    return !str->data || str->len <= 0 || str->cap <= 0;
}

string str_dup(const string* str) {
    assert(str);

    string new_str = {
        .len = str->len,
        .cap = str->cap,
    };

    new_str.data = calloc(new_str.cap, sizeof(char));
    assert(new_str.data);

    memcpy(new_str.data, str->data, str->len);

    return new_str;
}
