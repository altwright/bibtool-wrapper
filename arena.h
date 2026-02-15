//
// Created by wright on 2/15/26.
//

#ifndef BIBTOOL_WRAPPER_ARENA_H
#define BIBTOOL_WRAPPER_ARENA_H

#include "types.h"

bool arena_init(i64 size);

void arena_uninit();

void *arena_alloc(i64 size);

void arena_free(void *ptr);

#endif //BIBTOOL_WRAPPER_ARENA_H
