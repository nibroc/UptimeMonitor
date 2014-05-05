#ifndef PROCPARSE_PROCPARSE_H
#define	PROCPARSE_PROCPARSE_H

#ifdef	__cplusplus
extern "C" {
#endif

enum procparse_result {
	PROCPARSE_SUCCESS = 0,
	PROCPARSE_ERR_FILEOPEN = 1,
	PROCPARSE_ERR_FILEREAD = 2,
	PROCPARSE_ERR_PARSE = 3,
	PROCPARSE_ERR_BUFSMALL = 4,
	PROCPARSE_ERR_INVAL = 5,
	PROCPARSE_ERR_OTHER = 6
};

const char* procparse_strerr(enum procparse_result rc);

#ifdef	__cplusplus
}
#endif

#endif

