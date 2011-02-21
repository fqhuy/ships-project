include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps OpenCL log4cxx config++)
define_ships_module(core ${deps}) 
