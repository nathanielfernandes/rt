cmake_minimum_required(VERSION 3.10)

# set the project name
project(rt)

file(GLOB_RECURSE rt_src CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(rt ${rt_src})

target_include_directories(rt PUBLIC ./include)

target_include_directories(rt PUBLIC ./build/vendor/include)

# SDL2
target_link_libraries(rt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/vendor/lib/libSDL2.dylib)

set_property(TARGET rt PROPERTY CXX_STANDARD 20)