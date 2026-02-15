//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_RECORDS_H
#define BIBTOOL_WRAPPER_RECORDS_H

#include "types.h"
#include "bibtool_guard.h"

typedef enum : u32 {
#ifndef X_ENTRY_TYPES
#define X_ENTRY_TYPES \
X(ARTICLE) \
X(BOOK) \
X(COUNT)
#endif

#ifndef X
#define X(type) \
ENTRY_TYPE_##type,
#endif
    X_ENTRY_TYPES
#undef X
} EntryType;

typedef enum : u32 {
#ifndef X_FIELD_TYPES
#define X_FIELD_TYPES \
    X(AUTHOR) \
    X(TRANSLATOR) \
    X(COUNT)
#endif

#ifndef X
#define X(type) \
    FIELD_TYPE_##type,
#endif
    X_FIELD_TYPES
#undef X
} FieldType;

typedef struct {
    string_view first_name;
    string_view last_name;
    string_view suffix;
} Author;

typedef struct {
    ARRAY(Author)
    string str;
} Authors;

EntryType record_get_entry_type(Record record);

string record_get_value_str(Record record, FieldType field);

Authors record_get_authors(Record record);

void authors_free(Authors* authors);

#endif //BIBTOOL_WRAPPER_RECORDS_H
