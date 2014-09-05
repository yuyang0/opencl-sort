/*Time-stamp: <2013-01-15 11:28:12 by Yu Yang>
 * =======================================================================
 *       Filename:  json.h
 *        Created:  2014-09-03 11:11:44
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy1990cn@gmail.com
 * =======================================================================
 */

#ifndef _JSON_H_
#define _JSON_H_ 1

char *skipEscapeChar(char *data);
char *skipSpaces(char *data);
char *skipString(char *data);

char *getValue(char *data, char *key);

#endif /* _JSON_H_ */

