##########block_lib start##########
# create library
aux_source_directory("src/block" BLOCK_DIR_SRCS)
add_library("MagicCube_block" ${BLOCK_DIR_SRCS})
##########block_lib start##########