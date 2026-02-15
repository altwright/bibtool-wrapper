//
// Created by wright on 2/10/26.
//

#include <stdio.h>

#include "library.h"
#include <assert.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    bib_init("test");

    bool failed = bib_open_db("data/library.bib");
    assert(!failed);

    char *html_str = bib_create_bib_entry_html("jewish_war", CITE_STYLE_CHICAGO);

    if (html_str) {
        free(html_str);
    }

    bib_close_db();

    bib_uninit();
}
