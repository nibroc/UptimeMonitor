#include "loadavg.h"

#include "stdio.h"

enum procparse_result procparse_loadavg(struct ProcParseLoadAvg* avg) {
	int matched;

	FILE* f = fopen("/proc/loadavg", "r");

	if (f == NULL) {
		return PROCPARSE_ERR_FILEOPEN;
	}

	matched = fscanf(f, "%f %f %f %d/%d %d", &avg->load1, &avg->load5, &avg->load15,
						&avg->runningProcs, &avg->totalProcs, &avg->lastProcId);

	fclose(f);

	if (matched == 6) {
		return PROCPARSE_SUCCESS;
	}

	return PROCPARSE_ERR_PARSE;
}
