#include "config.hpp"
#include <boost/timer/timer.hpp>
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"
#include "app.hpp"

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
  boost::timer::auto_cpu_timer t;
  platform_info info;
  info.print(cout);
  
  device_info dinfo;
  dinfo.print(cout);
}

void app::find_duplicates(const std::list<std::string>& paths) {
  boost::timer::auto_cpu_timer t;
  filesystem_scanner scanner;
  
  std::list<std::string> found;
  
  for (auto path : paths) {
    scanner.find_all(path, found);
  }
  
  std::cout << "Found " << found.size() << " Files" << std::endl;
}


