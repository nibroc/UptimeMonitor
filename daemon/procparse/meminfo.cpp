
#include "meminfo.h"

#include <cstdio>

int meminfo(struct MemInfo* mem)
{

	int matched;
	char buf[128];

	FILE* f = fopen("/proc/meminfo", "r");

	if (f == NULL) {
		return 1;
	}

	matched = std::fscanf(f, "%*s %d", &mem->total);

	/* Astoudingly lazy way to gobble the rest of the line... */
	if (!std::fgets(buf, 128, f)) {
		return 3;
	}

	matched += std::fscanf(f, "%*s %d", &mem->free);

	std::fclose(f);

	if (matched == 2) {
		return 0;
	}

	return 2;
}
