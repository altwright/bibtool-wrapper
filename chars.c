//
// Created by wright on 2/15/26.
//

#include "chars.h"

#include <string.h>
#include <ctype.h>

i64 find_next_nonspace_char_idx(const char *str) {
    i64 idx = -1;

    if (!str) {
        return idx;
    }

    i64 str_len = (i64) strlen(str);

    for (i64 char_idx = 1; char_idx < str_len; char_idx++) {
        if (isprint(str[char_idx])) {
            idx = char_idx;
            break;
        }
    }

    return idx;
}

i64 find_next_space_char_idx(const char *str) {
    i64 idx = -1;

    if (!str) {
        return idx;
    }

    i64 str_len = (i64) strlen(str);

    for (i64 char_idx = 1; char_idx < str_len; char_idx++) {
        if (isspace(str[char_idx])) {
            idx = char_idx;
            break;
        }
    }

    return idx;
}

i64 find_prior_nonspace_char_idx(const char *str, i64 current_idx) {
    i64 idx = -1;

    if (!str || current_idx <= 0) {
        return idx;
    }

    for (i64 char_idx = current_idx - 1; char_idx >= 0; char_idx--) {
        if (isprint(str[char_idx])) {
            idx = char_idx;
            break;
        }
    }

    return idx;
}