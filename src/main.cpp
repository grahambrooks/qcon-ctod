#include "config.hpp"
#include <boost/timer/timer.hpp>
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"

int main(int argc, const char* argv[]) {
  std::cout << "QCon Duplicatetext finder" << std::endl;

  if (true) {
    boost::timer::auto_cpu_timer t;
    run_program();
  }

  filesystem_scanner scanner;

  path_list found;

  if (argc >= 1) {
    boost::timer::auto_cpu_timer t;
    path p(argv[1]);

    scanner.find_all(p, found);

    std::cout << "Found " << found.size() << " Files" << std::endl;
  } else {
    std::cout << "Usage " << argv[0] << " [path]" << std::endl;
  }
}
