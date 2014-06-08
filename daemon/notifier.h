#ifndef UPMON_NOTIFIER_H
#define	UPMON_NOTIFIER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/uptime.h"

typedef enum NotifierResult {
	NOTIFIER_SUCCESS = 0,
	NOTIFIER_ERR_RESPONSE = 1,
	NOTIFIER_ERR_CONNECTION = 2
} NotifierResult;

typedef struct Notifier Notifier;

Notifier* notifier_create(const char* url);

void notifier_destroy(Notifier* n);

const char* notifier_error(const Notifier* n);

NotifierResult notifier_send(Notifier* n, const char* host, struct ProcParseLoadAvg* avg,
								struct ProcParseMemInfo* mem, struct ProcParseUptime* up);

#ifdef	__cplusplus
}
#endif

#endif
