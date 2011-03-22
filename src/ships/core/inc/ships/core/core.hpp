#ifndef __SHIPS_CORE_HPP__
#define __SHIPS_CORE_HPP__

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <limits.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <stdint.h>
#include <map>
#include <string>
#include <fstream>
#include <exception>
#include <vector>
#include <CL/cl.hpp>
#include <libconfig.h++>

#include "log4cxx/logger.h"
#include "log4cxx/xml/domconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>

#include "tinyxml.h"

//#include "shipsconfig.h"
#include "exceptions.hpp"
#include "util.hpp"
#include "common.hpp"
#include "allocator.hpp"
#include "color.hpp"
#include "config.hpp"
#include "device_manager.hpp"
#include "geometry_2d.hpp"
#include "geometry_3d.hpp"
#include "pixel.hpp"
#include "memory_model.hpp"
#include "execution_model.hpp"
#include "array.hpp"
#include "sample_model.hpp"
#include "metadata.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "image.hpp"
#include "operations.hpp"
#include "matrix_operations.hpp"
#include "timer.hpp"

namespace Sp {
}
#endif // SHIPS_CORE_HPP
