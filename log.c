/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * =======================================================================
 *       Filename:  log.c
 *        Created:  2014-09-02 14:26:09
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy2012cn@NOSPAM.gmail.com
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include "util.h"
#include "json.h"
#include "log.h"

static int numRecords = 0;
static char *logData = NULL;
/*
 * arr contains the pointors to log records which have challenge field.
 */
static char **arr = NULL;
/*
 * read the log file to memory and split it into array.
 */
char **logToArray(const char *logFname)
{
    char *data = readFile(logFname);
    logData = data;             /* store the pointor to global variable*/
    /*
     * TODO: use the correct value returned by splitRecords
     */
    numRecords = strcount(data, '\n');

    return splitRecords(data);
}

char *extractChallenge(char *logRecord)
{
    return getValue(logRecord, "\"challenge\"");
}

/*
 * TODO: resolve the memory link.
 * A memory link. we must free the memory that store the content of
 * the log file
 */
entry_t *getChallengeArray(const char *fname)
{
    char **logArr = logToArray(fname);
    /* char **validArr = malloc(sizeof(char *) * (numRecords + 1)); */
    char *validArr[numRecords + 1];
    char *challenges[numRecords + 1];
    /* the number of the records that have challenge field */
    int numValid = 0;
    for(char **ele=logArr; *ele != NULL; ele++)
    {
        char *challenge = extractChallenge(*ele);
        /*
         * some log records don't have challenge field, we must
         * delete these records
         */
        if(challenge != NULL){
            printf("challenge: %s ( %d)\n", challenge, numValid+1);

            validArr[numValid] = *ele;
            challenges[numValid] = challenge;
            numValid++;
        }
    }
    entry_t *entries = malloc(sizeof(entry_t) * numValid);
    /*
     * arr is a global variable, we need use this varible to fetch
     * log record through index
     */
    arr = malloc(sizeof(char *) * (numValid + 1));
    assert(entries != NULL);
    assert(arr != NULL);
    /* copy the content of logAttr to arr. */
    memcpy(arr, validArr, numValid*sizeof(char*));
    *(arr+numValid) = NULL;

    for(int i = 0; i < numValid; i++)
    {
        entry_t *p = entries + i;
        p->idx = i;
        strncpy(p->challenge, challenges[i], CHALLENGE_LEN);
        p->challenge[CHALLENGE_LEN] = '\0';
    }
    /* release the resources */
    Free(logArr);
    for(int i=0; i < numValid; i++)
    {
        Free(challenges[i]);
    }

    return entries;
}

char *entryToLog(entry_t *e)
{
    int idx = e->idx;
    return arr[idx];
}

void releaseResource()
{
    Free(logData);
    Free(arr);
}
