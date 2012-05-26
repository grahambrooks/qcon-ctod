#include "config.hpp"
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"
#include "app.hpp"
#include "task_timer.hpp"

int app::run(int argc, const char* argv[]) {
  std::cout << "QCon Duplicatetext finder" << std::endl;

  try {
    std::list<std::string> paths;
    
    for (auto i = 1; i < argc; i++) {
      if (argv[i][strlen(argv[i])] != '/') {
	paths.insert(paths.end(), string(argv[i]) + "/");
      } else {
	paths.insert(paths.end(), string(argv[i]));
      }
    }
    
    find_duplicates(paths);
    
    print_platform_info();
    
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!\n";
    return 1;
  }
  
  return 0;
}

void app::print_platform_info() {
  task_timer timer("Platform information");

  platform_info info;
  info.print(cout);
  
  device_info dinfo;
  dinfo.print(cout);
}

void app::find_duplicates(const std::list<std::string>& paths) {
  task_timer timer("Finding files");

  filesystem_scanner scanner;
  
  std::list<std::string> found;
  
  for (auto path : paths) {
    scanner.find_all(path, found);
  }
  
  std::cout << "Found " << found.size() << " Files" << std::endl;

  // for (auto p  : found) {
  //   std::cout << p << std::endl;
  // }
}


