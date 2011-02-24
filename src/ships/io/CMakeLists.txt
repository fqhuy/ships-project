include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps xerces-c ships_core png)
define_ships_module(io ${deps}) 
