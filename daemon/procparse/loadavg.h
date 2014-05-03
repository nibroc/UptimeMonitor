#ifndef PROCPARSE_LOADAVG_H
#define PROCPARSE_LOADAVG_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct LoadAvg {
	float load1;
	float load5;
	float load15;
	int runningProcs;
	int totalProcs;
	int lastProcId;
};

int loadavg(struct LoadAvg* avg);

#ifdef __cplusplus
}
#endif

#endif
