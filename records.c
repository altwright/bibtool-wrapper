//
// Created by wright on 2/14/26.
//

#include "records.h"

#include <ctype.h>

#include "types.h"
#include <string.h>

static const char *k_field_type_strs[] = {
#ifndef X
#define X(type) \
    #type,
#endif
    X_FIELD_TYPES
#undef X
};

EntryType record_get_entry_type(Record record) {
    rec_type record_type = RecordType(record);
    if (!IsNormalRecord(record_type)) {
        return ENTRY_TYPE_COUNT;
    }

    Symbol record_type_str = get_entry_type(record_type);
    if (!record_type_str) {
        return ENTRY_TYPE_COUNT;
    }

    string record_type_dyn_str = str_make("%s", record_type_str);
    str_to_upper(&record_type_dyn_str);

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

static string record_get_value_from_field_str(Record record, const char *field_name_str) {
    string value = {};

    if (!field_name_str) {
        return value;
    }

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

            value = str_make("%.*s", field_value_sym_len, (const char *) field_value_sym);
            break;
        }

        current_field += 2;
    }

    return value;
}

static const char *field_type_to_str(FieldType type) {
    const char *field_type_str = nullptr;

    if (type < FIELD_TYPE_COUNT) {
        field_type_str = k_field_type_strs[type];
    }

    return field_type_str;
}

string record_get_value_str(Record record, FieldType field) {
    string value = {};

    string field_str = str_make(field_type_to_str(field));
    if (str_empty(&field_str)) {
        return value;
    }

    str_to_lower(&field_str);

    value = record_get_value_from_field_str(record, field_str.data);

    return value;
}

static i64 find_next_nonspace_char_idx(const char *str) {
    i64 idx = -1;

    if (!str) {
        return idx;
    }

    i64 str_len = (i64) strlen(str);

    for (i64 char_idx = 1; char_idx < str_len; char_idx++) {
        if (!isspace(str[char_idx])) {
            idx = char_idx;
            break;
        }
    }

    return idx;
}

static i64 find_next_space_char_idx(const char *str) {
    i64 idx = -1;

    if (!str) {
        return idx;
    }

    i64 str_len = (i64) strlen(str);

    for (i64 char_idx = 1; char_idx < str_len; char_idx++) {
        if (isspace(str[char_idx])) {
            idx = char_idx;
            break;
        }
    }

    return idx;
}

