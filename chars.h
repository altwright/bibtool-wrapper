//
// Created by wright on 2/15/26.
//

#ifndef BIBTOOL_WRAPPER_CHARS_H
#define BIBTOOL_WRAPPER_CHARS_H

#include "types.h"

i64 find_next_nonspace_char_idx(const char *str);

i64 find_next_space_char_idx(const char *str);

i64 find_prior_nonspace_char_idx(const char *str, i64 current_idx);

#endif //BIBTOOL_WRAPPER_CHARS_H
