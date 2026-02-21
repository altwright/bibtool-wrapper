//
// Created by wright on 2/10/26.
//

#include <stdio.h>

#include "library.h"
#include <assert.h>
#include <stdlib.h>

#include "libs/altcore/hashmap.h"

typedef struct {
    HASHMAP_FIELDS(const char*, int)
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

    Hashmap hashmap = {HASHMAP_TYPE_STR_KEY};
    int default_val = -1;

    HASHMAP_MAKE(&hashmap, &default_val);

    const char* key1 = "test";
    int val = 10;
    HASHMAP_PUT(&hashmap, &key1, &val);

    val = 1;

    const char* key2 = "test2";
    val = HASHMAP_GET(&hashmap, &key1);

    printf("%d\n", val);

    HASHMAP_FREE(&hashmap);
}