Authors record_get_authors(Record record) {
    Authors authors = {};

    authors.str = record_get_value_str(record, FIELD_TYPE_AUTHOR);

    if (!str_empty(&authors.str)) {
        authors.cap = 4;
        ARRAY_MAKE(&authors);

        string lowered_author_str = str_dup(&authors.str);
        str_to_lower(&lowered_author_str);

        i64s and_substr_indexes = {
            .cap = 4
        };
        ARRAY_MAKE(&and_substr_indexes);

        char *and_substr_start = lowered_author_str.data;

        const char *and_str = "and";
        size_t and_str_len = strlen(and_str);

        while (and_substr_start = strstr(and_substr_start, and_str), and_substr_start) {
            i64 and_substr_start_idx = and_substr_start - lowered_author_str.data;
            ARRAY_PUSH(&and_substr_indexes, &and_substr_start_idx);

            and_substr_start += and_str_len;
        }

        char *author_name_start = authors.str.data;
        bool not_empty = true;

        if (isspace(author_name_start[0])) {
            i64 next_nonspace_idx = find_next_nonspace_char_idx(author_name_start);
            if (next_nonspace_idx < 0) {
                not_empty = false;
            }

            author_name_start += next_nonspace_idx;
        }

        if (not_empty) {
            for (i32 author_idx = 0; author_idx < and_substr_indexes.len + 1; author_idx++) {
                Author author = {};

                string_view author_str_view = {
                    .data = author_name_start,
                };

                if (author_idx < and_substr_indexes.len) {
                    i64 author_name_start_idx = author_name_start - authors.str.data;
                    i64 next_and_substr_idx = and_substr_indexes.data[author_idx];
                    author_str_view.len = next_and_substr_idx - author_name_start_idx;
                } else {
                    author_str_view.len = (i64) strlen(author_name_start);
                }

                i64s comma_indexes = {
                    .cap = 4
                };
                ARRAY_MAKE(&comma_indexes);

                for (i64 char_idx = 0; char_idx < author_str_view.len; char_idx++) {
                    if (author_str_view.data[char_idx] == ',') {
                        ARRAY_PUSH(&comma_indexes, &char_idx);
                    }
                }

                if (ARRAY_EMPTY(&comma_indexes)) {
                    // {Firstname Lastname}
                    author.first_name.data = author_str_view.data;
                    author.first_name.len = author_str_view.len;
                    i64 next_space_idx = find_next_space_char_idx(author_str_view.data);
                    if (next_space_idx >= 0 && next_space_idx < author_str_view.len) {
                        author.first_name.len = next_space_idx;

                        const char *last_name_str = author_str_view.data + next_space_idx;
                        i64 next_nonspace_idx = find_next_nonspace_char_idx(last_name_str);
                        if (next_nonspace_idx < 0) {
                            last_name_str = nullptr;
                        } else {
                            last_name_str += next_nonspace_idx;
                        }

                        if (last_name_str) {
                            author.last_name.data = last_name_str;
                            author.last_name.len = (author_str_view.data + author_str_view.len) - last_name_str;
                        }
                    }
                } else {
                    // {Lastname, ... }

                    author.last_name.data = author_str_view.data;
                    author.last_name.len = comma_indexes.data[0];
                    i64 next_space_idx = find_next_space_char_idx(author_str_view.data + comma_indexes.data[0]);
                    if (next_space_idx >= 0 && next_space_idx < (author_str_view.len - comma_indexes.data[0])) {
                        if (comma_indexes.len == 1) {
                            // {Lastname, Firstname}

                            const char *first_name_str = author_str_view.data + comma_indexes.data[0] + next_space_idx;
                            i64 next_nonspace_idx = find_next_nonspace_char_idx(first_name_str);
                            if (next_nonspace_idx < 0) {
                                first_name_str = nullptr;
                            } else {
                                first_name_str += next_nonspace_idx;
                            }

                            if (first_name_str) {
                                author.first_name.data = first_name_str;
                                author.first_name.len = (author_str_view.data + author_str_view.len) - first_name_str;
                            }
                        } else {
                            // {Lastname, Suffix, Firstname}

                            const char *suffix_str = author_str_view.data + next_space_idx;
                            i64 next_nonspace_idx = find_next_nonspace_char_idx(suffix_str);
                            if (next_nonspace_idx < 0) {
                                suffix_str = nullptr;
                            } else {
                                suffix_str += next_nonspace_idx;
                            }

                            if (suffix_str) {
                                author.suffix.data = suffix_str;
                                author.suffix.len = comma_indexes.data[1] - comma_indexes.data[0] - 1;
                            }

                            const char *first_name_str = author_str_view.data + comma_indexes.data[1] + 1;
                            next_nonspace_idx = find_next_nonspace_char_idx(first_name_str);
                            if (next_nonspace_idx < 0) {
                                first_name_str = nullptr;
                            } else {
                                first_name_str += next_nonspace_idx;
                            }

                            if (first_name_str) {
                                author.first_name.data = first_name_str;
                                author.first_name.len = (author_str_view.data + author_str_view.len) - first_name_str;
                            }
                        }
                    }
                }

                ARRAY_FREE(&comma_indexes);

                ARRAY_PUSH(&authors, &author);

                if (author_idx < and_substr_indexes.len) {
                    author_name_start = authors.str.data + and_substr_indexes.data[author_idx] + and_str_len;
                    if (isspace(author_name_start[0])) {
                        i64 next_nonspace_idx = find_next_nonspace_char_idx(author_name_start);
                        if (next_nonspace_idx < 0) {
                            break;
                        }

                        author_name_start += next_nonspace_idx;
                    }
                }
            }
        }

        str_free(&lowered_author_str);
        ARRAY_FREE(&and_substr_indexes);
    }

    return authors;
}
