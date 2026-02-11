#include "library.h"

#include <assert.h>
#include <stdio.h>
#include <bibtool/bibtool.h>
#include <bibtool/symbols.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef int32_t i32;
typedef int64_t i64;

typedef uint32_t u32;
typedef uint64_t u64;

#ifndef X_ENTRY_TYPES
#define X_ENTRY_TYPES \
    X(ARTICLE) \
    X(BOOK) \
    X(COUNT)
#endif

typedef enum : u32 {
#ifndef X
#define X(type) \
    ENTRY_TYPE_##type,
#endif
    X_ENTRY_TYPES
#undef X
} EntryType;

typedef struct {
    char *data;
    i64 len;
    i64 cap;
} DynamicString;

static DB g_db = NoDB;

static DynamicString dyn_str_make(const char *fmt, ...) {
    va_list args_read, args_write;
    va_start(args_read);
    va_start(args_write);

    i32 len = vsnprintf(nullptr, 0, fmt, args_read);
    i64 cap = ((2 * (len + 1) + 3) >> 2) << 2;
    if (cap < 32) {
        cap = 32;
    }

    DynamicString str = {
        .len = len,
        .cap = cap,
    };

    str.data = calloc(cap, sizeof(char));
    assert(str.data);

    i32 bytes_written = vsnprintf(str.data, len + 1, fmt, args_write);
    assert(bytes_written == len);

    va_end(args_read);
    va_end(args_write);

    return str;
}

static void dyn_str_append(DynamicString *str, const char *fmt, ...) {
    va_list args_read, args_write;
    va_start(args_read);
    va_start(args_write);

    i32 len = vsnprintf(nullptr, 0, fmt, args_read);

    while ((len + 1) > (str->cap - str->len)) {
        i64 new_cap = 2 * str->cap;
        char *new_data = realloc(str->data, new_cap * sizeof(char));
        assert(new_data);
        str->data = new_data;
        str->cap = new_cap;
    }

    i32 bytes_written = vsnprintf(str->data + str->len, len + 1, fmt, args_write);
    assert(bytes_written == len);

    str->len += len;

    va_end(args_read);
    va_end(args_write);
}

static void dyn_str_to_lower(DynamicString *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) tolower(str->data[char_idx]);
    }
}

static void dyn_str_to_upper(DynamicString *str) {
    for (i32 char_idx = 0; char_idx < str->len; char_idx++) {
        str->data[char_idx] = (char) toupper(str->data[char_idx]);
    }
}

static char *dup_string(const char *str) {
    size_t len = strlen(str);
    char *dup = malloc(len + 1);
    strncpy(dup, str, len + 1);
    return dup;
}

void bib_init(const char *program_name) {
    char *dup_program_name = dup_string(program_name);

    init_error(stderr);
    init_bibtool(dup_program_name);

    free(dup_program_name);
}

bool bib_open_db(const char *filepath) {
    bool result = true;

    g_db = new_db();

    String dup_filepath = (String) dup_string(filepath);
    result = read_db(g_db, dup_filepath, false);
    free(dup_filepath);

    return result;
}

void bib_close_db() {
    free_db(g_db);
    g_db = NoDB;
}

int32_t bib_get_entry_count() {
    if (!g_db) {
        return -1;
    }

    int32_t count = 0;
    db_count(g_db, &count);

    return count;
}

void bib_print_db(FILE *fp) {
    if (!g_db || !fp) {
        return;
    }

    print_db(fp, g_db, "n");

    Symbol sym;
    Record record;
}

void bib_print_db_entry(FILE *fp, const char *citation_key) {
    if (!g_db || !fp) {
        return;
    }

    String dup_key = (String) dup_string(citation_key);
    for (int32_t char_idx = 0; char_idx < strlen((const char *) dup_key); char_idx++) {
        dup_key[char_idx] = tolower(dup_key[char_idx]);
    }

    Symbol key_sym = symbol(dup_key);
    free(dup_key);

    if (!key_sym) {
        return;
    }

    Record record = db_find(g_db, key_sym);

    dup_key = nullptr;

    if (!record) {
        return;
    }

    fprintf(fp, "Found %s\n", citation_key);
}

static char* record_get_value_from_field(Record record, const char *field_name_str) {
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

char *bib_get_html_reference(const char *citation_key, CitationStyle style) {
    if (!g_db || !citation_key) {
        return nullptr;
    }

    DynamicString html_str = {};

    DynamicString key_str = dyn_str_make("%s", citation_key);
    dyn_str_to_lower(&key_str);

    Symbol key_symbol = symbol((String) key_str.data);
    if (!key_symbol) {
        return nullptr;
    }

    Record record = db_find(g_db, key_symbol);
    if (record == RecordNULL) {
        return nullptr;
    }

    rec_type record_type = RecordType(record);
    if (!IsNormalRecord(record_type)) {
        return nullptr;
    }

    Symbol record_type_str = get_entry_type(record_type);
    if (!record_type_str) {
        return nullptr;
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
        return nullptr;
    }
#undef X

    if (record_type_enum == ENTRY_TYPE_COUNT) {
        return nullptr;
    }

    switch (style) {
        case CITE_STYLE_APA: {
            switch (record_type_enum) {
                case ENTRY_TYPE_ARTICLE: {
                    char* value_str = record_get_value_from_field(record, "language");
                    if (value_str) {
                        printf("%s\n", value_str);
                    }
                    break;
                }
                case ENTRY_TYPE_BOOK: {
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

    return html_str.data;
}
