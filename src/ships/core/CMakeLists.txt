include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps tinyxml OpenCL log4cxx config++)
define_ships_module(core ${deps}) 
