#include "hostname.h"

#include <unistd.h>

int hostname(char* hn, size_t len)
{
	if (gethostname(hn, len) == 0) {
		return 0;
	} else {
		/* Check errno */
		return 1;
	}
}
