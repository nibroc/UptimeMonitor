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

const char* string_buffer_get(const StringBuffer* s);

size_t string_buffer_len(const StringBuffer* s);

bool string_buffer_set(StringBuffer* s, const char* str, size_t len);
bool string_buffer_setc(StringBuffer* s, const char* str);
bool string_buffer_setb(StringBuffer* dst, const StringBuffer* src);

bool string_buffer_append(StringBuffer* s, const char* str, size_t len);
bool string_buffer_appendc(StringBuffer* s, const char* str);
bool string_buffer_appendb(StringBuffer* dst, const StringBuffer* src);

bool string_buffer_reserve(StringBuffer* s, size_t min_cap);

void string_buffer_clear(StringBuffer* s);

#ifdef	__cplusplus
}
#endif

#endif

