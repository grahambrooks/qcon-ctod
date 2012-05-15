#include "config.hpp"

class filesystem_scanner {
public:
  bool find_file(const path & dir_path, const string & file_name, path & path_found);
  
  void find_all(const path& dir_path, path_list& found);
};
