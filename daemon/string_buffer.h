/**
 * @file
 * @brief Declares the interface for manipulation of StringBuffer objects.
 *
 * StringBuffer provides a simple, safe interface to operate on a string. It can
 * be used to build and manipulate strings without having to worry about memory
 * management.
 *
 * At its core, it is simply a manager of a buffer of bytes. It is not aware of
 * character encodings, and it does not provide string related functionalities.
 * It is much closer to a vector of characters than it is to an actual string.
 */

#ifndef UPMON_STRING_BUFFER_H
#define	UPMON_STRING_BUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

/**
 * Internal representation of a StringBuffer.
 *
 * StringBuffer is used internally to track the string contents and meta data.
 * This struct is not meant to be used directly. It should only be manipulated
 * via the string_buffer_* functions.
 *
 * This representation is subjected to change. Neither its members or its size
 * should be considered stable across even small revisions.
*/
typedef struct StringBuffer {
	/** Pointer to a dynamically allocated buffer */
	char* str;
	/** Length of the stored string */
	size_t len;
	/** Number of bytes the buffer can currently hold */
	size_t cap;
	/** Small, embedded buffer so short strings can avoid an allocation */
	char small_str[64];
} StringBuffer;

/** Initialize the StringBuffer <em>s</em> so that it is ready for use. */
void string_buffer_init(StringBuffer* s);

/** Cleanup the StringBuffer <em>s</em> so that it can be disposed. */
void string_buffer_cleanup(StringBuffer* s);

/**
 * Returns a c-string representation of the value of the provided StringBuffer <em>s</em>.
 *
 * Note that the returned pointer is only considered value as long as no mutating
 * operations are performed on <em>s</em>.
 */
const char* string_buffer_cstr(const StringBuffer* s);

/** Returns the length of the contents of <em>s</em> */
size_t string_buffer_length(const StringBuffer* s);

/**
 * Sets the contents of <em>s</em> to the sequence of bytes [str[0], str[len]).
 *
 * Note that the source data is copied and thus does not have to exist any longer
 * than the call to this function.
 */
bool string_buffer_set_bytes(StringBuffer* s, const char* str, size_t len);

/** Set the value of <em>s</em> to the sequence of characters [str[0], str[strlen(str)]) */
bool string_buffer_set_cstr(StringBuffer* s, const char* str);

/** Set the value of <em>dst</em> to the value of <em>src</em> */
bool string_buffer_set_string_buffer(StringBuffer* dst, const StringBuffer* src);

/** Append the sequence of bytes [str[0], str[len]) to <em>s</em> */
bool string_buffer_append_bytes(StringBuffer* s, const char* str, size_t len);

/** Append the sequence of bytes [str[0], str[strlen(str)]) to <em>s</em> */
bool string_buffer_append_cstr(StringBuffer* s, const char* str);

/** Append the value of <em>src</em> to <em>dst</em> */
bool string_buffer_append_string_buffer(StringBuffer* dst, const StringBuffer* src);

/** Clear <em>s</em> so that is it valued as an empty string */
void string_buffer_clear(StringBuffer* s);

#ifdef	__cplusplus
}
#endif

#endif

