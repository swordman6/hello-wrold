cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}
#                    -framework OpenGL 
#                    -framework AppKit 
#                    -framework Security 
#                    -framework CoreFoundation 
#                    -framework CoreVideo 
#                    -framework CoreMedia 
#                    -framework QuartzCore 
#                    -framework CoreFoundation 
#                    -framework VideoDecodeAcceleration 
#                    -framework Cocoa 
#                    -framework AudioToolbox 
#                    -framework VideoToolbox 
#                    -framework OpenCL")

set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")

add_compile_options(-Wall)
#add_compile_options(-D_POSIX_C_SOURCE=200112L)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

project(helloworld)

add_subdirectory(src)
add_subdirectory(colorcv)
#add_subdirectory(packaging)
add_subdirectory(parse)