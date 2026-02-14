//
// Created by wright on 2/14/26.
//

#include "records.h"

#include "types.h"

EntryType record_get_entry_type(Record record) {
    rec_type record_type = RecordType(record);
    if (!IsNormalRecord(record_type)) {
        return ENTRY_TYPE_COUNT;
    }

    Symbol record_type_str = get_entry_type(record_type);
    if (!record_type_str) {
        return ENTRY_TYPE_COUNT;
    }

    DynamicString record_type_dyn_str = dyn_str_make("%s", record_type_str);
    dyn_str_to_upper(&record_type_dyn_str);

    EntryType record_type_enum = ENTRY_TYPE_COUNT;

#ifndef X
#define X(type) \
if (strncmp(record_type_dyn_str.data, #type, record_type_dyn_str.len) == 0) \
{ \
record_type_enum = ENTRY_TYPE_##type; \
} \
else
#endif
    X_ENTRY_TYPES {
        return ENTRY_TYPE_COUNT;
    }
#undef X

    return record_type_enum;
}

char *record_get_value_from_field(Record record, const char *field_name_str) {
    if (!field_name_str) {
        return nullptr;
    }

    DynamicString value = {};

    size_t field_name_str_len = strlen(field_name_str);

    Symbol *current_field = RecordHeap(record);

    for (int32_t field_idx = RecordFree(record); field_idx > 0; field_idx -= 2) {
        Symbol field_name_sym = current_field[0];

        if (field_name_sym &&
            strncmp(field_name_str, (const char *) field_name_sym, field_name_str_len) == 0
        ) {
            Symbol field_value_sym = current_field[1];
            size_t field_value_sym_len = strlen((const char *) field_value_sym);

            if (field_value_sym[0] == '{') {
                field_value_sym++;
                field_value_sym_len--;
            }

            if (field_value_sym[field_value_sym_len - 1] == '}') {
                field_value_sym_len--;
            }

            value = dyn_str_make("%.*s", field_value_sym_len, (const char *) field_value_sym);
            break;
        }

        current_field += 2;
    }

    return value.data;
}
