#include "config.hpp"

#include <OpenCL/opencl.h>
#include "program.hpp"

void initialise_input_data(float* data, size_t element_count) {
  for(auto i = 0; i < element_count; i++)
    data[i] = rand() / (float)RAND_MAX;
}


size_t check_results(float *input_data, float* results, size_t element_count) {
  auto correct = 0;

  for(auto i = 0; i < element_count; i++) {
    if(results[i] == input_data[i] * input_data[i])
      correct++;
  }

  return correct;
}


void run_program() {
  const size_t DATA_SIZE = 1024;

  auto count = DATA_SIZE;

  float input_data[DATA_SIZE];
  float results[DATA_SIZE];
  
  
  initialise_input_data(input_data, DATA_SIZE);

  cl_device_id device_id;
  auto err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
  auto context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  auto commands = clCreateCommandQueue(context, device_id, 0, &err);
  auto program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  auto kernel = clCreateKernel(program, "square", &err);
  auto input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * count, NULL, NULL);
  auto output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL);

  err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float) * count, input_data, 0, NULL, NULL);

  err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
  err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);

  auto local = count;
  err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);

  auto global = count;
  err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);

  clFinish(commands);
  err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * count, results, 0, NULL, NULL );
    
  auto correct = check_results(input_data, results, count);
  
  printf("Computed '%lu/%lu' correct values!\n", correct, count);
  
  clReleaseMemObject(input);
  clReleaseMemObject(output);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
}
