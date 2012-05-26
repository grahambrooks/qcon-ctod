#include "config.hpp"
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"
#include <dirent.h>
#include "app.hpp"

int main(int argc, const char* argv[]) {
  app a;
  
  a.run(argc, argv);
  
  std::cout << std::endl;

  return 0;
}
