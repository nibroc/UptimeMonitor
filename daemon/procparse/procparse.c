#include "procparse.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static const char* msg[] = {
	"no error", /* PROCPARSE_SUCCESS */
	"could not open file", /* PROCPARSE_ERR_FILEOPEN */
	"could not read file", /* PROCPARSE_ERR_FILEREAD */
	"could not parse data", /* PROCPARSE_ERR_PARSE */
	"buffer too small", /* PROCPARSE_ERR_BUFSMALL */
	"invalid input values", /* PROCPARSE_ERR_INVAL */
	"other" /* PROCPARSE_ERR_OTHER */
};

const char* procparse_strerr(enum procparse_result rc) {
	assert(rc >= 0 && rc < sizeof(msg) / sizeof(msg[0]));
	return msg[rc];
}
