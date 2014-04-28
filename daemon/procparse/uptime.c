
#include "uptime.h"

#include <stdio.h>

int uptime(struct Uptime* up)
{
	
	int matched;
	
	FILE* f = fopen("/proc/uptime", "r");
	
	if (f == NULL) {
		return 1;
	}
	
	matched = fscanf(f, "%lf %lf", &up->total, &up->idle);
	
	fclose(f);
	
	if (matched == 2) {
		return 0;
	}
	
	return 2;
	
}
