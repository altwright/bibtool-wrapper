#include "library.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "altcore/memory.h"
#include "altcore/arenas.h"
#include "chicago.h"
#include "types.h"
#include "records.h"
#include "bibtool_guard.h"

static DB g_db = NoDB;
static Arena g_arena = {};
static bool g_bib_initialized = false;

void bib_init(const char *program_name) {
    if (!g_bib_initialized) {
        i64 malloc_cap = 1024LL * 1024LL;
        alt_init(malloc_cap);

        g_arena = arena_make(malloc_cap / 2);

        Arena tmp = g_arena;

        string dup_program_name = str_make(&tmp, "%s", program_name);

        init_error(stderr);
        init_bibtool(dup_program_name.data);

        g_bib_initialized = true;
    }
}

void bib_uninit() {
    if (g_bib_initialized) {
        arena_free(&g_arena);
        alt_uninit();

        g_bib_initialized = false;
    }
}

bool bib_open_db(const char *filepath) {
    bool result = true;

    g_db = new_db();

    Arena tmp = g_arena;

    string dup_filepath = str_make(&tmp, "%s", filepath);
    result = read_db(g_db, (String) dup_filepath.data, false);

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

    Arena tmp = g_arena;

    Record record = record_get_from_db(&tmp, g_db, citation_key);
    if (record == RecordNULL) {
        return nullptr;
    }

    EntryType record_type_enum = record_get_entry_type(&tmp, record);

    if (record_type_enum == ENTRY_TYPE_COUNT) {
        return nullptr;
    }

    string html_str = {};

    switch (style) {
        case CITE_STYLE_CHICAGO: {
            switch (record_type_enum) {
                case ENTRY_TYPE_BOOK: {
                    html_str = chicago_create_book_bib_entry_html(&tmp, record);
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

    return out_html;
}

char *bib_create_note_html(const char *citation_key, CitationStyle style, const char *section_ref) {
    if (!g_db || !citation_key) {
        return nullptr;
    }

    Arena tmp = g_arena;

    Record record = record_get_from_db(&tmp, g_db, citation_key);
    if (record == RecordNULL) {
        return nullptr;
    }

    EntryType record_type_enum = record_get_entry_type(&tmp, record);
    if (record_type_enum == ENTRY_TYPE_COUNT) {
        return nullptr;
    }

    string html_str = {};
    switch (style) {
        case CITE_STYLE_CHICAGO: {
            switch (record_type_enum) {
                case ENTRY_TYPE_BOOK: {
                    html_str = chicago_create_book_note_html(&tmp, record, section_ref);
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

    char *out_html = malloc(sizeof(char) * (html_str.len + 1));
    assert(out_html);
    memcpy(out_html, html_str.data, html_str.len);
    out_html[html_str.len] = '\0';

    return out_html;
}

char *bib_create_short_note_html(const char *citation_key, CitationStyle style, const char *section_ref) {
    if (!g_db || !citation_key) {
        return nullptr;
    }

    Arena tmp = g_arena;

    Record record = record_get_from_db(&tmp, g_db, citation_key);
    if (record == RecordNULL) {
        return nullptr;
    }

    EntryType record_type_enum = record_get_entry_type(&tmp, record);
    if (record_type_enum == ENTRY_TYPE_COUNT) {
        return nullptr;
    }

    string html_str = {};

    switch (style) {
        case CITE_STYLE_CHICAGO: {
            switch (record_type_enum) {
                case ENTRY_TYPE_BOOK: {
                    html_str = chicago_create_book_short_note_html(&tmp, record, section_ref);
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

    char *out_html = malloc(sizeof(char) * (html_str.len + 1));
    assert(out_html);
    memcpy(out_html, html_str.data, html_str.len);
    out_html[html_str.len] = '\0';

    return out_html;
}
