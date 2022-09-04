project(helloworld)

include_directories("${CMAKE_SOURCE_DIR}/parse")
#link_directories("${CMAKE_SOURCE_DIR}/tcp")

aux_source_directory(. PARSE_SRCS)

add_library(parse ${PARSE_SRCS})