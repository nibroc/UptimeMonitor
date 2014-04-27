#ifndef __POST_H_
#define __POST_H_

#include "procparse/hostname.h"
#include "procparse/meminfo.h"
#include "procparse/uptime.h"
#include "procparse/loadavg.h"

int post(const char* url, const char* host, const struct Uptime* up, const struct MemInfo* mem, const struct LoadAvg* avg);

#endif
