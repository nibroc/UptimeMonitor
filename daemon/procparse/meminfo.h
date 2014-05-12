#ifndef PROCPARSE_MEMINFO_H
#define PROCPARSE_MEMINFO_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ProcParseMemInfo {
	int total;
	int free;
};

enum procparse_result procparse_meminfo(struct ProcParseMemInfo* mem);

#ifdef __cplusplus
}
#endif

#endif
