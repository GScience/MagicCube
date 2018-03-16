cmake_minimum_required(VERSION 3.1)

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 14)

#create project
project(MagicCube)

include("util/CMakeLists.txt")
include("block/CMakeLists.txt")
include("server/CMakeLists.txt")
include("client/CMakeLists.txt")