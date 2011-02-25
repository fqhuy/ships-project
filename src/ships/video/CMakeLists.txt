include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/inc")
set(deps ships_core ships_io)
define_ships_module(video ${deps}) 
