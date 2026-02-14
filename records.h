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

EntryType record_get_entry_type(Record record);

char *record_get_value_from_field(Record record, const char *field_name_str);

#endif //BIBTOOL_WRAPPER_RECORDS_H
