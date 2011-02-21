# thanks to OpenCV project for this file ^_^
# this is template for a SHIPS module 
macro(define_ships_module name)
	
	project(ships_${name})
	add_definitions(-DSPAPI_EXPORTS)

	include_directories("${CPLUS_INCLUDE_PATH}"
						"${CMAKE_CURRENT_SOURCE_DIR}/inc"
	                    "${CMAKE_CURRENT_SOURCE_DIR}/src"
	                    "${CMAKE_CURRENT_BINARY_DIR}")
	
	#dependencies injection (from other modules)
	foreach(d ${ARGN})
		if(${d} MATCHES "ships_")
			#if(${d} MATCHES "ships_lapack")
			#else()
			string(REPLACE "ships_" "${CMAKE_CURRENT_SOURCE_DIR}/../" d_dir ${d})
			include_directories("${d_dir}/inc")
			message(STATUS "including ${d_dir}/inc")
			#endif()
		endif()
  	endforeach()

	#source files
	file(GLOB lib_srcs "src/*.cpp")
	file(GLOB lib_int_hdrs "src/*.hpp*")
	source_group("Src" FILES ${lib_srcs} ${lib_int_hdrs})
	#header files
	file(GLOB lib_hdrs "inc/ships/${name}/*.hpp*")
	source_group("Include" FILES ${lib_hdrs})
	#target
	set(the_target "ships_${name}")
	#build library
	add_library(${the_target} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

	#if(PCHSupport_FOUND)
	#    set(pch_header ${CMAKE_CURRENT_SOURCE_DIR}/src/precomp.hpp)
	#    if(${CMAKE_GENERATOR} MATCHES "Visual*" OR ${CMAKE_GENERATOR} MATCHES "Xcode*")
	#        if(${CMAKE_GENERATOR} MATCHES "Visual*")
	#            set(${the_target}_pch "src/precomp.cpp")
	#        endif()            
	#        add_native_precompiled_header(${the_target} ${pch_header})
	#    elseif(CMAKE_COMPILER_IS_GNUCXX AND ${CMAKE_GENERATOR} MATCHES ".*Makefiles")
	#        add_precompiled_header(${the_target} ${pch_header})
	#    endif()
	#endif()

	# For dynamic link numbering convenions
	set_target_properties(${the_target} PROPERTIES
	    VERSION ${SHIPS_VERSION}
	    SOVERSION ${SHIPS_SOVERSION}
	    OUTPUT_NAME "${the_target}${SHIPS_DLLVERSION}"
	    )

	# Additional target properties
	set_target_properties(${the_target} PROPERTIES
	    DEBUG_POSTFIX "${SHIPS_DEBUG_POSTFIX}"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
	    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	    )

	# Add the required libraries for linking:
	target_link_libraries(${the_target} ${SHIPS_LINKER_LIBS} ${IPP_LIBS} ${ARGN})

	#if(MSVC)
	#    if(CMAKE_CROSSCOMPILING)
	#        set_target_properties(${the_target} PROPERTIES
	#            LINK_FLAGS "/NODEFAULTLIB:secchk"
	#            )
	#    endif()
	#    set_target_properties(${the_target} PROPERTIES
	#        LINK_FLAGS "/NODEFAULTLIB:libc"
	#        )
	#endif()

	# Dependencies of this target:
	add_dependencies(${the_target} ${ARGN})

	install(TARGETS ${the_target}
	    RUNTIME DESTINATION bin COMPONENT main
	    LIBRARY DESTINATION lib COMPONENT main
	    ARCHIVE DESTINATION lib COMPONENT main)

	install(FILES ${lib_hdrs}
	    DESTINATION include/ships/${name}
	    COMPONENT main)

endmacro()
