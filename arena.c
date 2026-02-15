//
// Created by wright on 2/15/26.
//

#include "arena.h"
#include <stdlib.h>

struct arenaBuffer;

typedef struct arenaBuffer {
    u8 *data;
    i64 cap;
    struct arenaBuffer *next;
} ArenaBuffer;

static ArenaBuffer *g_buffer = nullptr;
static i64 g_current_arena_offset = 0;;

bool arena_init(i64 size) {
    bool result = true;

    if (!g_buffer) {
        g_buffer = calloc(1, sizeof(ArenaBuffer));
        assert(g_buffer);

        g_buffer->data = calloc(1, size);
        assert(g_buffer->data);

        g_buffer->cap = size;
    } else {
        result = false;
    }

    return result;
}

void arena_uninit() {
    ArenaBuffer *current_buffer = g_buffer;

    while (current_buffer) {
        if (current_buffer->data) {
            free(current_buffer->data);
            current_buffer->data = nullptr;
            current_buffer->cap = 0;
        }

        ArenaBuffer *next_buffer = current_buffer->next;
        free(current_buffer);
        current_buffer = next_buffer;
    }

    g_buffer = nullptr;
}

void *arena_alloc(i64 size) {
    if (!g_buffer) {
        return nullptr;
    }

    void *data = nullptr;

    i64 aligned_size = ((size + 3) >> 2) << 2;

    ArenaBuffer *current_buffer = g_buffer;
    i64 current_buffer_offset = g_current_arena_offset;

    while (current_buffer_offset >= current_buffer->cap) {
        current_buffer_offset -= current_buffer->cap;
        assert(current_buffer->next);
        current_buffer = current_buffer->next;
    }

    while (aligned_size > (current_buffer->cap - current_buffer_offset)) {
        if (!current_buffer->next) {
            ArenaBuffer *next_buffer = calloc(1, sizeof(ArenaBuffer));
            assert(next_buffer);
            next_buffer->cap = aligned_size > current_buffer->cap ? aligned_size : current_buffer->cap;
            next_buffer->data = calloc(1, next_buffer->cap);
            assert(next_buffer->data);
            current_buffer->next = next_buffer;
        }
        current_buffer = current_buffer->next;
        current_buffer_offset = 0;
    }

    data = current_buffer->data + current_buffer_offset;
    g_current_arena_offset += aligned_size;

    return data;
}

i64 arena_offset(void* ptr) {
    i64 ptr_offset = -1;

    ArenaBuffer *current_buffer = g_buffer;
    u8 *byte_ptr = ptr;
    i64 offset = 0;

    while (
        current_buffer &&
        !(
            (byte_ptr >= current_buffer->data) &&
            (byte_ptr < current_buffer->data + current_buffer->cap)
        )
    ) {
        offset += current_buffer->cap;
        current_buffer = current_buffer->next;
    }

    if (current_buffer) {
        i64 byte_ptr_offset = byte_ptr - current_buffer->data;
        offset += byte_ptr_offset;
        ptr_offset = offset;
    }

    return ptr_offset;
}

void arena_free(i64 from) {
    if (from < 0) {
        from = 0;
    }
    g_current_arena_offset = from;
}
