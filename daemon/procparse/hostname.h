#ifndef PROCPARSE_HOSTNAME_H
#define PROCPARSE_HOSTNAME_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

enum procparse_result procparse_hostname(char* hn, size_t len);

#ifdef __cplusplus
}
#endif

#endif
