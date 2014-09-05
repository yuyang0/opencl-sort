/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 * =======================================================================
 *       Filename:  bsort.c
 *        Created:  2014-08-29 23:04:19
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
#include <assert.h>
#include <time.h>
// OpenCL includes
#if defined(__APPLE__) || defined(__MACOSX)
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.h>
#endif

#include "util.h"
#include "clutil.h"
#include "log.h"

#define GROUP_SIZE     256
const unsigned int INPUT_LENGTH = 1<<18;
const cl_uint sortOrder = 1; // descending order else 1 for ascending order

int main(int argc, char *argv[])
{
    entry_t *entries = getChallengeArray("./resource/log-data.txt");
    const size_t datasize = sizeof(entry_t) * numChallenges;

    cl_int err;
    cl_uint numPlatforms = 0;
	cl_platform_id *platforms = NULL;

    const int length = INPUT_LENGTH;

    char *input = (char *)entries;    //input array on host
    char *output = (char *)entries;   //output array on host
    cl_mem inputBuffer = NULL;  // Input array on the device

	// Use clGetPlatformIDs() to retrieve the number of
	// platforms
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	assert(err == CL_SUCCESS);
	assert(numPlatforms > 0);
    printf("found %d platforms.\n", numPlatforms);

	// Allocate enough space for each platform
	platforms =	(cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
	assert(platforms != NULL);

    // Fill in platforms with clGetPlatformIDs()
	err = clGetPlatformIDs(numPlatforms, platforms, NULL);
	assert(err == CL_SUCCESS);


    cl_uint numDevices = 0;
	cl_device_id *devices = NULL;

	// Use clGetDeviceIDs() to retrieve the number of
	// devices present
	err = clGetDeviceIDs(platforms[0],
                         CL_DEVICE_TYPE_GPU,     //only get GPU
                         0,
                         NULL,
                         &numDevices);
    if(numDevices > 0){
        printf("found %d GPUs for platform 0 \n", numDevices);
    }else{
        err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_CPU,
                                0, NULL, &numDevices);
        if(numDevices > 0){
            printf("found %d CPUs for platform 0 \n", numDevices);
        }else{
            printf("can't find CPU or GPU devices\n");
            exit(-1);
        }
    }
	assert(err == CL_SUCCESS);

	// Allocate enough space for each device
	devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));
	assert(devices != NULL);

    err = clGetDeviceIDs(
		platforms[0],
		CL_DEVICE_TYPE_GPU,
		numDevices,
		devices,
		NULL);
	clCheckEqWithMsg(err, CL_SUCCESS, "can't get devices.");

    //-----------------------------------------------------
	// Create a context
	//-----------------------------------------------------
	cl_context ctx = NULL;

	ctx = clCreateContext(NULL, numDevices, devices,
                              NULL, NULL, &err);
	clCheckEqWithMsg(err, CL_SUCCESS, "Can't create context.");

	//-----------------------------------------------------
	// Create a command queue
	//-----------------------------------------------------

	cl_command_queue queue;

	queue = clCreateCommandQueue(ctx, devices[0], 0, &err);
	clCheckEqWithMsg(err, CL_SUCCESS, "Can't create command queue..");

	//-----------------------------------------------------
	// Create device buffers
	//-----------------------------------------------------
	inputBuffer = clCreateBuffer(ctx, CL_MEM_READ_WRITE,
                                 datasize, NULL, &err);
    clCheckEqWithMsg(err, CL_SUCCESS, "can't create device buffer...\n");
	//-----------------------------------------------------
	// Write host data to device buffers
	//-----------------------------------------------------
	err = clEnqueueWriteBuffer(queue, inputBuffer, CL_FALSE, 0,
                               datasize, input, 0, NULL, NULL);
    clCheckEqWithMsg(err, CL_SUCCESS, "can't write host data to device buffer.\n");
	//-----------------------------------------------------
	// STEP 7: Create and compile the program
	//-----------------------------------------------------
	char *pgmSource = (char *)readFile("bsortKernel.cl");

	// Create a program using clCreateProgramWithSource()
	cl_program program = clCreateProgramWithSource(ctx,
                                                   1,
                                                   (const char**)&pgmSource,
                                                   NULL,
                                                   &err);
    clCheckEqWithMsg(err, CL_SUCCESS, "can't create program..");
	// Build (compile) the program for the devices with clBuildProgram()
	err = clBuildProgram(program, numDevices, devices, NULL, NULL, NULL);

    if (err != CL_SUCCESS)
    {
        char buildLog[16384];
        clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);
        printf("Error in kernel: %s\n", buildLog);
        exit(-1);
    }
	//-----------------------------------------------------
	// Create the kernel
	//-----------------------------------------------------
	cl_kernel kernel = NULL;

	kernel = clCreateKernel(program, "parallelBitonicSort", &err);
	clCheckEqWithMsg(err, CL_SUCCESS, "Can't get kernel from program.");
	//-----------------------------------------------------
	// Set the kernel arguments
	//-----------------------------------------------------
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem),(void*)&inputBuffer);
    err |= clSetKernelArg(kernel, 3, sizeof(cl_uint),(void*)&sortOrder);

	clCheckEqWithMsg(err, CL_SUCCESS, "Can't set kernel's arguments.");

	//-----------------------------------------------------
	// Configure the work-item structure
	//-----------------------------------------------------

	//-----------------------------------------------------
	// Enqueue the kernel for execution
	//-----------------------------------------------------
    cl_uint stages = 0;
    for(unsigned int i = INPUT_LENGTH; i > 1; i >>= 1){
        ++stages;
    }

    size_t globalThreads[1] = {INPUT_LENGTH/2};
    size_t threadsPerGroup[1] = {GROUP_SIZE};

	for(cl_uint stage = 0; stage < stages; ++stage) {
        clSetKernelArg(kernel, 1, sizeof(cl_uint),(void*)&stage);

        for(cl_uint subStage = 0; subStage < stage +1; subStage++) {
            clSetKernelArg(kernel, 2, sizeof(cl_uint),(void*)&subStage);
            cl_event exeEvt;
            cl_ulong executionStart, executionEnd;
            err = clEnqueueNDRangeKernel(queue,
                                         kernel,
                                         1,
                                         NULL,
                                         globalThreads,
                                         threadsPerGroup,
                                         0,
                                         NULL,
                                         &exeEvt);
            clWaitForEvents(1, &exeEvt);
            clCheckEqWithMsg(err, CL_SUCCESS, "Kernel execution failure!\n");

            // let's understand how long it took?
            clGetEventProfilingInfo(exeEvt, CL_PROFILING_COMMAND_START, sizeof(executionStart), &executionStart, NULL);
            clGetEventProfilingInfo(exeEvt, CL_PROFILING_COMMAND_END, sizeof(executionEnd), &executionEnd, NULL);
            clReleaseEvent(exeEvt);

            printf("Execution of the bitonic sort took %lu.%lu s\n", (executionEnd - executionStart)/1000000000, (executionEnd - executionStart)%1000000000);
        }
    }

	//-----------------------------------------------------
	// Read the output buffer back to the host
	//-----------------------------------------------------
	clEnqueueReadBuffer(queue,
                        inputBuffer,
                        CL_TRUE,
                        0,
                        datasize,
                        output,
                        0,
                        NULL,
                        NULL);

    /* for(unsigned int i = 0; i < INPUT_LENGTH; i++) */
    /*     printf("%d\t", output[i]); */

    printf("checking result...\n");
    for(int i=0; i< numChallenges; i++)
    {
        entry_t *p = entries + i;
        printf("challenge(%9d): %s, index: %d \n", i, p->challenge, p->idx);
    }
	//-----------------------------------------------------
	// Release OpenCL resources
	//-----------------------------------------------------

	// Free OpenCL resources
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);

	clReleaseMemObject(inputBuffer);

	clReleaseContext(ctx);

	// Free host resources
    releaseLogResource();
	Free(platforms);
	Free(devices);
	Free(pgmSource);
    Free(entries);

    return 0;
}
