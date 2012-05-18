#include "config.hpp"
#include <OpenCL/opencl.h>
#include "device_info.hpp"

void device_info::print(std::ostream& o) {
  o << "================================================================================" << std::endl;
  o << "OpenCL Device Info" << std::endl;

  cl_platform_id ids[10];
  cl_uint num_platforms;
  
  auto rc = clGetPlatformIDs (10, ids, &num_platforms);
  
  if (rc == CL_INVALID_VALUE) {
    o << "Failed to read platform info error code " << rc << std::endl;
  } else {
    for (auto i = 0; i < num_platforms; i++) {
      cl_device_id device_ids[100];
      cl_uint available_devices = 0;
      cl_int rc = clGetDeviceIDs(ids[i], CL_DEVICE_TYPE_ALL, 100, device_ids, &available_devices);

      if (rc == CL_SUCCESS) {
	for (auto j = 0; j < available_devices; j++) {
	  o << "--------------------------------------------------------------------------------" << std::endl;
	  o << "Device ID " << ids[j] << std::endl;

	  cl_device_type device_type;
	  cl_uint uint_value;
	  size_t actual_size;

	  cl_int rc = clGetDeviceInfo (device_ids[j], CL_DEVICE_TYPE, sizeof(device_type), &device_type, &actual_size);

	  if (rc == CL_SUCCESS) {
	    o << "Device Type " << device_type << std::endl;
	  }


	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_VENDOR_ID,                     "CL_DEVICE_VENDOR_ID                     ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_COMPUTE_UNITS,             "CL_DEVICE_MAX_COMPUTE_UNITS             ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,      "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      ");

	  // Arrays don't work so well
	  //print_value<size_t[100]>(o, device_ids[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,       "CL_DEVICE_MAX_WORK_ITEM_SIZES");

	  print_value<size_t>(o, device_ids[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,            "CL_DEVICE_MAX_WORK_GROUP_SIZE           ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,   "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,  "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,    "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,   "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,  "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_CLOCK_FREQUENCY,           "CL_DEVICE_MAX_CLOCK_FREQUENCY           ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_ADDRESS_BITS,                  "CL_DEVICE_ADDRESS_BITS                  ");

	  print_value<cl_ulong>(o, device_ids[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE,            "CL_DEVICE_MAX_MEM_ALLOC_SIZE            ");
	  print_value<cl_bool>(o, device_ids[j], CL_DEVICE_IMAGE_SUPPORT,                  "CL_DEVICE_IMAGE_SUPPORT                 ");



	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_READ_IMAGE_ARGS,           "CL_DEVICE_MAX_READ_IMAGE_ARGS           ");
	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_WRITE_IMAGE_ARGS,          "CL_DEVICE_MAX_WRITE_IMAGE_ARGS          ");

	  print_value<size_t>(o, device_ids[j], CL_DEVICE_IMAGE2D_MAX_WIDTH,              "CL_DEVICE_IMAGE2D_MAX_WIDTH             ");
	  print_value<size_t>(o, device_ids[j], CL_DEVICE_IMAGE2D_MAX_HEIGHT,             "CL_DEVICE_IMAGE2D_MAX_HEIGHT            ");

	  print_value<size_t>(o, device_ids[j], CL_DEVICE_IMAGE3D_MAX_WIDTH,              "CL_DEVICE_IMAGE3D_MAX_WIDTH             ");
	  print_value<size_t>(o, device_ids[j], CL_DEVICE_IMAGE3D_MAX_HEIGHT,             "CL_DEVICE_IMAGE3D_MAX_HEIGHT            ");
	  print_value<size_t>(o, device_ids[j], CL_DEVICE_IMAGE3D_MAX_DEPTH,              "CL_DEVICE_IMAGE3D_MAX_DEPTH             ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_SAMPLERS,                  "CL_DEVICE_MAX_SAMPLERS                  ");
	  print_value<size_t>(o, device_ids[j], CL_DEVICE_MAX_PARAMETER_SIZE,             "CL_DEVICE_MAX_PARAMETER_SIZE            ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MEM_BASE_ADDR_ALIGN,           "CL_DEVICE_MEM_BASE_ADDR_ALIGN           ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE,      "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,     "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     ");

	  print_value<cl_uint>(o, device_ids[j], CL_DEVICE_MAX_CONSTANT_ARGS,             "CL_DEVICE_MAX_CONSTANT_ARGS             ");

	  print_value<char[1024]>(o, device_ids[j], CL_DEVICE_NAME,                       "CL_DEVICE_NAME                          ");

	  print_value<char[1024]>(o, device_ids[j], CL_DEVICE_VENDOR,                     "CL_DEVICE_VENDOR                        ");
	  print_value<char[1024]>(o, device_ids[j], CL_DEVICE_VERSION,                    "CL_DEVICE_VERSION                       ");
	  print_value<char[1024]>(o, device_ids[j], CL_DEVICE_PROFILE,                    "CL_DEVICE_PROFILE                       ");
	  print_value<char[2048]>(o, device_ids[j], CL_DEVICE_EXTENSIONS,                 "CL_DEVICE_EXTENSIONS                    ");
	  cout << std::endl;
	}
      }
    }
  }
}


template<class T> void device_info::print_value(ostream& o, cl_device_id id, cl_device_info info_item, const char* message) {
    cl_device_type device_type;
    T value;
    size_t actual_size;
    
    auto  rc = clGetDeviceInfo(id, info_item, sizeof(T), &value, &actual_size);
    
    if (rc == CL_SUCCESS) {
      o << message << value << std::endl;
    } else {
      o << message << "NA" << std::endl;
    }
  }
