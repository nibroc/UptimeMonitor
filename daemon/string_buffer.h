#ifndef UPMON_STRING_BUFFER_H
#define	UPMON_STRING_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

typedef struct StringBuffer {
	char* str;
	size_t len;
	size_t cap;
	char small_str[64];
} StringBuffer;

void string_buffer_init(StringBuffer* s);

void string_buffer_cleanup(StringBuffer* s);

const char* string_buffer_cstr(const StringBuffer* s);

size_t string_buffer_length(const StringBuffer* s);

bool string_buffer_set_bytes(StringBuffer* s, const char* str, size_t len);
bool string_buffer_set_cstr(StringBuffer* s, const char* str);
bool string_buffer_set_string_buffer(StringBuffer* dst, const StringBuffer* src);

bool string_buffer_append_bytes(StringBuffer* s, const char* str, size_t len);
bool string_buffer_append_cstr(StringBuffer* s, const char* str);
bool string_buffer_append_string_buffer(StringBuffer* dst, const StringBuffer* src);

void string_buffer_clear(StringBuffer* s);

#ifdef	__cplusplus
}
#endif

#endif

