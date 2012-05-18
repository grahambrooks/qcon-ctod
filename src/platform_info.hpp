#ifndef PLATFORM_INFO_INCLUDED
#define PLATFORM_INFO_INCLUDED

#include <array>
#include <OpenCL/opencl.h>

class platform_info {
private:
  struct query_spec {
    cl_platform_info info;
    const char* name;
  };
  
  std::array<query_spec, 5> queries = {{
      { CL_PLATFORM_PROFILE, "Platform profile" },
      { CL_PLATFORM_VERSION, "Platform version" },
      { CL_PLATFORM_NAME, "Platform name" },
      { CL_PLATFORM_VENDOR, "Platform Vendor" },
      { CL_PLATFORM_EXTENSIONS, "Platform extensions" }
    }};
  void query_platform(cl_platform_id id, query_spec& query, ostream& o);
public:
  platform_info(){}

  void print(ostream& o);
};

#endif
