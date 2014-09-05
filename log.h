/*Time-stamp: <2013-01-15 11:28:12 by Yu Yang>
 * =======================================================================
 *       Filename:  log.h
 *        Created:  2014-09-03 11:53:39
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy1990cn@gmail.com
 * =======================================================================
 */

#ifndef _LOG_H_
#define _LOG_H_ 1

#define CHALLENGE_LEN 32

extern int numChallenges;

typedef struct entry_s
{
    char challenge[CHALLENGE_LEN+1];
    int idx;
}entry_t;

char **logToArray(const char *logFname);
char *extractChallenge(char *data);

entry_t *getChallengeArray(const char *fname);
char *entryToLog(entry_t *e);
void releaseLogResource();

void writeSortedLogToFile(entry_t *entries, int length, const char *outpuFname);
/* int getNumChallenges(); */
#endif /* _LOG_H_ */

