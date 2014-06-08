#include "string_buffer.h"

#include <stdlib.h>
#include <string.h>

void string_buffer_init(StringBuffer* s) {
	s->small_str[0] = '\0';
	s->str = NULL;
	s->len = 0;
	s->cap = sizeof(s->small_str);
}

void string_buffer_cleanup(StringBuffer* s) {
	free(s->str);
	s->len = 0;
	s->cap = sizeof(s->small_str);
}

const char* string_buffer_cstr(const StringBuffer* s) {
	return (s->cap <= sizeof(s->small_str)) ? s->small_str : s->str;
}

static char* string_buffer_buf(StringBuffer* s) {
	return (char*) string_buffer_cstr(s);
}

size_t string_buffer_len(const StringBuffer* s) {
	return s->len;
}

bool string_buffer_reserve(StringBuffer* s, size_t min_cap) {
	if (s->cap >= min_cap) {
		return true;
	}

	char* new_buf = realloc(s->str, min_cap);
	if (new_buf == NULL) {
		return false;
	}

	if (s->str == NULL) {
		memcpy(new_buf, s->small_str, sizeof(s->small_str));
	}

	s->str = new_buf;
	s->cap = min_cap;

	return true;
}

bool string_buffer_set(StringBuffer* s, const char* str, size_t len) {
	if (!string_buffer_reserve(s, len + 1)) {
		return false;
	}
	char* buf = string_buffer_buf(s);
	memcpy(buf, str, len);
	buf[len] = '\0';
	s->len = len;
	return true;
}
bool string_buffer_setc(StringBuffer* s, const char* str) {
	return string_buffer_set(s, str, strlen(str));
}

bool string_buffer_setb(StringBuffer* dst, const StringBuffer* src) {
	return string_buffer_set(dst, string_buffer_cstr(src), string_buffer_len(src));
}

bool string_buffer_append(StringBuffer* s, const char* str, size_t len) {
	if (!string_buffer_reserve(s, s->len + len + 1)) {
		return false;
	}
	char* dst = string_buffer_buf(s) + s->len;
	memcpy(dst, str, len);
	dst[len] = '\0';
	s->len += len;
	return true;
}

bool string_buffer_appendc(StringBuffer* s, const char* str) {
	return string_buffer_append(s, str, strlen(str));
}

bool string_buffer_appendb(StringBuffer* dst, const StringBuffer* src) {
	return string_buffer_append(dst, string_buffer_cstr(src), string_buffer_len(src));
}

void string_buffer_clear(StringBuffer* s) {
	s->len = 0;
	string_buffer_buf(s)[0] = '\0';
}
