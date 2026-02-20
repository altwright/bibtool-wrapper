//
// Created by wright on 2/14/26.
//

#ifndef BIBTOOL_WRAPPER_RECORDS_H
#define BIBTOOL_WRAPPER_RECORDS_H

#include "altcore/types.h"
#include "altcore/strings.h"
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
    X(TITLE) \
    X(SERIES) \
    X(YEAR) \
    X(TRANSLATOR) \
    X(PUBLISHER) \
    X(ADDRESS) \
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
} Name;

typedef struct {
    ARRAY_TYPE(Name)
    string str;
} Names;

EntryType record_get_entry_type(Arena* arena, Record record);

string record_get_value_str(Arena* arena, Record record, FieldType field);

Names record_get_names(Arena* arena, Record record, FieldType field);

Record record_get_from_db(Arena* arena, DB db, const char* key);

#endif //BIBTOOL_WRAPPER_RECORDS_H
