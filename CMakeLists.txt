cmake_minimum_required(VERSION 3.10)

# set the project name
project(rt)

file(GLOB_RECURSE rt_src CONFIGURE_DEPENDS src/*.cpp src/*.hpp build/vendor/dir/*)

add_executable(rt ${rt_src})

target_include_directories(rt PUBLIC /usr/local/include)

target_include_directories(rt PUBLIC ./include)
target_include_directories(rt PUBLIC ./build/vendor/include)
target_include_directories(rt PUBLIC ./build/vendor/dir)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(./submodules/glfw)

target_link_libraries(rt PUBLIC glfw)

# SDL2
# target_link_libraries(rt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/vendor/lib/libSDL2.dylib)
target_link_libraries(rt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/vendor/lib/libglew.a)

find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
    message("opengl found")
else(OPENGL_FOUND)
    message("opengl not found")
endif()

target_link_libraries(rt PUBLIC ${OPENGL_gl_LIBRARY})

set_property(TARGET rt PROPERTY CXX_STANDARD 20)