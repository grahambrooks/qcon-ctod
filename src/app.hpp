
class app {
public:
  int run(int argc, const char* argv[]);
private:
  void print_platform_info();
  void find_duplicates(const std::list<std::string>& paths);
  void print_processing_rate(long bytes, long elapsed_ns) {
    auto meg = bytes / (1024 * 1024);
    auto k = (bytes % (1024 * 12024)) / 1024;
    
    if (meg > 0) {
      std::cout << "Processing rate " << (meg * 1000000) / elapsed_ns << " Mbytes per second" << std::endl;
    } else {
      if (k > 0) {
        std::cout << "Processing rate " << (k * 1000000) / elapsed_ns << " Kbytes per second" << std::endl;
      } else {
        std::cout << "Processing rate " << (bytes * 1000000) / elapsed_ns << " bytes per second" << std::endl;
      }
    }

  }
};
