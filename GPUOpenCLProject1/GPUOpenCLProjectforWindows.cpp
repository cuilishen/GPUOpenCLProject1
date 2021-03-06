/*****************************************************************************
 * Copyright (c) 2013-2016 Intel Corporation
 * All rights reserved.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/
#define CL_TARGET_OPENCL_VERSION 220
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include <vector>
#include "CL\cl.h"
#include "utils.h"

 //for perf. counters
#include <Windows.h>

#include <iostream>
// Macros for OpenCL versions
#define OPENCL_VERSION_1_2  1.2f
#define OPENCL_VERSION_2_0  2.0f



// Suppress a compiler warning about 'clCreateCommandQueue': was declared deprecated
// for OpenCL 1.2
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

/* This function helps to create informative messages in
 * case when OpenCL errors occur. It returns a string
 * representation for an OpenCL error code.
 * (E.g. "CL_DEVICE_NOT_FOUND" instead of just -1.)
 */
const char* TranslateOpenCLError(cl_int errorCode)
{
	switch (errorCode)
	{
	case CL_SUCCESS:                            return "CL_SUCCESS";
	case CL_DEVICE_NOT_FOUND:                   return "CL_DEVICE_NOT_FOUND";
	case CL_DEVICE_NOT_AVAILABLE:               return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:             return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:                   return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:                 return "CL_OUT_OF_HOST_MEMORY";
	case CL_PROFILING_INFO_NOT_AVAILABLE:       return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:                   return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:              return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:              return "CL_BUILD_PROGRAM_FAILURE";
	case CL_MAP_FAILURE:                        return "CL_MAP_FAILURE";
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:       return "CL_MISALIGNED_SUB_BUFFER_OFFSET";                          //-13
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";   //-14
	case CL_COMPILE_PROGRAM_FAILURE:            return "CL_COMPILE_PROGRAM_FAILURE";                               //-15
	case CL_LINKER_NOT_AVAILABLE:               return "CL_LINKER_NOT_AVAILABLE";                                  //-16
	case CL_LINK_PROGRAM_FAILURE:               return "CL_LINK_PROGRAM_FAILURE";                                  //-17
	case CL_DEVICE_PARTITION_FAILED:            return "CL_DEVICE_PARTITION_FAILED";                               //-18
	case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";                         //-19
	case CL_INVALID_VALUE:                      return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:                return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:                   return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:                     return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:                    return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:           return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:              return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:                   return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:                 return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:                 return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:                    return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:                     return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:              return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:                    return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:         return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:                return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:          return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:                     return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:                  return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:                  return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:                   return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:                return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:             return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:            return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:             return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:              return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:            return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:                      return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:                  return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:                  return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:                return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:                  return "CL_INVALID_MIP_LEVEL";
	case CL_INVALID_GLOBAL_WORK_SIZE:           return "CL_INVALID_GLOBAL_WORK_SIZE";                           //-63
	case CL_INVALID_PROPERTY:                   return "CL_INVALID_PROPERTY";                                   //-64
	case CL_INVALID_IMAGE_DESCRIPTOR:           return "CL_INVALID_IMAGE_DESCRIPTOR";                           //-65
	case CL_INVALID_COMPILER_OPTIONS:           return "CL_INVALID_COMPILER_OPTIONS";                           //-66
	case CL_INVALID_LINKER_OPTIONS:             return "CL_INVALID_LINKER_OPTIONS";                             //-67
	case CL_INVALID_DEVICE_PARTITION_COUNT:     return "CL_INVALID_DEVICE_PARTITION_COUNT";                     //-68
//    case CL_INVALID_PIPE_SIZE:                  return "CL_INVALID_PIPE_SIZE";                                  //-69
//    case CL_INVALID_DEVICE_QUEUE:               return "CL_INVALID_DEVICE_QUEUE";                               //-70    

	default:
		return "UNKNOWN ERROR CODE";
	}
}


/* Convenient container for all OpenCL specific objects used in the sample
 *
 * It consists of two parts:
 *   - regular OpenCL objects which are used in almost each normal OpenCL applications
 *   - several OpenCL objects that are specific for this particular sample
 *
 * You collect all these objects in one structure for utility purposes
 * only, there is no OpenCL specific here: just to avoid global variables
 * and make passing all these arguments in functions easier.
 */
struct ocl_args_d_t
{
	ocl_args_d_t();
	~ocl_args_d_t();

	// Regular OpenCL objects:
	cl_context       context;           // hold the context handler
	cl_device_id     device;            // hold the selected device handler
	cl_command_queue commandQueue;      // hold the commands-queue handler
	cl_program       program;           // hold the program handler
	cl_kernel        kernel;
	cl_kernel        FFT8_kernel;       // hold the kernel handler for FFT_8
	cl_kernel        FFT4_kernel;       // hold the kernel handler for FFT_4
	cl_kernel        FFT2_kernel;       // hold the kernel handler for FFT_2
	float            platformVersion;   // hold the OpenCL platform version (default 1.2)
	float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
	float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)

	// Objects that are specific for algorithm implemented in this sample
	cl_mem           srcMem;              // hold first source buffer
	cl_mem           dstMem;            // hold destination buffer
};

ocl_args_d_t::ocl_args_d_t() :
	context(NULL),
	device(NULL),
	commandQueue(NULL),
	program(NULL),
	kernel(NULL),
	FFT8_kernel(NULL),
	FFT4_kernel(NULL),
	FFT2_kernel(NULL),
	platformVersion(OPENCL_VERSION_1_2),
	deviceVersion(OPENCL_VERSION_1_2),
	compilerVersion(OPENCL_VERSION_1_2),
	srcMem(NULL),
	dstMem(NULL)
{
}

