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

enum procparse_result meminfo(struct MemInfo* mem);

#ifdef __cplusplus
}
#endif

#endif
