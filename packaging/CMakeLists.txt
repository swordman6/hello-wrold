project(helloworld)

include_directories("${CMAKE_SOURCE_DIR}/packaging")
include_directories("/usr/local/include")
link_directories("/usr/local/lib")

aux_source_directory(. PACKAGING_SRCS)

add_library(packaging ${PACKAGING_SRCS})

target_link_libraries(packaging avfilte avformat avcodec 
                                swscale avutil swresample
                                z #z库 ffmpeg用到
 
                                #bz2库
                                libbz2.dylib
                                libiconv.dylib)
