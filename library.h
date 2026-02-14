#ifndef BIBTOOL_WRAPPER_LIBRARY_H
#define BIBTOOL_WRAPPER_LIBRARY_H

#include <stdint.h>
#include <stdio.h>

typedef enum : uint32_t {
    CITE_STYLE_APA = 0,
    CITE_STYLE_COUNT
} CitationStyle;

void bib_init(const char* program_name);
bool bib_open_db(const char* filepath);
void bib_close_db();
char* bib_get_reference_html(const char* citation_key, CitationStyle style);

#endif // BIBTOOL_WRAPPER_LIBRARY_H