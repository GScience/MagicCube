cmake_minimum_required(VERSION 3.1)

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 11)

# create executable
aux_source_directory("src/client" CLIENT_DIR_SRCS)
add_executable("MagicCube_client" ${CLIENT_DIR_SRCS})

# for test
set_target_properties("MagicCube_client" PROPERTIES ENABLE_EXPORTS ON)

# add lib
if (NOT MSVC)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
endif()

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

if (NOT MSVC)
	find_package(SDL2_image REQUIRED)
	include_directories(${SDL2_INCLUDE_DIRS})
	include_directories(${SDL2IMAGE_INCLUDE_DIR})
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries("MagicCube_client" PRIVATE ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARY})
else()
	find_library(SDL2_IMAGE_LIBRARY SDL2_image)
	find_library(dirent REQUIRED)
	target_link_libraries("MagicCube_client" PRIVATE GLEW::GLEW SDL2::SDL2 SDL2::SDL2main ${OPENGL_gl_LIBRARY} ${SDL2_IMAGE_LIBRARY})
endif()

# copy resources
if (MSVC)
	add_custom_command(TARGET "MagicCube_client" POST_BUILD
					  COMMAND ${CMAKE_COMMAND} -E copy_directory
					  "${PROJECT_SOURCE_DIR}/resource"
					  "${CMAKE_BINARY_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}")
else()
	add_custom_command(TARGET "MagicCube_client" POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
					   "${PROJECT_SOURCE_DIR}/resource"
					   "${CMAKE_BINARY_DIR}/bin")
endif()