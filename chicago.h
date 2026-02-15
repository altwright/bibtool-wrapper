//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_CHICAGO_H
#define BIBTOOL_WRAPPER_CHICAGO_H

#include "bibtool_guard.h"
#include "types.h"

string chicago_create_book_bib_entry_html(Record record);

string chicago_create_book_note_html(Record record, const char* section_ref);

string chicago_create_book_short_note_html(Record record, const char* section_ref);

#endif //BIBTOOL_WRAPPER_CHICAGO_H
