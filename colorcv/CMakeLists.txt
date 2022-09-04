project(helloworld)

include_directories("${CMAKE_SOURCE_DIR}/colorcv")
#link_directories("${CMAKE_SOURCE_DIR}/tcp")

aux_source_directory(. COLORCV_SRCS)

add_library(colorcv ${COLORCV_SRCS})