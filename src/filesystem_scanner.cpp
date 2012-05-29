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
      if((strcmp(fname->d_name, ".") == 0) || (strcmp(fname->d_name, "..") == 0) || (*fname->d_name) == '.')
	continue;
      
      string full_path = (*(dir_name.end()-1) == '/') ? dir_name + fname->d_name : dir_name + "/" + fname->d_name;

      if(fname->d_type == DT_DIR) {
	  scan_directory(full_path, found);
      }
      
      if(fname->d_type == DT_REG)
	found.insert(found.end(), full_path);
    } 
  }

  closedir(od);
}
