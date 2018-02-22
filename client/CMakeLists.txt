cmake_minimum_required(VERSION 3.1)

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 11)

##########client_lib##########

# create client lib
aux_source_directory("src/client" CLIENT_DIR_SRCS)
add_library("MagicCube_client_lib" ${CLIENT_DIR_SRCS})

# find packages
if (NOT MSVC)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
endif()

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

if (MSVC)
	find_library(SDL2_IMAGE_LIBRARY SDL2_image)
	find_library(dirent REQUIRED)
else()
	find_package(SDL2_image REQUIRED)
endif()

# use packages
set(CLIENT_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIR})

if (MSVC)
	set(CLIENT_INCLUDE_LIBS MagicCube_block GLEW::GLEW SDL2::SDL2 glm)
else()
	set(CLIENT_INCLUDE_LIBS ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY} ${SDL2_BUILDING_LIBRARY} ${SDL2IMAGE_LIBRARY})
endif()

include_directories(CLIENT_INCLUDE_DIRS)
target_link_libraries("MagicCube_client_lib" PRIVATE ${CLIENT_INCLUDE_LIBS})

# add refrences
include_directories("src/block")

# copy resources
if (MSVC)
	add_custom_command(TARGET "MagicCube_client_lib" POST_BUILD
					  COMMAND ${CMAKE_COMMAND} -E copy_directory
					  "${PROJECT_SOURCE_DIR}/resource"
					  "${CMAKE_BINARY_DIR}/bin/${CMAKE_CONFIGURATION_TYPES}")
else()
	add_custom_command(TARGET "MagicCube_client_lib" POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy_directory
					   "${PROJECT_SOURCE_DIR}/resource"
					   "${CMAKE_BINARY_DIR}/bin")
endif()

##########client_lib end##########

# create runable client
if (MSVC)
	add_executable("MagicCube_client" IMPORTED SDL2::SDL2main MagicCube_client_lib)
else()
	add_executable("MagicCube_client" IMPORTED ${SDL2MAIN_LIBRARY} MagicCube_client_lib)
endif()