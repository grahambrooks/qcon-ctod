#include "../src/config.hpp"
#include <OpenCL/opencl.h>
#include <gtest/gtest.h>

namespace OpenCL {
  class source {
    std::list<string> files;
  public:
    source(const char* filename) {
      files.insert(files.end(), filename);
    }
    
    int size() {
      return files.size();
    }
  };
}

TEST(MultipleSource, LoadsSingleSourceFile) {
  OpenCL::source single_source_file(__FILE__);
  
  ASSERT_EQ(1, single_source_file.size());
}