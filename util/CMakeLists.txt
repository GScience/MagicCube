cmake_minimum_required(VERSION 3.1)

##########util_lib start##########
aux_source_directory("src/util" UTIL_DIR_SRCS)
add_library("MagicCube_util" ${UTIL_DIR_SRCS})

# find package
if (MSVC)
	find_path(Result NAMES asio.hpp)
else()
	find_package(ASIO REQUIRED)
	include_directories(${Asio_INCLUDE_DIRS})
endif()
##########util_lib end##########