/*
 * destructor - called only once
 * Release all OpenCL objects
 * This is a regular sequence of calls to deallocate all created OpenCL resources in bootstrapOpenCL.
 *
 * You may want to call these deallocation procedures in the middle of your application execution
 * (not at the end) if you don't further need OpenCL runtime.
 * You may want to do that in order to free some memory, for example,
 * or recreate OpenCL objects with different parameters.
 *
 */
ocl_args_d_t::~ocl_args_d_t()
{
	cl_int err = CL_SUCCESS;

	if (kernel)
	{
		err = clReleaseKernel(kernel);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (FFT8_kernel)
	{
		err = clReleaseKernel(FFT8_kernel);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (FFT4_kernel)
	{
		err = clReleaseKernel(FFT4_kernel);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (FFT2_kernel)
	{
		err = clReleaseKernel(FFT2_kernel);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (program)
	{
		err = clReleaseProgram(program);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseProgram returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (srcMem)
	{
		err = clReleaseMemObject(srcMem);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (dstMem)
	{
		err = clReleaseMemObject(dstMem);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (commandQueue)
	{
		err = clReleaseCommandQueue(commandQueue);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseCommandQueue returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (device)
	{
		err = clReleaseDevice(device);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseDevice returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (context)
	{
		err = clReleaseContext(context);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clReleaseContext returned '%s'.\n", TranslateOpenCLError(err));
		}
	}

	/*
	 * Note there is no procedure to deallocate platform
	 * because it was not created at the startup,
	 * but just queried from OpenCL runtime.
	 */
}


/*
 * Check whether an OpenCL platform is the required platform
 * (based on the platform's name)
 */
bool CheckPreferredPlatformMatch(cl_platform_id platform, const char* preferredPlatform)
{
	size_t stringLength = 0;
	cl_int err = CL_SUCCESS;
	bool match = false;

	// In order to read the platform's name, we first read the platform's name string length (param_value is NULL).
	// The value returned in stringLength
	err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetPlatformInfo() to get CL_PLATFORM_NAME length returned '%s'.\n", TranslateOpenCLError(err));
		return false;
	}

	// Now, that we know the platform's name string length, we can allocate enough space before read it
	std::vector<char> platformName(stringLength);

	// Read the platform's name string
	// The read value returned in platformName
	err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, stringLength, &platformName[0], NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetplatform_ids() to get CL_PLATFORM_NAME returned %s.\n", TranslateOpenCLError(err));
		return false;
	}

	// Now check if the platform's name is the required one
	if (strstr(&platformName[0], preferredPlatform) != 0)
	{
		// The checked platform is the one we're looking for
		match = true;
	}

	return match;
}

/*
 * Find and return the preferred OpenCL platform
 * In case that preferredPlatform is NULL, the ID of the first discovered platform will be returned
 */
cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType)
{

	cl_uint numPlatforms = 0;
	cl_int err = CL_SUCCESS;

	// Get (in numPlatforms) the number of OpenCL platforms available
	// No platform ID will be return, since platforms is NULL
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetplatform_ids() to get num platforms returned %s.\n", TranslateOpenCLError(err));
		return NULL;
	}
	LogInfo("Number of available platforms: %u\n", numPlatforms);

	if (0 == numPlatforms)
	{
		LogError("Error: No platforms found!\n");
		return NULL;
	}

	std::vector<cl_platform_id> platforms(numPlatforms);

	cl_uint status;
	cl_platform_id platform;
	std::string platformVendor;
	if (0 < numPlatforms)
	{
		cl_platform_id* platforms = new cl_platform_id[numPlatforms];
		status = clGetPlatformIDs(numPlatforms, platforms, &numPlatforms);

		char platformName[100];
		for (unsigned i = 0; i < numPlatforms; ++i)
		{
			status = clGetPlatformInfo(platforms[i],
				CL_PLATFORM_VENDOR,
				sizeof(platformName),
				platformName,
				NULL);

			platform = platforms[i];
			platformVendor.assign(platformName);
			return platforms[0];
			if (!strcmp(platformName, "Advanced Micro Devices, Inc."))
			{
				break;
			}
		}

		//std::cout << "Platform found : " << platformName << "\n";
		delete[] platforms;
	}

	// Now, obtains a list of numPlatforms OpenCL platforms available
	// The list of platforms available will be returned in platforms
	err = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetplatform_ids() to get platforms returned %s.\n", TranslateOpenCLError(err));
		return NULL;
	}

	// Check if one of the available platform matches the preferred requirements
	for (cl_uint i = 0; i < numPlatforms; i++)
	{
		bool match = true;
		cl_uint numDevices = 0;

		// If the preferredPlatform is not NULL then check if platforms[i] is the required one
		// Otherwise, continue the check with platforms[i]
		if ((NULL != preferredPlatform) && (strlen(preferredPlatform) > 0))
		{
			// In case we're looking for a specific platform
			match = CheckPreferredPlatformMatch(platforms[i], preferredPlatform);
		}

		// match is true if the platform's name is the required one or don't care (NULL)
		if (match)
		{
			// Obtains the number of deviceType devices available on platform
			// When the function failed we expect numDevices to be zero.
			// We ignore the function return value since a non-zero error code
			// could happen if this platform doesn't support the specified device type.
			err = clGetDeviceIDs(platforms[i], deviceType, 0, NULL, &numDevices);
			if (CL_SUCCESS != err)
			{
				LogError("clGetDeviceIDs() returned %s.\n", TranslateOpenCLError(err));
			}

			if (0 != numDevices)
			{
				// There is at list one device that answer the requirements
				return platforms[i];
			}
		}
	}

	return NULL;
}


/*
 * This function read the OpenCL platdorm and device versions
 * (using clGetxxxInfo API) and stores it in the ocl structure.
 * Later it will enable us to support both OpenCL 1.2 and 2.0 platforms and devices
 * in the same program.
 */
int GetPlatformAndDeviceVersion(cl_platform_id platformId, ocl_args_d_t *ocl)
{
	cl_int err = CL_SUCCESS;

	// Read the platform's version string length (param_value is NULL).
	// The value returned in stringLength
	size_t stringLength = 0;
	err = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetPlatformInfo() to get CL_PLATFORM_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the platform's version string length, we can allocate enough space before read it
	std::vector<char> platformVersion(stringLength);

	// Read the platform's version string
	// The read value returned in platformVersion
	err = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, stringLength, &platformVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetplatform_ids() to get CL_PLATFORM_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	if (strstr(&platformVersion[0], "OpenCL 2.0") != NULL)
	{
		ocl->platformVersion = OPENCL_VERSION_2_0;
	}

	// Read the device's version string length (param_value is NULL).
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the device's version string length, we can allocate enough space before read it
	std::vector<char> deviceVersion(stringLength);

	// Read the device's version string
	// The read value returned in deviceVersion
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, stringLength, &deviceVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	if (strstr(&deviceVersion[0], "OpenCL 2.0") != NULL)
	{
		ocl->deviceVersion = OPENCL_VERSION_2_0;
	}

	// Read the device's OpenCL C version string length (param_value is NULL).
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the device's OpenCL C version string length, we can allocate enough space before read it
	std::vector<char> compilerVersion(stringLength);

	// Read the device's OpenCL C version string
	// The read value returned in compilerVersion
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, stringLength, &compilerVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	else if (strstr(&compilerVersion[0], "OpenCL C 2.0") != NULL)
	{
		ocl->compilerVersion = OPENCL_VERSION_2_0;
	}

	return err;
}


/*
 * Generate random value for input buffers
 */
void generateInput(cl_float2* inputArray, cl_uint size)
{
	srand(12345);
	// random initialization of input   
	for (cl_uint i = 0; i < size; ++i)
	{
		inputArray[i].x = ((float)rand() / (float)(RAND_MAX)) * (float)1.0;
		inputArray[i].y = ((float)rand() / (float)(RAND_MAX)) * (float)1.0;
		//inputArray[i].y = 0;
		LogError("x: %f.", inputArray[i].x);
		LogError("y: %f.\n", inputArray[i].y);
	}
}

/*
 * Print output buffers
 */
void PrintOutput(cl_float2* outputArray, cl_uint size)
{
	srand(12345);
	// random initialization of input   
	for (cl_uint i = 0; i < size; ++i)
	{
		//outputArray[i].x = ((float)rand() / (float)(RAND_MAX)) * (float)1.0;
		//outputArray[i].y = ((float)rand() / (float)(RAND_MAX)) * (float)1.0;
		//outputArray[i].y = 0;
		LogError("x: %f.", outputArray[i].x);
		LogError("y: %f.\n", outputArray[i].y);
	}
}


/*
 * This function picks/creates necessary OpenCL objects which are needed.
 * The objects are:
 * OpenCL platform, device, context, and command queue.
 *
 * All these steps are needed to be performed once in a regular OpenCL application.
 * This happens before actual compute kernels calls are performed.
 *
 * For convenience, in this application you store all those basic OpenCL objects in structure ocl_args_d_t,
 * so this function populates fields of this structure, which is passed as parameter ocl.
 * Please, consider reviewing the fields before going further.
 * The structure definition is right in the beginning of this file.
 */
int SetupOpenCL(ocl_args_d_t *ocl, cl_device_type deviceType)
{
	// The following variable stores return codes for all OpenCL calls.
	cl_int err = CL_SUCCESS;

	// Query for all available OpenCL platforms on the system
	// Here you enumerate all platforms and pick one which name has preferredPlatform as a sub-string
	//cl_platform_id platformId = FindOpenCLPlatform("Intel", deviceType);
	cl_platform_id platformId = FindOpenCLPlatform("NVIDIA", deviceType);
	if (NULL == platformId)
	{
		LogError("Error: Failed to find OpenCL platform.\n");
		return CL_INVALID_VALUE;
	}
	//创建GPU设备
	clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU,
		1,
		&ocl->device,
		NULL);
	// Create context with device of specified type.
	// Required device type is passed as function argument deviceType.
	// So you may use this function to create context for any CPU or GPU OpenCL device.
	// The creation is synchronized (pfn_notify is NULL) and NULL user_data
	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformId, 0 };
	ocl->context = clCreateContextFromType(contextProperties, deviceType, NULL, NULL, &err);
	if ((CL_SUCCESS != err) || (NULL == ocl->context))
	{
		LogError("Couldn't create a context, clCreateContextFromType() returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Query for OpenCL device which was used for context creation
	err = clGetContextInfo(ocl->context, CL_CONTEXT_DEVICES, sizeof(cl_device_id), &ocl->device, NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clGetContextInfo() to get list of devices returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	// Read the OpenCL platform's version and the device OpenCL and OpenCL C versions
	GetPlatformAndDeviceVersion(platformId, ocl);

	// Create command queue.
	// OpenCL kernels are enqueued for execution to a particular device through special objects called command queues.
	// Command queue guarantees some ordering between calls and other OpenCL commands.
	// Here you create a simple in-order OpenCL command queue that doesn't allow execution of two kernels in parallel on a target device.
#ifdef CL_VERSION_2_0
	if (OPENCL_VERSION_2_0 == ocl->deviceVersion)
	{
		const cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
		ocl->commandQueue = clCreateCommandQueueWithProperties(ocl->context, ocl->device, properties, &err);
	}
	else {
		// default behavior: OpenCL 1.2
		cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
		ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
	}
#else
	// default behavior: OpenCL 1.2
	cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
	ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
#endif
	if (CL_SUCCESS != err)
	{
		LogError("Error: clCreateCommandQueue() returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}
std::string openclcode_old = "\r\n\r\ntypedef float2 complex;\r\n\r\n            complex twiddle(uint k, float angle)\r\n            {\r\n                float x; float y = sincos(k * angle, &x); return (complex)(x, y);\r\n            }\r\n\r\n            complex complex_mul(float2 a, float2 b)\r\n            {\r\n                return (complex)(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);\r\n            }\r\n\r\n            __kernel void FFT_2(__global complex*input, __global complex*output, uint Ns ) {\r\n\r\n                uint work_item_id = get_global_id(0);\r\n                uint num_work_items = get_global_size(0);\r\n\r\n                complex in0, in1;\r\n                in0 = input[(0 * num_work_items) + work_item_id];\r\n                in1 = input[(1 * num_work_items) + work_item_id];\r\n\r\n                if (Ns != 1)\r\n                {\r\n                    float angle = -2 * M_PI * (work_item_id % Ns) / (Ns * 2);\r\n                    in1 = complex_mul(in1, twiddle(1, angle));\r\n                }\r\n\r\n                complex tmp;\r\n                tmp = in0;\r\n                in0 = in0 + in1;\r\n                in1 = tmp - in1;\r\n\r\n                uint Idout = (work_item_id / Ns) * Ns * 2 + (work_item_id % Ns);\r\n                output[(0 * Ns) + Idout] = in0;\r\n                output[(1 * Ns) + Idout] = in1;\r\n            }\r\n            __kernel void FFT_4(__global complex*input, __global complex*output, uint Ns )\n{\r\n                uint work_item_id = get_global_id(0);\r\n                uint num_work_items = get_global_size(0);\r\n\r\n                complex in0, in1, in2, in3;\r\n                in0 = input[(0 * num_work_items) + work_item_id];\r\n                in1 = input[(1 * num_work_items) + work_item_id];\r\n                in2 = input[(2 * num_work_items) + work_item_id];\r\n                in3 = input[(3 * num_work_items) + work_item_id];\r\n\r\n                if (Ns != 1)\r\n                {\r\n                    float angle = -2 * M_PI * (work_item_id % Ns) / (Ns * 2);\r\n                    in1 = complex_mul(in1, twiddle(1, angle));\r\n                    in2 = complex_mul(in2, twiddle(2, angle));\r\n                    in3 = complex_mul(in3, twiddle(3, angle));\r\n\r\n                }\r\n\r\n                complex v0, v1, v2, v3;\r\n                v0 = in0 + in2;\r\n                v2 = in0 - in2;\r\n                v1 = in1 + in3;\r\n                v3 = (complex)(in1.y - in3.y, in3.x - in1.x);\r\n                in0 = v0 + v1;\r\n                in2 = v0 - v1;\r\n                in1 = v2 + v3;\r\n                in3 = v2 - v3;\r\n\r\n                uint Idout = (work_item_id / Ns) * Ns * 4 + (work_item_id % Ns);\r\n                output[(0 * Ns) + Idout] = in0;\r\n                output[(1 * Ns) + Idout] = in1;\r\n                output[(2 * Ns) + Idout] = in2;\r\n                output[(3 * Ns) + Idout] = in3;\r\n            }\r\n\r\n            __kernel void FFT_8(__global complex*input, __global complex*output, uint Ns)\n{\r\n                uint work_item_id = get_global_id(0);\r\n                uint num_work_items = get_global_size(0);\r\n\r\n                complex in0, in1, in2, in3, in4, in5, in6, in7;\r\n                in0 = input[(0 * num_work_items) + work_item_id];\r\n                in1 = input[(1 * num_work_items) + work_item_id];\r\n                in2 = input[(2 * num_work_items) + work_item_id];\r\n                in3 = input[(3 * num_work_items) + work_item_id];\r\n                in4 = input[(4 * num_work_items) + work_item_id];\r\n                in5 = input[(5 * num_work_items) + work_item_id];\r\n                in6 = input[(6 * num_work_items) + work_item_id];\r\n                in7 = input[(7 * num_work_items) + work_item_id];\r\n\r\n                if (Ns != 1)\r\n                {\r\n                    float angle = -2 * M_PI * (work_item_id % Ns) / (Ns * 2);\r\n                    in1 = complex_mul(in1, twiddle(1, angle));\r\n                    in2 = complex_mul(in2, twiddle(2, angle));\r\n                    in3 = complex_mul(in3, twiddle(3, angle));\r\n                    in4 = complex_mul(in4, twiddle(4, angle));\r\n                    in5 = complex_mul(in5, twiddle(5, angle));\r\n                    in6 = complex_mul(in6, twiddle(6, angle));\r\n                    in7 = complex_mul(in7, twiddle(7, angle));\r\n                }\r\n\r\n                complex v0, v1, v2, v3, v4, v5, v6, v7;\r\n                v0 = in0 + in4;\r\n                v4 = in0 - in4;\r\n                v1 = in1 + in5;\r\n                v5 = in1 - in5;\r\n                v2 = in2 + in6;\r\n                v6 = (complex)(in2.y - in6.y, in6.x - in2.x);\r\n                v3 = in3 + in7;\r\n                v7 = (complex)(in3.y - in7.y, in7.x - in3.x);\r\n\r\n                complex tmp;\r\n                tmp = v0;\r\n                v0 = v0 + v2;\r\n                v2 = tmp - v2;\r\n                tmp = v1;\r\n                v1 = v1 + v3;\r\n                v3 = (complex)(tmp.y - v3.y, v3.x - tmp.x);\r\n\r\n                tmp = v4;\r\n                v4 = v4 + v6;\r\n                v6 = tmp - v6;\r\n                tmp = v5;\r\n                v5 = (complex)(M_SQRT1_2 * (v5.x + v7.x + v5.y + v7.y), M_SQRT1_2 * (v5.y + v7.y - v5.x - v7.x));\r\n                v7 = (complex)(M_SQRT1_2 * (v7.x - tmp.x - v7.y + tmp.y), M_SQRT1_2 * (v7.y - tmp.y + v7.x - tmp.x));\r\n\r\n                in0 = v0 + v1;\r\n                in4 = v0 - v1;\r\n\r\n                in1 = v4 + v5;\r\n                in5 = v4 - v5;\r\n\r\n                in2 = v2 + v3;\r\n                in6 = v2 - v3;\r\n\r\n                in3 = v6 + v7;\r\n                in7 = v6 - v7;\r\n\r\n                uint Idout = (work_item_id / Ns) * Ns * 8 + (work_item_id % Ns);\r\n                output[(0 * Ns) + Idout] = in0;\r\n                output[(1 * Ns) + Idout] = in1;\r\n                output[(2 * Ns) + Idout] = in2;\r\n                output[(3 * Ns) + Idout] = in3;\r\n                output[(4 * Ns) + Idout] = in4;\r\n                output[(5 * Ns) + Idout] = in5;\r\n                output[(6 * Ns) + Idout] = in6;\r\n                output[(7 * Ns) + Idout] = in7;\r\n            }\r\n\r\n\r\n            ";
std::string openclcode =
"typedef float2 complex;"

"complex	twiddle(uint k, float angle) {"
"float x; float y = sincos(k*angle, &x); 	return (complex)(x, y);"
"}"

"complex complex_mul(float2 a, float2 b) {"
"return (complex)(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);"
"}"

"__kernel void FFT_2(__global complex* input, __global complex* output, uint Ns) {"

"uint work_item_id = get_global_id(0);								"
"uint num_work_items = get_global_size(0);						   "
"																   "
"uint Idout = (work_item_id / Ns)*Ns * 2 + (work_item_id%Ns);	   "
"complex in0, in1;												   "
"in0 = input[(0 * num_work_items) + work_item_id];				   "
"in1 = input[(1 * num_work_items) + work_item_id];				   "
"																   "
"if (Ns != 1)													   "
"{																   "
"	float angle = -2 * M_PI*(work_item_id % Ns) / (Ns * 2);		   "
"	in1 = complex_mul(in1, twiddle(1, angle));					   "
"}																   "
"																   "
"complex  tmp;													   "
"tmp = in0;														   "
"in0 = in0 + in1;												   "
"in1 = tmp - in1;												   "
"																   "
"output[(0 * Ns) + Idout] = in0;								   "
"output[(1 * Ns) + Idout] = in1;								   "
"}"
"__kernel void FFT_4(__global complex* input, __global complex* output, uint Ns)									"
"{																												   "
"	uint work_item_id = get_global_id(0);																		   "
"	uint num_work_items = get_global_size(0);																	   "
"																												   "
"	complex  in0, in1, in2, in3;																				   "
"	in0 = input[(0 * num_work_items) + work_item_id];															   "
"	in1 = input[(1 * num_work_items) + work_item_id];															   "
"	in2 = input[(2 * num_work_items) + work_item_id];															   "
"	in3 = input[(3 * num_work_items) + work_item_id];															   "
"																												   "
"	if (Ns != 1)																								   "
"	{																											   "
"		float angle = -2 * M_PI*(work_item_id % Ns) / (Ns * 2);													   "
"		in1 = complex_mul(in1, twiddle(1, angle));																   "
"		in2 = complex_mul(in2, twiddle(2, angle));																   "
"		in3 = complex_mul(in3, twiddle(3, angle));																   "
"																												   "
"	}																											   "
"																												   "
"	complex v0, v1, v2, v3;																						   "
"	v0 = in0 + in2;																								   "
"	v2 = in0 - in2;																								   "
"	v1 = in1 + in3;																								   "
"	v3 = (complex)(in1.y - in3.y, in3.x - in1.x);																   "
"	in0 = v0 + v1;																								   "
"	in2 = v0 - v1;																								   "
"	in1 = v2 + v3;																								   "
"	in3 = v2 - v3;																								   "
"																												   "
"	uint Idout = (work_item_id / Ns)*Ns * 4 + (work_item_id%Ns);												   "
"	output[(0 * Ns) + Idout] = in0;																				   "
"	output[(1 * Ns) + Idout] = in1;																				   "
"	output[(2 * Ns) + Idout] = in2;																				   "
"	output[(3 * Ns) + Idout] = in3;																				   "
"}																												   "
"																												   "
"__kernel void FFT_8(__global complex* input, __global complex* output, uint Ns)								   "
"{																												   "
"	uint work_item_id = get_global_id(0);																		   "
"	uint num_work_items = get_global_size(0);																	   "
"		uint Idout = (work_item_id / Ns)*Ns * 8 + (work_item_id%Ns);																										   "

"	complex  in0, in1, in2, in3, in4, in5, in6, in7;															   "
"	in0 = input[(0 * num_work_items) + work_item_id];															   "
"	in1 = input[(1 * num_work_items) + work_item_id];															   "
"	in2 = input[(2 * num_work_items) + work_item_id];															   "
"	in3 = input[(3 * num_work_items) + work_item_id];															   "
"	in4 = input[(4 * num_work_items) + work_item_id];															   "
"	in5 = input[(5 * num_work_items) + work_item_id];															   "
"	in6 = input[(6 * num_work_items) + work_item_id];															   "
"	in7 = input[(7 * num_work_items) + work_item_id];															   "
"																												   "
"	if (Ns != 1)																								   "
"	{																											   "
"		float angle = -2 * M_PI*(work_item_id % Ns) / (Ns * 2);													   "
"		in1 = complex_mul(in1, twiddle(1, angle));																   "
"		in2 = complex_mul(in2, twiddle(2, angle));																   "
"		in3 = complex_mul(in3, twiddle(3, angle));																   "
"		in4 = complex_mul(in4, twiddle(4, angle));																   "
"		in5 = complex_mul(in5, twiddle(5, angle));																   "
"		in6 = complex_mul(in6, twiddle(6, angle));																   "
"		in7 = complex_mul(in7, twiddle(7, angle));																   "
"	}																											   "
"																												   "
"	complex  v0, v1, v2, v3, v4, v5, v6, v7;																	   "
"	v0 = in0 + in4;																								   "
"	v4 = in0 - in4;																								   "
"	v1 = in1 + in5;																								   "
"	v5 = in1 - in5;																								   "
"	v2 = in2 + in6;																								   "
"	v6 = (complex)(in2.y - in6.y, in6.x - in2.x);																   "
"	v3 = in3 + in7;																								   "
"	v7 = (complex)(in3.y - in7.y, in7.x - in3.x);																   "
"																												   "
"	complex tmp;																								   "
"	tmp = v0;																									   "
"	v0 = v0 + v2;																								   "
"	v2 = tmp - v2;																								   "
"	tmp = v1;																									   "
"	v1 = v1 + v3;																								   "
"	v3 = (complex)(tmp.y - v3.y, v3.x - tmp.x);																	   "
"																												   "
"	tmp = v4;																									   "
"	v4 = v4 + v6;																								   "
"	v6 = tmp - v6;																								   "
"	tmp = v5;																									   "
"	v5 = (complex)(M_SQRT1_2* (v5.x + v7.x + v5.y + v7.y), M_SQRT1_2*(v5.y + v7.y - v5.x - v7.x));				   "
"	v7 = (complex)(M_SQRT1_2* (v7.x - tmp.x - v7.y + tmp.y), M_SQRT1_2*(v7.y - tmp.y + v7.x - tmp.x));			   "
"																												   "
"	in0 = v0 + v1;																								   "
"	in4 = v0 - v1;																								   "
"																												   "
"	in1 = v4 + v5;																								   "
"	in5 = v4 - v5;																								   "
"																												   "
"	in2 = v2 + v3;																								   "
"	in6 = v2 - v3;																								   "
"																												   "
"	in3 = v6 + v7;																								   "
"	in7 = v6 - v7;																								   "
"																												   "
"													   "
"	output[(0 * Ns) + Idout] = in0;																				   "
"	output[(1 * Ns) + Idout] = in1;																				   "
"	output[(2 * Ns) + Idout] = in2;																				   "
"	output[(3 * Ns) + Idout] = in3;																				   "
"	output[(4 * Ns) + Idout] = in4;																				   "
"	output[(5 * Ns) + Idout] = in5;																				   "
"	output[(6 * Ns) + Idout] = in6;																				   "
"	output[(7 * Ns) + Idout] = in7;																				   "
"}";


/*
 * Create and build OpenCL program from its source code
 */
int CreateAndBuildProgram(ocl_args_d_t *ocl)
{
	cl_int err = CL_SUCCESS;

	// Upload the OpenCL C source code from the input file to source
	// The size of the C program is returned in sourceSize
	char* source = NULL;
	size_t src_size = 0;
	err = ReadSourceFromFile("FFT.cl", &source, &src_size);
	if (CL_SUCCESS != err)
	{
		LogError("Error: ReadSourceFromFile returned %s.\n", TranslateOpenCLError(err));
		goto Finish;
	}

	// And now after you obtained a regular C string call clCreateProgramWithSource to create OpenCL program object.
	//ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&source, &src_size, &err);
	//src_size = strlen(openclcode);

	const char * Newsource = openclcode.c_str();
	size_t sourceSize[] = { strlen(Newsource) };

	ocl->program = clCreateProgramWithSource(ocl->context, 1, &Newsource, sourceSize, &err);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clCreateProgramWithSource returned %s.\n", TranslateOpenCLError(err));
		goto Finish;
	}

	// Build the program
	// During creation a program is not built. You need to explicitly call build function.
	// Here you just use create-build sequence,
	// but there are also other possibilities when program consist of several parts,
	// some of which are libraries, and you may want to consider using clCompileProgram and clLinkProgram as
	// alternatives.
	//err = clBuildProgram(ocl->program, 1, &ocl->device, " -g -s \"c:\\users\\betau\\onedrive\\documents\\visual studio 2015\\Projects\\FFT_OCL\\FFT_OCL\\FFT.cl\"", NULL, NULL);
	err = clBuildProgram(ocl->program, 1, &ocl->device, NULL, NULL, NULL);

	
	//err = clBuildProgram(ocl->program, 1, &ocl->device, openclcode, NULL, NULL);

	if (CL_SUCCESS != err)
	{
		LogError("Error: clBuildProgram() for source program returned %s.\n", TranslateOpenCLError(err));

		// In case of error print the build log to the standard output
		// First check the size of the log
		// Then allocate the memory and obtain the log from the program
		if (err == CL_BUILD_PROGRAM_FAILURE)
		{
			size_t log_size = 0;
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			std::vector<char> build_log(log_size);
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, log_size, &build_log[0], NULL);

			LogError("Error happened during the build of OpenCL program.\nBuild log:%s", &build_log[0]);
		}
	}

	Finish:
	if (source)
	{
		delete[] source;
		source = NULL;
	}

	return err;
}


/*
 * Create OpenCL buffers from host memory
 * These buffers will be used later by the OpenCL kernel
 */
int CreateBufferArguments(ocl_args_d_t *ocl, cl_float2* input, cl_float2* output, cl_uint size)
{
	cl_int err = CL_SUCCESS;

	// Create new OpenCL buffer objects
	// As these buffer are used only for read by the kernel, you are recommended to create it with flag CL_MEM_READ_ONLY.
	// Always set minimal read/write flags for buffers, it may lead to better performance because it allows runtime
	// to better organize data copying.
	// You use CL_MEM_COPY_HOST_PTR here, because the buffers should be populated with bytes at inputA and inputB.

	ocl->srcMem = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(*input) * size, input, &err);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clCreateBuffer for srcA returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// If the output buffer is created directly on top of output buffer using CL_MEM_USE_HOST_PTR,
	// then, depending on the OpenCL runtime implementation and hardware capabilities, 
	// it may save you not necessary data copying.
	// As it is known that output buffer will be write only, you explicitly declare it using CL_MEM_WRITE_ONLY.
	ocl->dstMem = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(*output) * size, output, &err);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clCreateBuffer for dstMem returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}


/*
 * Set kernel arguments
 */
cl_uint SetKernelArguments(cl_kernel kernel, cl_mem srcMem, cl_mem dstMem, cl_uint Ns)
{
	cl_int err = CL_SUCCESS;

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&srcMem);
	if (CL_SUCCESS != err)
	{
		LogError("error: Failed to set argument srcA, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dstMem);
	if (CL_SUCCESS != err)
	{
		LogError("Error: Failed to set argument dstMem, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	err = clSetKernelArg(kernel, 2, sizeof(cl_uint), (void *)&Ns);
	if (CL_SUCCESS != err)
	{
		LogError("Error: Failed to set argument dstMem, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	return err;
}


/*
 * Execute the kernel
 */
cl_uint EnqueueFFTKernel(cl_command_queue commandQueue, cl_kernel kernel, cl_uint size)
{
	cl_int err = CL_SUCCESS;

	// Define global iteration space for clEnqueueNDRangeKernel.
	size_t globalWorkSize[1] = { size };


	// execute kernel
	err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(err));
		return err;
	}

	return err;
}


/*
 * "Read" the result buffer (mapping the buffer to the host memory address)
 */
cl_int ReadAndVerify(cl_command_queue commandQueue, cl_mem result, cl_uint size, cl_float2 *output)
{

	cl_int err = CL_SUCCESS;

	// Enqueue a command to map the buffer object (ocl->dstMem) into the host address space and returns a pointer to it
	// The map operation is blocking
	output = (cl_float2 *)clEnqueueMapBuffer(commandQueue, result, true, CL_MAP_READ, 0, sizeof(cl_float2) * size, 0, NULL, NULL, &err);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clEnqueueMapBuffer returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Call clFinish to guarantee that output region is updated
	err = clFinish(commandQueue);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clFinish returned %s\n", TranslateOpenCLError(err));
	}

	// Unmapped the output buffer before releasing it
	err = clEnqueueUnmapMemObject(commandQueue, result, output, 0, NULL, NULL);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clEnqueueUnmapMemObject returned %s\n", TranslateOpenCLError(err));
	}
}

/*
 * main execution routine
 * Basically it consists of three parts:
 *   - generating the inputs
 *   - running OpenCL kernel
 *   - reading results of processing
 */
int _tmain(int argc, TCHAR* argv[])
{
	cl_int err;
	ocl_args_d_t ocl;
	cl_device_type deviceType = CL_DEVICE_TYPE_GPU;

	LARGE_INTEGER perfFrequency;
	LARGE_INTEGER performanceCountNDRangeStart;
	LARGE_INTEGER performanceCountNDRangeStop;

	cl_uint size = 1 << 4;

	//initialize Open CL objects (context, queue, etc.)
	if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType))
	{
		return -1;
	}

	// allocate working buffers. 
	// the buffer should be aligned with 4K page and size should fit 64-byte cached line

	int leng = (sizeof(cl_float2));

	cl_uint optimizedSize = ((sizeof(cl_float2) * size - 1) / 64 + 1) * 64;
	cl_float2* input = (cl_float2*)_aligned_malloc(optimizedSize, 4096);
	cl_float2* output = (cl_float2*)_aligned_malloc(optimizedSize, 4096);
	if (NULL == input || NULL == output)
	{
		LogError("Error: _aligned_malloc failed to allocate buffers.\n");
		return -1;
	}

	LogInfo("以下是input\n");
	//random input
	generateInput(input, size);
	/*LogInfo("以下是随机output\n");
	generateInput(output, size);*/

	// Create OpenCL buffers from host memory
	// These buffers will be used later by the OpenCL kernel
	if (CL_SUCCESS != CreateBufferArguments(&ocl, input, output, size))
	{
		return -1;
	}

	// Create and build the OpenCL program
	if (CL_SUCCESS != CreateAndBuildProgram(&ocl))
	{
		return -1;
	}

	cl_uint stages = (cl_uint)log2(size);							//Total number of stages
	cl_uint FFT_8_stages = stages / 3;						//Number of stages to compute using our largest FFT func (FFT_8)
	cl_uint remainder_stages = stages % 3;					//Remainding stages
	cl_uint Ns = 1;											//Defined as pow (2 , number of completed stages). Used to calculate twiddle factors and index remapping
	cl_mem tmp;

	bool queueProfilingEnable = true;
	if (queueProfilingEnable)
		QueryPerformanceCounter(&performanceCountNDRangeStart);

	if (FFT_8_stages)
	{
		// Program consists of kernels.
		// Each kernel can be called (enqueued) from the host part of OpenCL application.
		// To call the kernel, you need to create it from existing program.
		ocl.FFT8_kernel = clCreateKernel(ocl.program, "FFT_8", &err);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
			return -1;
		}

		for (size_t i = 0; i < FFT_8_stages; i++)
		{
			// Passing arguments into OpenCL kernel.
			if (CL_SUCCESS != SetKernelArguments(ocl.FFT8_kernel, ocl.srcMem, ocl.dstMem, Ns))
			{
				return -1;
			}

			if (CL_SUCCESS != EnqueueFFTKernel(ocl.commandQueue, ocl.FFT8_kernel, size / 8))
			{
				return -1;
			}

			//Remapping input and output buffer for next stage
			tmp = ocl.srcMem;
			ocl.srcMem = ocl.dstMem;
			ocl.dstMem = tmp;
			Ns = Ns << 3;										//Each FFT_8 enqueued kernel completes 3 stages
		}
	}
	
	//clEnqueueBarrier(ocl.commandQueue);
	if (remainder_stages == 1) {
		ocl.FFT2_kernel = clCreateKernel(ocl.program, "FFT_2", &err);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
			return -1;
		}

		// Passing arguments into OpenCL kernel.
		if (CL_SUCCESS != SetKernelArguments(ocl.FFT2_kernel, ocl.srcMem, ocl.dstMem, Ns))
		{
			return -1;
		}

		if (CL_SUCCESS != EnqueueFFTKernel(ocl.commandQueue, ocl.FFT2_kernel, size / 2))
		{
			return -1;
		}

		//Remapping input and output buffer for next stage
		tmp = ocl.srcMem;
		ocl.srcMem = ocl.dstMem;
		ocl.dstMem = tmp;
		Ns = Ns << 1;
	}

	else if (remainder_stages == 2) {
		ocl.FFT4_kernel = clCreateKernel(ocl.program, "FFT_4", &err);
		if (CL_SUCCESS != err)
		{
			LogError("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
			return -1;
		}

		// Passing arguments into OpenCL kernel.
		if (CL_SUCCESS != SetKernelArguments(ocl.FFT4_kernel, ocl.srcMem, ocl.dstMem, Ns))
		{
			return -1;
		}

		if (CL_SUCCESS != EnqueueFFTKernel(ocl.commandQueue, ocl.FFT4_kernel, size / 4))
		{
			return -1;
		}

		//Remapping input and output buffer for next stage
		tmp = ocl.srcMem;
		ocl.srcMem = ocl.dstMem;
		ocl.dstMem = tmp;
		Ns = Ns << 2;
	}
	

	
	err = clFinish(ocl.commandQueue);
	if (CL_SUCCESS != err)
	{
		LogError("Error: clFinish return %s\n", TranslateOpenCLError(err));
		return err;
	}

	if (queueProfilingEnable)
		QueryPerformanceCounter(&performanceCountNDRangeStop);


	///!!!非常重要，必须读取dstMem,否则NVIDIA显卡读取不到数据，找了一天才找到原因
	//ReadAndVerify(ocl.commandQueue, ocl.srcMem, size, output);
	ReadAndVerify(ocl.commandQueue, ocl.dstMem, size, output);

	int outputLength = sizeof(output);


	for (cl_float2* p = &output[0]; p <= &output[sizeof(output) / sizeof(output[0]) - 1]; p++)
	{
		//cout << *p << " ";
		LogInfo("Error: clFinish return %s\n", p->x);
	}

	LogInfo("以下是output\n");
	PrintOutput(output,size);
	LogInfo("以下是input\n");
	PrintOutput(input, size);

	// retrieve performance counter frequency
	if (queueProfilingEnable)
	{
		QueryPerformanceFrequency(&perfFrequency);
		LogInfo("NDRange performance counter time %f ms.\n",
			1000.0f*(float)(performanceCountNDRangeStop.QuadPart - performanceCountNDRangeStart.QuadPart) / (float)perfFrequency.QuadPart);
	}

	//LogInfo("以下是output\n");
	//PrintOutput(output, size);
	//LogInfo("以下是input\n");
	//PrintOutput(input, size);

	_aligned_free(input);
	_aligned_free(output);
	//extra
	char str[] = "Hello C++";

	std::cout << "Value of str is : " << str;
	return 0;
}