#ifndef PROCPARSE_HOSTNAME_H
#define PROCPARSE_HOSTNAME_H

#include "procparse.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

enum procparse_result hostname(char* hn, size_t len);

#ifdef __cplusplus
}
#endif

#endif
