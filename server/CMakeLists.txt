cmake_minimum_required(VERSION 3.1)

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 11)

# create executable
aux_source_directory("src/server" BLOCK_DIR_SRCS)
add_library("MagicCube_server" ${BLOCK_DIR_SRCS})

find_library(asio REQUIRED)

list (APPEND SERVER_LINK_LIBS "MagicCube_util")

list(APPEND SERVER_INCLUDE_DIRS "src/block" "src/client" "src/server" "src/util" "src/server/net_package")

include_directories(${SERVER_INCLUDE_DIRS})
target_link_libraries("MagicCube_server" PRIVATE ${SERVER_LINK_LIBS})