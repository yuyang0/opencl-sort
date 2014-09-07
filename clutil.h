/*Time-stamp: <2013-01-15 11:28:12 by Yu Yang>
 * =======================================================================
 *       Filename:  clutil.h
 *        Created:  2014-09-03 11:00:35
 *       Compiler:  gcc
 *
 *         Author:  Yu Yang
 *			Email:  yy1990cn@gmail.com
 * =======================================================================
 */

#ifndef _CLUTIL_H_
#define _CLUTIL_H_ 1

const char *clGetErrorCode(cl_int error);
const char *clGetErrorString(cl_int err);
void clCheckError(int err, char *msg);

cl_context CreateContext();
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device);

#endif /* _CLUTIL_H_ */

