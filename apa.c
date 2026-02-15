//
// Created by wright on 2/14/26.
//

#include "apa.h"

#include "records.h"

string apa_create_book_reference_html(Record record) {
    string html = {};

    Authors authors = record_get_authors(record);

    for (i32 author_idx = 0; author_idx < authors.len; author_idx++) {

    }

    string translator_val = record_get_value_str(record, FIELD_TYPE_TRANSLATOR);
    bool is_translation = !str_empty(&translator_val);

    authors_free(&authors);

    return html;
}
