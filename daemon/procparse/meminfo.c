#include "meminfo.h"

#include <stdio.h>

int meminfo(struct MemInfo* mem)
{

	int matched;
	char buf[128];

	FILE* f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return 1;
	}

	matched = fscanf(f, "%*s %d", &mem->total);

	/* Astoudingly lazy way to gobble the rest of the line... */
	if (!fgets(buf, 128, f)) {
		fclose(f);
		return 3;
	}

	matched += fscanf(f, "%*s %d", &mem->free);

	fclose(f);

	if (matched == 2) {
		return 0;
	}

	return 2;
}
