#include "config.hpp"
#include "filesystem_scanner.hpp"
#include "opencl_program.hpp"
#include "app.hpp"
#include "task_timer.hpp"
#include "source_file_reader.hpp"

extern "C" void hash_line(source_line* sl, source_line* ol);

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

// Satisfy OpenCL dependency on the hashing driver in digest_program.c
extern "C" long get_global_id(int i) {
  return 0;
}


void app::find_duplicates(const std::list<std::string>& paths) {
  task_timer timer("Finding files");
  
  filesystem_scanner scanner;
  
  std::list<std::string> found;
  
  for (auto path : paths) {
    scanner.find_all(path, found);
  }
  
  SourceTokenizer st;  
  
  long long bytes = 0;
  long lines = 0;
  
  for (auto p  : found) {
    try {
      SourceTokens tokens;
      
      bytes += st.parse_file(p.c_str(), tokens);
      lines += tokens.size(); 
      
      for (auto token : tokens.source_lines()) {
        source_line ol;
        hash_line(token, &ol);
      }
      
    } catch (invalid_file_exception ife) {
      std::cout << p << " - binary" << std::endl;
    }
  }
  std::cout << "Processed " << bytes << " " << lines << " lines" << std::endl;
  std::cout << "Found " << found.size() << " Files" << std::endl;
  
  print_processing_rate(bytes, timer.elapsed());
}


