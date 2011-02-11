include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps ships_core png)
define_ships_module(io ${deps}) 
