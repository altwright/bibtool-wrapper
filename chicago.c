//
// Created by wright on 2/14/26.
//

#include "chicago.h"

#include <ctype.h>

#include "chars.h"
#include "records.h"

static string chicago_format_first_name(string_view first_name_view) {
    string abbrev = {};
    const char *first_name_start = first_name_view.data;
    i64 first_name_start_len = first_name_view.len;

    if (isspace(first_name_start[0])) {
        i64 next_nonspace_idx = find_next_nonspace_char_idx(first_name_start);

        if (next_nonspace_idx < 0) {
            return abbrev;
        }

        first_name_start += next_nonspace_idx;
        first_name_start_len -= next_nonspace_idx;
    }

    abbrev = str_make("");

    i64s space_indexes = {};
    ARRAY_MAKE(&space_indexes);

    for (i64 char_idx = 0; char_idx < first_name_start_len; char_idx++) {
        if (isspace(first_name_start[char_idx])) {
            ARRAY_PUSH(&space_indexes, &char_idx);

            while (isspace(first_name_start[char_idx]) && char_idx < first_name_start_len) {
                char_idx++;
            }
        }
    }

    if (space_indexes.len <= 0) { // First name only
        str_append(&abbrev, "%.*s", first_name_start_len, first_name_start);
    } else {
        str_append(&abbrev, "%.*s ", space_indexes.data[0], first_name_start);

        for (i32 space_indexes_idx = 0; space_indexes_idx < space_indexes.len; space_indexes_idx++) {
            i64 *space_index = &space_indexes.data[space_indexes_idx];
            const char* space_char_start = first_name_start + *space_index;
            i64 next_nonspace_idx = find_next_nonspace_char_idx(space_char_start);
            if (next_nonspace_idx >= 0) {
                const char* middle_name_start = space_char_start + next_nonspace_idx;
                str_append(&abbrev, "%c.", *middle_name_start);

                if (space_indexes_idx < space_indexes.len - 1) {
                    str_append(&abbrev, " ");
                }
            }
        }
    }

    return abbrev;
}

static string chicago_format_subsequent_author_name(const Name *author) {
    string html = str_make("");

    if (author->first_name.data) {
        string abbrev_first_name = chicago_format_first_name(author->first_name);
        if (!str_empty(&abbrev_first_name)) {
            str_append(&html, "%.*s", abbrev_first_name.len, abbrev_first_name.data);
        }

        if (author->last_name.data) {
            str_append(&html, " ");
        }
    }

    if (author->last_name.data) {
        str_append(&html, "%.*s", author->last_name.len, author->last_name.data);

        if (author->suffix.data) {
            str_append(&html, " %.*s", author->suffix.len, author->suffix.data);
        }
    }

    return html;
}

string chicago_create_book_bib_entry_html(Record record) {
    string html = {};

    Names authors = record_get_names(record, FIELD_TYPE_AUTHOR);
    if (ARRAY_EMPTY(&authors)) {
        return html;
    }

    html = str_make("");
    str_append(&html, "<p>");

    for (i32 author_idx = 0; author_idx < authors.len; author_idx++) {
        const Name *author = &authors.data[author_idx];

        if (author_idx == 0) {
            if (author->last_name.data) {
                str_append(&html, "%.*s", author->last_name.len, author->last_name.data);

                if (author->suffix.data) {
                    str_append(&html, " %.*s", author->suffix.len, author->suffix.data);
                }

                str_append(&html, ", ");
            }

            if (author->first_name.data) {
                string abbrev_first_name = chicago_format_first_name(author->first_name);
                if (!str_empty(&abbrev_first_name)) {
                    str_append(&html, "%.*s", abbrev_first_name.len, abbrev_first_name.data);
                }
                str_free(&abbrev_first_name);
            }
        } else {
            string subsequent_name = chicago_format_subsequent_author_name(author);
            str_append(&html, "%s", subsequent_name.data);
            str_free(&subsequent_name);
        }

        if (author_idx < authors.len - 1) {
            str_append(&html, ", and ");
        } else {
            str_append(&html, ". ");
        }
    }

    string translator_val = record_get_value_str(record, FIELD_TYPE_TRANSLATOR);
    bool is_translation = !str_empty(&translator_val);
    if (is_translation) {
        str_append(&html, "Translated by ");
        Names translators_names = record_get_names(record, FIELD_TYPE_TRANSLATOR);

        for (i32 translator_name_idx = 0; translator_name_idx < translators_names.len; translator_name_idx++) {
        }
    }

    names_free(&authors);

    str_append(&html, "</p>");
    return html;
}
