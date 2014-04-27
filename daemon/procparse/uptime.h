#ifndef __PP_UPTIME_H
#define __PP_UPTIME_H

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
