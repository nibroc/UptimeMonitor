#include "hostname.h"

#include <errno.h>
#include <unistd.h>

enum procparse_result procparse_hostname(char* hn, size_t len) {
	if (gethostname(hn, len) == 0) {
		return PROCPARSE_SUCCESS;
	} else {
		/* Check errno */
		switch (errno) {
			case ENAMETOOLONG:
				return PROCPARSE_ERR_BUFSMALL;
			case EINVAL:
			case EFAULT:
				return PROCPARSE_ERR_INVAL;
			default:
				return PROCPARSE_ERR_OTHER;
		}
	}
}
