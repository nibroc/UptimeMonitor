#ifndef UPMON_STRING_BUFFER_H
#define	UPMON_STRING_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

typedef struct string_buffer {
	char* str;
	size_t len;
	size_t cap;
	char small_str[64];
} string_buffer;

void string_buffer_init(string_buffer* s);

void string_buffer_cleanup(string_buffer* s);

const char* string_buffer_get(const string_buffer* s);

size_t string_buffer_len(const string_buffer* s);

bool string_buffer_set(string_buffer* s, const char* str, size_t len);
bool string_buffer_setc(string_buffer* s, const char* str);

bool string_buffer_append(string_buffer* s, const char* str, size_t len);
bool string_buffer_appendc(string_buffer* s, const char* str);

bool string_buffer_reserve(string_buffer* s, size_t min_cap);

void string_buffer_clear(string_buffer* s);

#ifdef	__cplusplus
}
#endif

#endif

