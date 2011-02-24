/* 
 * File:   io.hpp
 * Author: fqhuy
 *
 * Created on January 28, 2011, 5:43 PM
 */

#ifndef IO_HPP
#define	IO_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <map>
#include "ships/core/core.hpp"

#include "plugin_manager.hpp"
#include "imageio.hpp"
#include "iostream.hpp"
#include "matrixio.hpp"
#include "plugin_octave_text_matrix.hpp"
#include "plugin_bmp.hpp"
#include "plugin_png.hpp"

namespace Sp{
static log4cxx::LoggerPtr io_logger(log4cxx::Logger::getLogger("ships.io"));
}
#endif	/* IO_HPP */

