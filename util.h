/*Time-stamp: <2013-01-15 11:28:12 by Yu Yang>
 * =======================================================================
 *       Filename:  util.h
 *        Created:  2014-08-30 10:36:29
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy1990cn@gmail.com
 * =======================================================================
 */

#ifndef _UTIL_H_
#define _UTIL_H_ 1

#define TRUE 1
#define FALSE 0

#define Free(ptr)            \
do{                          \
  if (ptr != NULL){          \
    free(ptr);               \
    ptr = NULL;              \
  }                          \
}while(0)

void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);

char *readFile(const char *fname);

void checkEqWithMsg(int status, int expect, char *msg);
void checkNeqWithMsg(int status, int expect, char *msg);
void checkLtWithMsg(int status, int expect, char *msg);
void checkLteWithMsg(int status, int expect, char *msg);
void checkGtWithMsg(int status, int expect, char *msg);
void checkGteWithMsg(int status, int expect, char *msg);

/*
 * string utilities
 */
bool strstartswith(const char *str, const char *prefix);
bool strendswith(const char *str, const char *suffix);
int strcount(char *ss, char c);

char *strlstrip(char *str, char *d_chars);
char *strrstrip(char *str, char *d_chars);
char *strstrip(char *str, char *d_chars);

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
    #define DEBUG_PRINT(fmt, args...)    /* do nothing in release build*/
#endif

#endif /* _UTIL_H_ */

