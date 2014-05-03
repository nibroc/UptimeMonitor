#ifndef PROCPARSE_UPTIME_H
#define PROCPARSE_UPTIME_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Uptime {
	double total;
	double idle;
};

int uptime(struct Uptime* up);

#ifdef __cplusplus
}
#endif

#endif
