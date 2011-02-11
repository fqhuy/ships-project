macro(make_test name)
	project(${name})

	file(GLOB test_srcs "src/*.cpp")
	source_group("Src" FILES ${test_srcs})
	file(GLOB test_hdrs "src/*.hpp*")
	source_group("Include" FILES ${test_hdrs})
	
	foreach(d ${ARGN})
		if(${d} MATCHES "ships_")
			#if(${d} MATCHES "ships_lapack")
			#else()
			string(REPLACE "ships_" "${CMAKE_SOURCE_DIR}/src/ships/" d_dir ${d})
			include_directories("${d_dir}/inc")
			#message(STATUS "including ${d_dir}/inc")
			#endif()
		endif()
  	endforeach()
  	
	#include_directories("${CMAKE_SOURCE_DIR}/inc/ships"
	#					"${CMAKE_SOURCE_DIR}/src/ships/core/inc"
						#"${CMAKE_SOURCE_DIR}/src/ships/imgproc/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/features2d/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/flann/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/calib3d/include"
	#					"${CMAKE_SOURCE_DIR}/src/ships/io/inc"
						#"${CMAKE_SOURCE_DIR}/src/ships/objdetect/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/video/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/legacy/include"
						#"${CMAKE_SOURCE_DIR}/src/ships/contrib/include"
	                    #"${CMAKE_CURRENT_SOURCE_DIR}/src"
	 #                   "${CMAKE_CURRENT_BINARY_DIR}")
	
	#include_directories(../cxts)
	
	set(the_target "${name}")
	
	add_executable(${the_target} ${test_srcs} ${test_hdrs})
	
	
	# Additional target properties
	set_target_properties(${the_target} PROPERTIES
		#DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
		)
		
	target_link_libraries(${the_target} ${SHIPS_LINKER_LIBS} ${SHIPS_TESTING_LINKER_LIBS})	
	
	foreach(d ${ARGN})
		if(${d} MATCHES "ships_")	
			add_dependencies(${the_target} ${d})
			target_link_libraries(${the_target} ${d})		
		endif()
  	endforeach()
  	
	#add_dependencies(${the_target} ships_core ships_io)
	
	# Add the required libraries for linking:
	#target_link_libraries(${the_target} ${SHIPS_LINKER_LIBS} ${SHIPS_TESTING_LINKER_LIBS} ships_core ships_io)

	enable_testing()
	get_target_property(LOC ${the_target} LOCATION)
	add_test(test_ships_core "${LOC}")

endmacro()
