/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * =======================================================================
 *       Filename:  clutil.c
 *        Created:  2014-09-03 10:58:50
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

// OpenCL includes
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.h>
#endif

#include "clutil.h"

const char *clGetErrorCode(cl_int error)
{
    switch(error){
        // run-time and JIT compiler errors
    case  0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

const char *clGetErrorString(cl_int err) {
    switch (err) {
    case CL_SUCCESS:                            return "CL_SUCCESS: Success!";
    case CL_DEVICE_NOT_FOUND:                   return "CL_DEVICE_NOT_FOUND: Device not found.";
    case CL_DEVICE_NOT_AVAILABLE:               return "CL_DEVICE_NOT_AVAILABLE: Device not available";
    case CL_COMPILER_NOT_AVAILABLE:             return "CL_COMPILER_NOT_AVAILABLE: Compiler not available";
    case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "CL_MEM_OBJECT_ALLOCATION_FAILURE: Memory object allocation failure";
    case CL_OUT_OF_RESOURCES:                   return "CL_OUT_OF_RESOURCES: Out of resources";
    case CL_OUT_OF_HOST_MEMORY:                 return "CL_OUT_OF_HOST_MEMORY: Out of host memory";
    case CL_PROFILING_INFO_NOT_AVAILABLE:       return "CL_PROFILING_INFO_NOT_AVAILABLE: Profiling information not available";
    case CL_MEM_COPY_OVERLAP:                   return "CL_MEM_COPY_OVERLAP: Memory copy overlap";
    case CL_IMAGE_FORMAT_MISMATCH:              return "CL_IMAGE_FORMAT_MISMATCH: Image format mismatch";
    case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "CL_IMAGE_FORMAT_NOT_SUPPORTED: Image format not supported";
    case CL_BUILD_PROGRAM_FAILURE:              return "CL_BUILD_PROGRAM_FAILURE: Program build failure";
    case CL_MAP_FAILURE:                        return "CL_MAP_FAILURE: Map failure";
    case CL_INVALID_VALUE:                      return "CL_INVALID_VALUE: Invalid value";
    case CL_INVALID_DEVICE_TYPE:                return "CL_INVALID_DEVICE_TYPE: Invalid device type";
    case CL_INVALID_PLATFORM:                   return "CL_INVALID_PLATFORM: Invalid platform";
    case CL_INVALID_DEVICE:                     return "CL_INVALID_DEVICE: Invalid device";
    case CL_INVALID_CONTEXT:                    return "CL_INVALID_CONTEXT: Invalid context";
    case CL_INVALID_QUEUE_PROPERTIES:           return "CL_INVALID_QUEUE_PROPERTIES: Invalid queue properties";
    case CL_INVALID_COMMAND_QUEUE:              return "CL_INVALID_COMMAND_QUEUE: Invalid command queue";
    case CL_INVALID_HOST_PTR:                   return "CL_INVALID_HOST_PTR: Invalid host pointer";
    case CL_INVALID_MEM_OBJECT:                 return "CL_INVALID_MEM_OBJECT: Invalid memory object";
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: Invalid image format descriptor";
    case CL_INVALID_IMAGE_SIZE:                 return "CL_INVALID_IMAGE_SIZE: Invalid image size";
    case CL_INVALID_SAMPLER:                    return "CL_INVALID_SAMPLER: Invalid sampler";
    case CL_INVALID_BINARY:                     return "CL_INVALID_BINARY: Invalid binary";
    case CL_INVALID_BUILD_OPTIONS:              return "CL_INVALID_BUILD_OPTIONS: Invalid build options";
    case CL_INVALID_PROGRAM:                    return "CL_INVALID_PROGRAM: Invalid program";
    case CL_INVALID_PROGRAM_EXECUTABLE:         return "CL_INVALID_PROGRAM_EXECUTABLE: Invalid program executable";
    case CL_INVALID_KERNEL_NAME:                return "CL_INVALID_KERNEL_NAME: Invalid kernel name";
    case CL_INVALID_KERNEL_DEFINITION:          return "CL_INVALID_KERNEL_DEFINITION: Invalid kernel definition";
    case CL_INVALID_KERNEL:                     return "CL_INVALID_KERNEL: Invalid kernel";
    case CL_INVALID_ARG_INDEX:                  return "CL_INVALID_ARG_INDEX: Invalid argument index";
    case CL_INVALID_ARG_VALUE:                  return "CL_INVALID_ARG_VALUE: Invalid argument value";
    case CL_INVALID_ARG_SIZE:                   return "CL_INVALID_ARG_SIZE: Invalid argument size";
    case CL_INVALID_KERNEL_ARGS:                return "CL_INVALID_KERNEL_ARGS: Invalid kernel arguments";
    case CL_INVALID_WORK_DIMENSION:             return "CL_INVALID_WORK_DIMENSION: Invalid work dimension";
    case CL_INVALID_WORK_GROUP_SIZE:            return "CL_INVALID_WORK_GROUP_SIZE: Invalid work group size";
    case CL_INVALID_WORK_ITEM_SIZE:             return "CL_INVALID_WORK_ITEM_SIZE: Invalid work item size";
    case CL_INVALID_GLOBAL_OFFSET:              return "CL_INVALID_GLOBAL_OFFSET: Invalid global offset";
    case CL_INVALID_EVENT_WAIT_LIST:            return "CL_INVALID_EVENT_WAIT_LIST: Invalid event wait list";
    case CL_INVALID_EVENT:                      return "CL_INVALID_EVENT: Invalid event";
    case CL_INVALID_OPERATION:                  return "CL_INVALID_OPERATION: Invalid operation";
    case CL_INVALID_GL_OBJECT:                  return "CL_INVALID_GL_OBJECT: Invalid OpenGL object";
    case CL_INVALID_BUFFER_SIZE:                return "CL_INVALID_BUFFER_SIZE: Invalid buffer size";
    case CL_INVALID_MIP_LEVEL:                  return "CL_INVALID_MIP_LEVEL: Invalid mip-map level";
    default: return "Unknown";
    }
}
/*
 * the folowing function is used to check errors. when the
 * status doesn't equal to CL_SUCCESS, we first print
 * the error code and message specified by the caller, then exit the program.
 */
void clCheckError(int err, char *msg)
{
    if(err != CL_SUCCESS)
    {
        const char *ss = clGetErrorString(err);
        printf("%s\n", ss);
        printf("message: \n", msg);
        exit(-1);
    }
}

cl_context CreateContext()
{
    cl_int err;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    // First, select an OpenCL platform to run on.  For this example, we
    // simply choose the first available platform.  Normally, you would
    // query for all available platforms and select the most appropriate one.
    err = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    clCheckError(err, "can't get the first platform\n");
    // Next, create an OpenCL context on the platform.  Attempt to
    // create a GPU-based context, and if that fails, try to create
    // a CPU-based context.
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
                                      NULL, NULL, &err);
    if (err != CL_SUCCESS)
    {
        printf("Could not create GPU context, trying CPU...\n");
        context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU,
                                          NULL, NULL, &err);
        clCheckError(err, "Failed to create an OpenCL GPU or CPU context.");
    }

    return context;
}

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
    cl_int err;
    cl_device_id *devices;
    cl_command_queue commandQueue = NULL;
    size_t deviceBufferSize = -1;

    // First get the size of the devices buffer
    err = clGetContextInfo(context, CL_CONTEXT_DEVICES,
                           0, NULL, &deviceBufferSize);
    clCheckError(err, "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)");
    if (deviceBufferSize <= 0)
    {
        printf("No devices available.");
        return NULL;
    }

    // Allocate memory for the devices buffer
    devices = malloc(deviceBufferSize);
    err = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
    if (err != CL_SUCCESS)
    {
        free(devices);
        printf("Failed to get device IDs");
        return NULL;
    }

    // In this example, we just choose the first available device.  In a
    // real program, you would likely use all available devices or choose
    // the highest performance device based on OpenCL device queries
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
    if (commandQueue == NULL)
    {
        free(devices);
        printf("Failed to create commandQueue for device 0");
        return NULL;
    }

    *device = devices[0];
    free(devices);
    return commandQueue;
}
