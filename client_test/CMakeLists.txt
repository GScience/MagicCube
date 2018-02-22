cmake_minimum_required(VERSION 3.1)

enable_testing()

# set output dir
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set cxx version
set (CMAKE_CXX_STANDARD 11)

# add lib
find_package(GTest REQUIRED)

# add include dir
include_directories("src/client")

# create executable
aux_source_directory("src/client_test" CLIENT_TEST_DIR_SRCS)
add_executable(MagicCube_client_test ${CLIENT_TEST_DIR_SRCS})

# pthread
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
	set_property(TARGET MagicCube_client_test PROPERTY COMPILE_OPTIONS "-pthread")
	set_property(TARGET MagicCube_client_test PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

# link
if (NOT MSVC)
	include_directories(${GTEST_INCLUDE_DIRS})
	target_link_libraries(MagicCube_client_test PRIVATE MagicCube_client_lib MagicCube_block ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
else()
	target_link_libraries(MagicCube_client_test PRIVATE MagicCube_client_lib MagicCube_block GTest::GTest GTest::Main ${CMAKE_THREAD_LIBS_INIT})
endif()

# setup test
add_test(ClientTest bin/MagicCube_client_test)