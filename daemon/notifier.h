#ifndef UPMON_NOTIFIER_H
#define	UPMON_NOTIFIER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/uptime.h"

typedef struct notifier notifier;

notifier* notifier_create(const char* url);

void notifier_destroy(notifier* n);

int notifier_send(notifier* n, const char* host, struct ProcParseLoadAvg* avg,
					struct ProcParseMemInfo* mem, struct ProcParseUptime* up);

#ifdef	__cplusplus
}
#endif

#endif
