#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#include <list>
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>
#include <OpenCL/opencl.h>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

typedef std::string string;
using std::list;
using std::vector;

typedef list<string> string_list;
typedef std::vector<string> string_vector;

using std::cout;
typedef std::ostream ostream;
typedef std::istream istream;
using std::endl;

#endif
