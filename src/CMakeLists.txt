project(helloworld)

include_directories("${CMAKE_SOURCE_DIR}/colorcv")
include_directories("${CMAKE_SOURCE_DIR}/packaging")
include_directories("${CMAKE_SOURCE_DIR}/parse")

link_directories("${CMAKE_SOURCE_DIR}/libs")

aux_source_directory(. M_SRCS)

add_executable(app ${M_SRCS})

target_link_libraries(app colorcv
                          #packaging
                          parse)