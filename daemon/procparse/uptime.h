#ifndef PROCPARSE_UPTIME_H
#define PROCPARSE_UPTIME_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ProcParseUptime {
	double total;
	double idle;
};

enum procparse_result procparse_uptime(struct ProcParseUptime* up);

#ifdef __cplusplus
}
#endif

#endif
