cmake_minimum_required(VERSION 3.1)

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 11)

##########util_lib start##########

aux_source_directory("src/util" UTIL_DIR_SRCS)
add_library("MagicCube_util" ${UTIL_DIR_SRCS})

# add asio support
find_library(ASIO REQUIRED)
if (NOT MSVC)
	include_directories(${Asio_INCLUDE_DIRS})
endif()

##########util_lib end##########