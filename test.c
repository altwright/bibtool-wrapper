//
// Created by wright on 2/10/26.
//

#include <stdio.h>

#include "library.h"
#include <assert.h>

int main(int argc, char **argv) {
    bib_init("test");

    bool failed = bib_open_db("data/Mearsheimer_realism.bib");
    assert(!failed);

    bib_get_reference_html("SmithNicholasRoss2022MRat", CITE_STYLE_APA);

    bib_close_db();
}
