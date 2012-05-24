#include "config.hpp"
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"

namespace po = boost::program_options;

class app {
public:
  int run(int argc, const char* argv[]) {
    try {
      po::options_description desc("Allowed options");

      desc.add_options()
	("help", "produce help message")
	("compression", po::value<int>(), "set compression level")
        ;
      
      po::variables_map vm;        
      po::store(po::parse_command_line(argc, argv, desc), vm);
      po::notify(vm);    
      
      if (vm.count("help")) {
	cout << desc << "\n";
	return 1;
      }
      
      if (vm.count("compression")) {
	std::cout << "Compression level was set to " << vm["compression"].as<int>() << ".\n";
      } else {
	std::cout << "Compression level was not set.\n";
      }
    }
    catch(std::exception& e) {
      std::cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
      std::cerr << "Exception of unknown type!\n";
    }


    return 0;
  }
};



int main(int argc, const char* argv[]) {
  std::cout << "QCon Duplicatetext finder" << std::endl;

  app a;

  a.run(argc, argv);
  
  platform_info info;
  info.print(cout);
  device_info dinfo;
  dinfo.print(cout);
  
  //  print_platform_info(cout);

  if (true) {
    boost::timer::auto_cpu_timer t;
    run_program();
  }

  std::cout << std::endl;

  if (argc > 1) {
    filesystem_scanner scanner;
    
    path_list found;
    
    boost::timer::auto_cpu_timer t;
    path p(argv[1]);

    scanner.find_all(p, found);

    std::cout << "Found " << found.size() << " Files" << std::endl;
  } else {
    std::cout << "Usage " << argv[0] << " [path]" << std::endl;
  }
  return 0;
}
