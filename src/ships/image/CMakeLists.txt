include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps ships_core OpenCL log4cxx)
define_ships_module(image ${deps}) 
