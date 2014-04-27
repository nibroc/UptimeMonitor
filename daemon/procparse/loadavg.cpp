
#include "loadavg.h"

#include "stdio.h"

int loadavg(struct LoadAvg* avg)
{

	int matched;

	FILE* f = fopen("/proc/loadavg", "r");

	if (f == NULL) {
		return 1;
	}

	matched = fscanf(f, "%f %f %f %d/%d %d", &avg->load1, &avg->load5, &avg->load15, &avg->runningProcs, &avg->totalProcs, &avg->lastProcId);

	fclose(f);

	if (matched == 6) {
		return 0;
	}

	return 2;
}
