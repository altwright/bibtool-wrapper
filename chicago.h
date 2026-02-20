//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_CHICAGO_H
#define BIBTOOL_WRAPPER_CHICAGO_H

#include "altcore/strings.h"
#include "altcore/arenas.h"
#include "bibtool_guard.h"

string chicago_create_book_bib_entry_html(Arena* arena, Record record);

string chicago_create_book_note_html(Arena* arena, Record record, const char* section_ref);

string chicago_create_book_short_note_html(Arena* arena, Record record, const char* section_ref);

#endif //BIBTOOL_WRAPPER_CHICAGO_H
