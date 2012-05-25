#include "filesystem_scanner.hpp"

#include <dirent.h>

void filesystem_scanner::find_all(const string& path, path_list& found) {
  scan_directory(string(path), found);
}

void filesystem_scanner::scan_directory(const string& dir_name, path_list& found) {
  struct dirent *fname;
  
  DIR *od = opendir(dir_name.c_str());

  if (od != NULL) {
    while((fname = readdir(od)) != NULL) { 
      if((strcmp(fname->d_name, ".") == 0) || (strcmp(fname->d_name, "..") == 0))
	continue;
      
      if(fname->d_type == DT_DIR) {
	string p(dir_name);
	scan_directory(p + "/" + fname->d_name, found);
      }
      
      if(fname->d_type == DT_REG)
	found.insert(found.end(), fname->d_name);
    } 
  }
}
