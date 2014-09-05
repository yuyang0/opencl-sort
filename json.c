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

static char *skipElement(char *data);

char *skipEscapeChar(char *data)
{
    assert(*data == '\\');
    return data+2;
}

char *skipSpaces(char *data)
{
    for(; isspace(*data); data++)
        ;
    return data;
}

char *skipString(char *data)
{
    assert(*data == '\"');
    data++;      //skip the start quote
    for(; *data != '\"'; data++)
    {
        if(*data == '\\'){
            data = skipEscapeChar(data);
        }
    }
    return data+1;
}

static char *skipNumber(char *data)
{
    if(*data == '+' || *data == '-'){
        data++;
    }
    for(; isdigit(*data); data++)
        ;
    return data;
}

static char *skipList(char *data)
{
    assert(*data == '[');
    data++;      //skip the start square bracket
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
            continue;
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
        return skipString(data);
    }else if(*data == '['){
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
 * NOTICE: the key is a string and should contain start quote and end quote.
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
            continue;
        }else{
            printf("Error: expect , but gives %c\n", *data);
            exit(-1);
        }
    }
    return NULL;
}


/* int main(int argc, char *argv[]) */
/* { */
/*     char *log = "{\"slice\": \"value of slice\", \"challenge\": \"value of challenge\"}"; */
/*     char *ret = getValue(log, "\"challenge\""); */
/*     printf("challenge: %s\n", ret); */
/*     return 0; */
/* } */
