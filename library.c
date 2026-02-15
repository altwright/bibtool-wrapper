#include "library.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "chicago.h"
#include "types.h"
#include "records.h"
#include "bibtool_guard.h"

static DB g_db = NoDB;

void bib_init(const char *program_name) {
    string dup_program_name = str_make("%s", program_name);

    init_error(stderr);
    init_bibtool(dup_program_name.data);

    str_free(&dup_program_name);
}

bool bib_open_db(const char *filepath) {
    bool result = true;

    g_db = new_db();

    string dup_filepath = str_make("%s", filepath);
    result = read_db(g_db, (String) dup_filepath.data, false);

    str_free(&dup_filepath);

    return result;
}

void bib_close_db() {
    free_db(g_db);
    g_db = NoDB;
}

char *bib_get_reference_html(const char *citation_key, CitationStyle style) {
    if (!g_db || !citation_key) {
        return nullptr;
    }

    string html_str = {};

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

    switch (style) {
        case CITE_STYLE_CHICAGO: {
            switch (record_type_enum) {
                case ENTRY_TYPE_ARTICLE:
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

    return html_str.data;
}
