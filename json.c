/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * =======================================================================
 *       Filename:  json.c
 *        Created:  2014-09-03 10:02:35
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy2012cn@NOSPAM.gmail.com
 * =======================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "util.h"
#include "json.h"

#define DEBUG 0

static char *skipElement(char *data);

void debug(char *ss)
{
    if(DEBUG){
        printf("DEBUG: %s\n", ss);
    }
}

static char *skipEscapeChar(char *data)
{
    assert(*data == '\\');
    return data+2;
}

static char *skipSpaces(char *data)
{
    for(; isspace(*data); data++)
        ;
    return data;
}

static char *skipString(char *data)
{
    assert(*data == '\"');
    data++;      //skip the start quote
    for(; *data != '\"'; data++)
    {
        if(*data == '\\'){
            data = skipEscapeChar(data);
        }
    }
    debug("skipping string finished...");
    return data+1;
}

static char *skipNumber(char *data)
{
    if(*data == '+' || *data == '-'){
        data++;
    }
    for(; isdigit(*data); data++)
        ;
    debug("skipping number finished...");
    return data;
}

static char *skipList(char *data)
{
    assert(*data == '[');
    data++;      //skip the start square brace
    /* skip empty array */
    data = skipSpaces(data);
    if (*data == ']'){
        return data+1;
    }

    for(; ;)
    {
        data = skipSpaces(data);
        data = skipElement(data);
        data = skipSpaces(data);
        if(*data == ','){
            data++;
            continue;
        }else if(*data == ']'){
            return data+1;
        }else{
            printf("Error: need a comma, but gives %c", *data);
            exit(-1);
        }
    }
    return data;
}

static char *skipObject(char *data)
{
    assert(*data == '{');
    data++;    // skip the start brace

    /* empty object */
    data = skipSpaces(data);
    if(*data == '}'){
        return data+1;
    }
    while(1){
        /* skip the key */
        data = skipSpaces(data);
        data = skipElement(data);
        data = skipSpaces(data);
        /* skip the separater... */
        if(*data != ':'){
            printf("Error: need : but gives %c\n", *data);
            exit(-1);
        }else{
            data++;
        }

        /* skip value */
        data = skipSpaces(data);
        data = skipElement(data);
        data = skipSpaces(data);
        if(*data == '}'){
            return data+1;
        }else if(*data == ','){
            data++;
        }else{
            printf("Error: expect , but gives %c\n", *data);
            exit(-1);
        }
    }
    return data;
}

static char *skipElement(char *data)
{
    if(strstartswith(data, "true")){
        return data + strlen("true");
    }else if(strstartswith(data, "false")){
        return data + strlen("false");
    }else if(strstartswith(data, "null")){
        return data + strlen("null");
    }else if(*data == '+' || *data=='-' || isdigit(*data)){
        return skipNumber(data);
    }else if(*data == '\"'){
        debug("skipping string\n");
        return skipString(data);
    }else if(*data == '['){
        debug("skipping list\n");
        return skipList(data);
    }else if(*data == '{'){
        return skipObject(data);
    }else{
        printf("unkown prefix of this element \n");
        exit(-1);
    }
}

/*
 * fetch the value associated with key.
 * NOTICE: the key is a string and should contain start quote and end quote...
 *         if you pass a buffer argument, it means that this function
 *         should use the memory surpport by caller. if you pass NULL
 *         to argument`
 */
char *getValue(char *data, char *key)
{
    assert(*data == '{');
    data++;    // skip the start brace

    /* empty object */
    data = skipSpaces(data);
    if(*data == '}'){
        return NULL;
    }
    char *kPos, *vPos;
    for(; ;){
        /* skip the key */
        data = skipSpaces(data);
        /* store the position of the key */
        kPos = data;
        data = skipElement(data);
        data = skipSpaces(data);
        /* skip the separater... */
        if(*data != ':'){
            printf("Error: need ':' but gives %c\n", *data);
            exit(-1);
        }else{
            data++;
        }

        /* skip value */
        data = skipSpaces(data);
        /* store the position of the value. */
        vPos = data;
        data = skipElement(data);
        if(strstartswith(kPos, key)){
            /* skip the quote... */
            if (*vPos == '\"'){
                vPos++;
            }
            char *end = data-1;
            if (*end == '\"'){
                end--;
            }
            int len = end-vPos+1;
            char *ret = (char *)malloc(len+1);
            strncpy(ret, vPos, len);
            ret[len] = '\0';
            return ret;
        }
        data = skipSpaces(data);
        if(*data == '}'){
            return NULL;
        }else if(*data == ','){
            data++;
        }else{
            printf("Error: expect , but gives %c\n", *data);
            exit(-1);
        }
    }
    return NULL;
}

/*
 * split the string into json records
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

char **splitRecords(char *data)
{
    int numRecords = 0;
    char *start = data, *end;
    for(; *data != '\0'; numRecords++)
    {
        printf("record %d\n", numRecords);

        start = skipSpaces(start);
        if(*start == '\0'){
            break;
        }
        end = nextRecord(start);
        *end = '\0';
        start = end + 1;
    }
    char **arr = malloc(sizeof(char *)*(numRecords+1));
    for(int i = 0; i< numRecords; i++)
    {
        arr[i] = data;
        data = data + strlen(data) + 1;
    }
    arr[numRecords] = NULL;
    return arr;
}
/* int main(int argc, char *argv[]) */
/* { */
/*     char *log = "{\"slice\": \"value of slice\", \"challenge\": \"value of challenge\"}"; */
/*     char *ret = getValue(log, "\"challenge\""); */
/*     printf("challenge: %s\n", ret); */
/*     return 0; */
/* } */
