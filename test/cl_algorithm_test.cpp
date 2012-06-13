#include "../src/config.hpp"
#include <OpenCL/opencl.h>
#include <gtest/gtest.h>

extern "C" long get_global_id(int i) {
  return 0;
}

typedef struct {
  int line_number;
  long length;
  long size;
  unsigned char digest[16];
  unsigned char text[1];
} source_line;

extern "C" void hash_line(source_line* sl);

void hex_dump(void* data, size_t length) {
  auto d = (const unsigned char*)data;
  for (auto i = 0; i < length; i++) {
    if (i % 8 == 0) {
      printf("\n");
    }
    
    printf("0x%X ",d[i]);
  }
  printf("\n");
}


class OpenCLProgram {
  string _entry_point;
  string _filepath;
  int err;
  std::map<int, string> _error_map;
  
  long long file_size(FILE* f) {
    fseek(f , 0, SEEK_END);
    auto fs = ftell(f);
    rewind(f);
    return fs;
  }
  
  
public:
  OpenCLProgram(const char* filepath, const char* entry_point) {
    _entry_point = entry_point;
    _filepath = filepath;
    err = CL_SUCCESS;
    
    std::pair<int, string> error_data[] {
      std::make_pair(CL_SUCCESS, "OK"),
      std::make_pair(CL_DEVICE_NOT_FOUND, "Device not Found"),
      std::make_pair(CL_DEVICE_NOT_AVAILABLE, "Device not available"),
      std::make_pair(CL_COMPILER_NOT_AVAILABLE, "Compiler not available"),
      std::make_pair(CL_MEM_OBJECT_ALLOCATION_FAILURE, "Memory object allocation failure"),
      std::make_pair(CL_OUT_OF_RESOURCES, "Out of resource"),
      std::make_pair(CL_OUT_OF_HOST_MEMORY, "Out of host memory"),
      std::make_pair(CL_PROFILING_INFO_NOT_AVAILABLE, "Profiling info not available"),
      std::make_pair(CL_MEM_COPY_OVERLAP, "Memory copy overlap"),
      std::make_pair(CL_IMAGE_FORMAT_MISMATCH, "Image format mismatch"),
      std::make_pair(CL_IMAGE_FORMAT_NOT_SUPPORTED, "Image format not supported"),
      std::make_pair(CL_BUILD_PROGRAM_FAILURE, "Build program failure"),
      std::make_pair(CL_MAP_FAILURE, "Map failure"),
      std::make_pair(CL_MISALIGNED_SUB_BUFFER_OFFSET, "Misaligned sub buffer offset"),
      std::make_pair(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST, "Execu status error for events in wait list"),
      
      std::make_pair(CL_INVALID_VALUE, "Invalid value"),
      std::make_pair(CL_INVALID_DEVICE_TYPE, "Invalid device type"),
      std::make_pair(CL_INVALID_PLATFORM, "Invalid platform"),
      std::make_pair(CL_INVALID_DEVICE, "Invalid device"),
      std::make_pair(CL_INVALID_CONTEXT, "Invalid context"),
      std::make_pair(CL_INVALID_QUEUE_PROPERTIES, "Invalid queue properties"),
      std::make_pair(CL_INVALID_COMMAND_QUEUE, "Invalid command queue"),
      std::make_pair(CL_INVALID_HOST_PTR, "Invalid host pointer"),
      std::make_pair(CL_INVALID_MEM_OBJECT, "Invalid memory object"),
      std::make_pair(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "Invalid image format descriptor"),
      std::make_pair(CL_INVALID_IMAGE_SIZE, "Invalid image size"),
      std::make_pair(CL_INVALID_SAMPLER, "Invalid sampler"),
      std::make_pair(CL_INVALID_BINARY, "Invalid Binary"),
      std::make_pair(CL_INVALID_BUILD_OPTIONS, "Invalid build options"),
      std::make_pair(CL_INVALID_PROGRAM, "Invalid Program"),
      std::make_pair(CL_INVALID_PROGRAM_EXECUTABLE, "Invalid program executable"),
      std::make_pair(CL_INVALID_KERNEL_NAME, "Invalid kernel name"),
      std::make_pair(CL_INVALID_KERNEL_DEFINITION, "Invalid kernel definition"),
      std::make_pair(CL_INVALID_KERNEL, "Invalid Kernel"),
      std::make_pair(CL_INVALID_ARG_INDEX, "Invalid argument index"),
      std::make_pair(CL_INVALID_ARG_VALUE, "Invalid argument value"),
      std::make_pair(CL_INVALID_ARG_SIZE, "Invalid argument size"),
      std::make_pair(CL_INVALID_KERNEL_ARGS, "Invalid kernel arguments"),
      std::make_pair(CL_INVALID_WORK_DIMENSION, "Invalid work dimension"),
      std::make_pair(CL_INVALID_WORK_GROUP_SIZE, "Invalid work group size"),
      std::make_pair(CL_INVALID_WORK_ITEM_SIZE, "Invalid work item size"),
      std::make_pair(CL_INVALID_GLOBAL_OFFSET, "Invalid global offset"),
      std::make_pair(CL_INVALID_EVENT_WAIT_LIST, "Invalid event wait list"),
      std::make_pair(CL_INVALID_EVENT, "Invalid event"),
      std::make_pair(CL_INVALID_OPERATION, "Invalid operation"),
      std::make_pair(CL_INVALID_GL_OBJECT, "Invalid GL object"),
      std::make_pair(CL_INVALID_BUFFER_SIZE, "Invalid buffer size"),
      std::make_pair(CL_INVALID_MIP_LEVEL, "Invalid MIP level"),
      std::make_pair(CL_INVALID_GLOBAL_WORK_SIZE, "Invalid Global work size"),
      std::make_pair(CL_INVALID_PROPERTY, "Invalid property"),
    };
    
    _error_map = std::map<int, string>(error_data, error_data + sizeof(error_data)/sizeof(error_data[0]));
  }
  
