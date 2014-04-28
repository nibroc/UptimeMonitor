#include "meminfo.h"

#include <stdio.h>

int meminfo(struct MemInfo* mem)
{
	int matched;

	FILE* f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return 1;
	}

	matched = fscanf(f, "%*s %d %*s\n", &mem->total);
	matched += fscanf(f, "%*s %d %*s\n", &mem->free);

	fclose(f);

	return (matched == 2) ? 0 : 2;
}
