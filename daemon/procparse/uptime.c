
#include "uptime.h"

#include <stdio.h>

enum procparse_result uptime(struct Uptime* up)
{
	FILE* f = fopen("/proc/uptime", "r");

	if (f == NULL) {
		return PROCPARSE_ERR_FILEOPEN;
	}

	int matched = fscanf(f, "%lf %lf", &up->total, &up->idle);

	fclose(f);

	if (matched == 2) {
		return PROCPARSE_SUCCESS;
	}

	return PROCPARSE_ERR_PARSE;
}
