#include "meminfo.h"

#include <stdio.h>

int meminfo(struct MemInfo* mem)
{
	FILE* f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return PROCPARSE_ERR_FILEOPEN;
	}

	int matched = fscanf(f, "%*s %d %*s\n", &mem->total);
	matched += fscanf(f, "%*s %d %*s\n", &mem->free);

	fclose(f);

	return (matched == 2) ? PROCPARSE_SUCCESS : PROCPARSE_ERR_PARSE;
}
