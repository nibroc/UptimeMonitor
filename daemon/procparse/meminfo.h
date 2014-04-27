#ifndef __PP_MEMINFO_H
#define __PP_MEMINFO_H

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
