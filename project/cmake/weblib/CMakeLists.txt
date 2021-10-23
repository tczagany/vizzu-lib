include(../common.txt)
include(../link.txt)

set(appsDir ${root}/src/apps)

file(GLOB_RECURSE sources ${appsDir}/weblib/*.cpp)
file(GLOB_RECURSE headers ${appsDir}/weblib/*.h)

if(EMSCRIPTEN)
	add_executable (cvizzu ${sources})

	if(${NODEJS})
		add_custom_command(TARGET cvizzu PRE_BUILD
			COMMAND node
			"${CMAKE_CURRENT_LIST_DIR}/envswitch.js"
			"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api"
			"NODE")
	else()
		add_custom_command(TARGET cvizzu PRE_BUILD
			COMMAND node
			"${CMAKE_CURRENT_LIST_DIR}/envswitch.js"
			"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api"
			"ES6")
	endif()

	target_link_libraries (cvizzu LINK_PUBLIC vizzulib)
else()
	add_library(cvizzu ${sources})
endif()

add_dependencies(cvizzu vizzulib)

include(../includes.txt)

if(EMSCRIPTEN)
	include(../emcc.txt)
	include(emcc.txt)

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"cvizzu.*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example/lib")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api/tmp/*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example/lib")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"cvizzu.*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example-node/lib")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api/tmp/*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example-node/lib")

endif()
