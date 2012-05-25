#include "config.hpp"

class filesystem_scanner {
  typedef std::list<std::string> path_list;
  void scan_directory(const string& path, path_list& found);
public:
  void find_all(const string& path, std::list<std::string>& found);
};
