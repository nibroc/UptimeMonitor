#include "hostname.h"

#include <stdio.h>

int hostname(char* hn, int mlen)
{

	FILE* f = fopen("/etc/hostname", "r");

	if (1 == fscanf(f, "%s", hn)) {
		fclose(f);
		return 0;
	}

	fclose(f);

	return 1;

}
