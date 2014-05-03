#ifndef PROCPARSE_MEMINFO_H
#define PROCPARSE_MEMINFO_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct MemInfo {
	int total;
	int free;
};

int meminfo(struct MemInfo* mem);

#ifdef __cplusplus
}
#endif

#endif
