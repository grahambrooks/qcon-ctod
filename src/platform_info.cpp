#include "config.hpp"
#include "platform_info.hpp"

void platform_info::print(std::ostream& o) {
  o << "================================================================================" << std::endl;
  o << "OpenCL PLATFORM INFO" << std::endl;

  cl_platform_id ids[10];
  cl_uint num_platforms;

  auto rc = clGetPlatformIDs (10, ids, &num_platforms);

  if (rc == CL_INVALID_VALUE) {
    o << "Failed to read platform info error code " << rc << std::endl;
  } else {
    o << num_platforms << " Platform(s) supported" << std::endl;

    for (auto i = 0; i < num_platforms; i++) {
      for (auto query : queries) {
	query_platform(ids[i], query, o);
      }
    }
  }
}

void platform_info::query_platform(cl_platform_id id, query_spec& query, ostream& o) {
  char param_value[1024];
  size_t param_value_size_ret;
  
  auto rc =  clGetPlatformInfo (id, query.info, sizeof(param_value), param_value, &param_value_size_ret);
  
  if (rc != CL_INVALID_VALUE) {
    o << query.name << " " << param_value << std::endl;
  }
}
