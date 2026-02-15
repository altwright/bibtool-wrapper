#ifndef BIBTOOL_WRAPPER_LIBRARY_H
#define BIBTOOL_WRAPPER_LIBRARY_H

#include <stdint.h>
#include <stdio.h>

typedef enum : uint32_t {
    CITE_STYLE_CHICAGO = 0,
    CITE_STYLE_COUNT
} CitationStyle;

void bib_init(const char *program_name);

void bib_uninit();

bool bib_open_db(const char *filepath);

void bib_close_db();

char *bib_create_bib_entry_html(const char *citation_key, CitationStyle style);

char *bib_create_note_html(const char *citation_key, CitationStyle style);

char *bib_create_short_note_html(const char *citation_key, CitationStyle style);

#endif // BIBTOOL_WRAPPER_LIBRARY_H
