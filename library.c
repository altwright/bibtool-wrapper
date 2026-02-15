#include "library.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "chicago.h"
#include "types.h"
#include "records.h"
#include "bibtool_guard.h"
#include "arena.h"

static DB g_db = NoDB;

void bib_init(const char *program_name) {
    arena_init(1024ULL * 1024ULL);

    string dup_program_name = str_make("%s", program_name);

    init_error(stderr);
    init_bibtool(dup_program_name.data);

    arena_free(0);
}

void bib_uninit() {
    arena_uninit();
}

bool bib_open_db(const char *filepath) {
    bool result = true;

    g_db = new_db();

    string dup_filepath = str_make("%s", filepath);
    result = read_db(g_db, (String) dup_filepath.data, false);

    arena_free(0);

    return result;
}

void bib_close_db() {
    free_db(g_db);
    g_db = NoDB;
}

char *bib_create_bib_entry_html(const char *citation_key, CitationStyle style) {
    if (!g_db || !citation_key) {
        return nullptr;
    }

    arena_free(0);

    string key_str = str_make("%s", citation_key);
    str_to_lower(&key_str);

    Symbol key_symbol = symbol((String) key_str.data);
    if (!key_symbol) {
        return nullptr;
    }

    Record record = db_find(g_db, key_symbol);
    if (record == RecordNULL) {
        return nullptr;
    }

    EntryType record_type_enum = record_get_entry_type(record);

    if (record_type_enum == ENTRY_TYPE_COUNT) {
        return nullptr;
    }

    string html_str = {};

    switch (style) {
        case CITE_STYLE_CHICAGO: {
            switch (record_type_enum) {
                case ENTRY_TYPE_BOOK: {
                    html_str = chicago_create_book_bib_entry_html(record);
                    break;
                }
                default:
                    return nullptr;
            }
            break;
        }
        default:
            return nullptr;
    }

    char *out_html = malloc(html_str.len + 1);
    assert(out_html);
    memcpy(out_html, html_str.data, html_str.len);
    out_html[html_str.len] = '\0';

    arena_free(0);

    return out_html;
}
