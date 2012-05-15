#include "filesystem_scanner.hpp"

bool filesystem_scanner::find_file(const path & dir_path, const string & file_name, path & path_found) {
  if (!exists(dir_path))
    return false;
  
  directory_iterator end_itr;
  
  for (boost::filesystem::directory_iterator itr( dir_path ); itr != end_itr; ++itr ) {
    if (boost::filesystem::is_directory(itr->status()) ) {
      if ( find_file( itr->path(), file_name, path_found ) ) return true;
      } else if ( itr->path().filename() == file_name ) {
      path_found = itr->path();
      return true;
    }
  }
  return false;
}


void filesystem_scanner::find_all(const path& dir_path, path_list& found) {
  if (exists(dir_path)) {
    boost::filesystem::directory_iterator end_itr;

    boost::filesystem::directory_iterator itr(dir_path);

    while (itr != end_itr) {

      if (boost::filesystem::is_directory(itr->status()) ) {
	find_all(itr->path(), found);
      } else {
	found.insert(found.end(), itr->path());
      }

      itr++;
    }
  }
}