//
// Created by wright on 2/10/26.
//

#include <stdio.h>

#include "library.h"
#include <assert.h>
#include <stdlib.h>

#include "libs/altcore/hashmap.h"
#include "libs/altcore/memory.h"

typedef struct {
    HASHMAP_FIELDS(int, int)
} Hashmap;

int main(int argc, char **argv) {
    bib_init("test");

    bool failed = bib_open_db("data/library.bib");
    assert(!failed);

    char *html_str = bib_create_bib_entry_html("jewish_war", CITE_STYLE_CHICAGO);

    if (html_str) {
        printf("%s\n", html_str);
        free(html_str);
    }

    bib_close_db();

    bib_uninit();

    Hashmap hashmap = {HASHMAP_TYPE_NON_STR_KEY};
    int default_val = -1;

    alt_init(1024LL * 1024LL);

    HASHMAP_MAKE(&hashmap, &default_val);

    int key1 = 0;
    int val = 10;
    HASHMAP_PUT(&hashmap, &key1, &val);

    int key2 = 1;
    val = 11;
    HASHMAP_PUT(&hashmap, &key2, &val);

    i64 pair_idx = HASHMAP_GET_IDX(&hashmap, &key2);
    assert(pair_idx >= 0);

    auto pair_ptr = HASHMAP_GET_ELEM(&hashmap, &pair_idx);

    HASHMAP_FOR(pair, &hashmap) {
        printf("%d : %d\n", pair->key, pair->value);
    }

    HASHMAP_FREE(&hashmap);

    alt_uninit();
}
