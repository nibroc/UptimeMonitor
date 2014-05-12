#ifndef PROCPARSE_LOADAVG_H
#define PROCPARSE_LOADAVG_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ProcParseLoadAvg {
	float load1;
	float load5;
	float load15;
	int runningProcs;
	int totalProcs;
	int lastProcId;
};

enum procparse_result procparse_loadavg(struct ProcParseLoadAvg* avg);

#ifdef __cplusplus
}
#endif

#endif
