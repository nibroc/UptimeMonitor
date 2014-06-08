#include "string_buffer.h"

#include <stdlib.h>
#include <string.h>

void string_buffer_init(string_buffer* s) {
	s->small_str[0] = '\0';
	s->str = NULL;
	s->len = 0;
	s->cap = sizeof(s->small_str);
}

void string_buffer_cleanup(string_buffer* s) {
	free(s->str);
	s->len = 0;
	s->cap = sizeof(s->small_str);
}

const char* string_buffer_get(const string_buffer* s) {
	return (s->cap <= sizeof(s->small_str)) ? s->small_str : s->str;
}

static char* string_buffer_buf(string_buffer* s) {
	return (char*) string_buffer_get(s);
}

size_t string_buffer_len(const string_buffer* s) {
	return s->len;
}

bool string_buffer_reserve(string_buffer* s, size_t min_cap) {
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

bool string_buffer_set(string_buffer* s, const char* str, size_t len) {
	if (!string_buffer_reserve(s, len + 1)) {
		return false;
	}
	char* buf = string_buffer_buf(s);
	memcpy(buf, str, len);
	buf[len] = '\0';
	s->len = len;
	return true;
}
bool string_buffer_setc(string_buffer* s, const char* str) {
	return string_buffer_set(s, str, strlen(str));
}

bool string_buffer_setb(string_buffer* dst, const string_buffer* src) {
	return string_buffer_set(dst, string_buffer_get(src), string_buffer_len(src));
}

bool string_buffer_append(string_buffer* s, const char* str, size_t len) {
	if (!string_buffer_reserve(s, s->len + len + 1)) {
		return false;
	}
	char* dst = string_buffer_buf(s) + s->len;
	memcpy(dst, str, len);
	dst[len] = '\0';
	s->len += len;
	return true;
}

bool string_buffer_appendc(string_buffer* s, const char* str) {
	return string_buffer_append(s, str, strlen(str));
}

bool string_buffer_appendb(string_buffer* dst, const string_buffer* src) {
	return string_buffer_append(dst, string_buffer_get(src), string_buffer_len(src));
}

void string_buffer_clear(string_buffer* s) {
	s->len = 0;
	string_buffer_buf(s)[0] = '\0';
}