  cl_program load_program_into_context(const string& filepath, cl_context& context) {
    FILE* f = fopen(filepath.c_str(), "r");
    
    auto fs = file_size(f);
    
    if (f == NULL) {
      printf("File not found %s\n", filepath.c_str());
      throw std::exception();
    }
    
    char * buffer = (char*) malloc(fs + 1);
    
    auto bytes_read = fread(buffer, 1, fs, f);
    if (bytes_read != fs) {
      printf("Failed to read all the file data\n");
      throw std::exception();
    }
    buffer[fs] = 0;
    
    fclose(f);
    
    cl_int err;
    
    auto program = clCreateProgramWithSource(context, 1, (const char**)&buffer, NULL, &err);
    
    return program;
  }
  
  void error_check() {
    error_check("");
  }
  
  void error_check(const char* cmd) {
    if (err != CL_SUCCESS){
      auto code = _error_map.find(err)->second;
      printf("Command %s failed %d: %s\n", cmd, err, code.c_str());
      throw std::exception();
    }
  }

  static void pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data) {
    printf("Error Notification: %s", errinfo);
  }
  
  void execute(void* input_data, unsigned long input_size, void* output_data, unsigned long output_size, size_t count) {
    
    cl_device_id device_id;
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
      std::cout << "GPU device not available" << std::endl;
      return;
    }
    
    auto context = clCreateContext(0, 1, &device_id, pfn_notify, NULL, &err);
    error_check("clCreateContext");
    auto commands = clCreateCommandQueue(context, device_id, 0, &err);
    error_check("clCreateCommandQueue");
    
    auto program = load_program_into_context(_filepath, context);
    
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    error_check("clBuildProgram");
    
    char log[10024] = {0};
    size_t log_size = 0;
    clGetProgramBuildInfo(program,
                          device_id,
                          CL_PROGRAM_BUILD_LOG,
                          10024,
                          log,
                          &log_size);
    printf("%s", log);
    
    auto kernel = clCreateKernel(program, _entry_point.c_str(), &err);
    error_check();
    
    auto input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  input_size, NULL, NULL);
    auto output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output_size, NULL, NULL);
    
    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, input_size, input_data, 0, NULL, NULL);
    error_check("clEnqueueWriteBuffer");
    
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    error_check();
    
    size_t local = count;
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    error_check("clGetKernelWorkGroupInfo");
    local = count;
    size_t global = count;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    error_check("clEnqueueNDRangeKernel");
    
    err = clFinish(commands);
    error_check("clFinish");
    
    err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, output_size, output_data, 0, NULL, NULL);
    error_check("clEnqueueReadBuffer");
    
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
  }
};




TEST(OpenCLAlgorithm, EmptyString) {
  unsigned char expected_digest[] = {0xE4, 0xC2, 0x37, 0x62, 0xED, 0x28, 0x23, 0xA2, 0x7E, 0x62, 0xA6, 0x4B, 0x95, 0xC0, 0x24, 0xE7};
  
  // Initialise an empty line reference
  source_line * l = new source_line;
  memset(l, 0xFF, sizeof(*l));
  l->line_number = 100;
  l->length = 0;
  
  // Load up the algorithm as an OpenCL kernel
  OpenCLProgram kernel("src/digest_program.c", "hash_driver");
  
  source_line*  output = new source_line;
  memset(output, 0xFF, sizeof(*output));
  
  // Execute for the single entry
  kernel.execute(l, sizeof(*l), output, sizeof(*output), 1);
  
  ASSERT_EQ(100, output->line_number);
  ASSERT_EQ(0, output->length);
  
  // Verify the result.
  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], output->digest[i]);
  }
}

