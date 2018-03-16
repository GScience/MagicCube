cmake_minimum_required(VERSION 3.1)

# create executable
aux_source_directory("src/block" BLOCK_DIR_SRCS)
add_library("MagicCube_block" ${BLOCK_DIR_SRCS})