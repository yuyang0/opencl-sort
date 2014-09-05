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

/* the pointor to the memory which store the content of the log file */
static char *logData = NULL;
/* the number of records in log file */
static int numRecords = 0;
/* the number of the records that have challenge field */
int numChallenges = 0;
/* arr contains the pointors to log records which have challenge field. */
static char **challengeLogArr = NULL;

/*
 * read the log file to memory and split it into array.
 */

static char *nextRecord(char *data)
{
    int numBraces = 0;
    data = skipSpaces(data);
    assert(*data == '{');

    for(; *data != '\0'; data++)
    {
        if(*data == '{'){
            numBraces++;
        }
        if(*data == '}'){
            numBraces--;
        }
        if(*data == '\"'){
            data = skipString(data);
            data--;             /*  */
        }
        if(*data == '\\'){
            data = skipEscapeChar(data);
            data--;
        }

        if(numBraces == 0){
            data++;             /* ignore  } */
            break;
        }
    }
    if(numBraces > 0){
        printf("unbalanced braces..");
        exit(-1);
    }
    return data;
}

char **logToArray(const char *logFname)
{
    char *data = readFile(logFname);
    logData = data;             /* store the pointor to global variable*/

    numRecords = 0;             /* numRecords is a global variable */

    char *start = data;
    char *end = NULL;
    for(; *data != '\0'; numRecords++)
    {
        start = skipSpaces(start);
        if(*start == '\0'){
            break;
        }
        end = nextRecord(start);
        *end = '\0';
        start = end + 1;
    }
    DEBUG_PRINT("found %d log records in log file.\n", numRecords);
    char **array = malloc(sizeof(char *)*(numRecords+1));
    for(int i = 0; i< numRecords; i++)
    {
        array[i] = data;
        data = data + strlen(data) + 1;
    }
    array[numRecords] = NULL;
    return array;
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
    numChallenges = 0;
    for(char **ele=logArr; *ele != NULL; ele++)
    {
        char *challenge = extractChallenge(*ele);
        /*
         * some log records don't have challenge field, we must
         * delete these records
         */
        if(challenge != NULL){
            // DEBUG_PRINT("challenge: %s ( %d)\n", challenge, numChallenges+1);

            validArr[numChallenges] = *ele;
            challenges[numChallenges] = challenge;
            numChallenges++;
        }
    }
    DEBUG_PRINT("found %d records which have challenge fields\n", numChallenges);
    entry_t *entries = malloc(sizeof(entry_t) * numChallenges);
    /*
     * challengeLogArr is a global variable, we need use this varible to
     * fetch log record through index
     */
    challengeLogArr = malloc(sizeof(char *) * (numChallenges + 1));
    assert(entries != NULL);
    assert(challengeLogArr != NULL);
    /* copy the content of logAttr to challengeLogArr. */
    memcpy(challengeLogArr, validArr, numChallenges*sizeof(char*));
    *(challengeLogArr + numChallenges) = NULL;

    for(int i = 0; i < numChallenges; i++)
    {
        entry_t *p = entries + i;
        p->idx = i;
        strncpy(p->challenge, challenges[i], CHALLENGE_LEN);
        p->challenge[CHALLENGE_LEN] = '\0';
    }
    /* release the resources */
    Free(logArr);
    for(int i=0; i < numChallenges; i++)
    {
        Free(challenges[i]);
    }

    return entries;
}

char *entryToLog(entry_t *e)
{
    int idx = e->idx;
    return challengeLogArr[idx];
}

void releaseResource()
{
    Free(logData);
    Free(challengeLogArr);
}

/*
 * the entry array must be sorted
 * return a sorted log array
 */
void sortLog(entry_t *entries, int length, const char *outpuFname)
{
    FILE *fp = fopen(outpuFname, "wb");
    assert(fp != NULL);
    for(int i = 0; i < length; i++)
    {
        entry_t *p = entries + i;
        char *log = entryToLog(p);
        fwrite(log, strlen(log), sizeof(char), fp);
    }
    fclose(fp);
}

/* int getNumChallenges() */
/* { */
/*     DEBUG_PRINT("numChallenges: %d\n", numChallenges); */
/*     return numChallenges; */
/* } */
