##########client_lib start##########
# find packages
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

if (MSVC)
	find_library(SDL2_IMAGE_LIBRARY SDL2_image)
	find_library(dirent REQUIRED)
	set(CLIENT_LINK_LIBS GLEW::GLEW SDL2::SDL2 glm ${OPENGL_gl_LIBRARY} ${SDL2_IMAGE_LIBRARY})
	set(CLIENT_INCLUDE_DIRS)
else()
	find_package(SDL2_image REQUIRED)
	set(CLIENT_LINK_LIBS ${GLEW_LIBRARIES} ${OPENGL_gl_LIBRARY} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY})
	set(CLIENT_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIR})
endif()

list(APPEND CLIENT_LINK_LIBS MagicCube_block MagicCube_util MagicCube_server)

# add include dir
list(APPEND CLIENT_INCLUDE_DIRS "src/block" "src/client" "src/server" "src/util" "src/server/net_package")

include_directories(${CLIENT_INCLUDE_DIRS})

# create client lib
aux_source_directory("src/client" CLIENT_DIR_SRCS)
add_library("MagicCube_client_lib" ${CLIENT_DIR_SRCS} ${CLIENT_NET_PACKAGE_DIR_SRCS})

target_link_libraries("MagicCube_client_lib" PRIVATE ${CLIENT_LINK_LIBS})

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


##########client start##########
# create runnable client
add_executable("MagicCube_client" "src/ClientMain.cpp")

if (MSVC)
	target_link_libraries("MagicCube_client" MagicCube_client_lib SDL2::SDL2main)
else()
	target_link_libraries("MagicCube_client" MagicCube_client_lib)
endif()
##########client end##########


##########client_test start##########
# find packages
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# create executable
aux_source_directory("src/client_test" CLIENT_TEST_DIR_SRCS)
add_executable(MagicCube_client_test ${CLIENT_TEST_DIR_SRCS})

# pthread check
if(THREADS_HAVE_PTHREAD_ARG)
	set_property(TARGET MagicCube_client_test PROPERTY COMPILE_OPTIONS "-pthread")
	set_property(TARGET MagicCube_client_test PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

# link
if (MSVC)
	target_link_libraries(MagicCube_client_test PRIVATE MagicCube_client_lib MagicCube_block GTest::GTest GTest::Main ${CMAKE_THREAD_LIBS_INIT})
else()
	include_directories(${GTEST_INCLUDE_DIRS})
	target_link_libraries(MagicCube_client_test PRIVATE MagicCube_client_lib MagicCube_block ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

# add test
add_test(ClientTest bin/MagicCube_client_test)
##########client_test end##########