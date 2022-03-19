cmake_minimum_required(VERSION 2.8)

project(aurelian)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_libraries(dl)
link_libraries(OpenGL::GL)
link_libraries(glfw)

include_directories(include)

file(GLOB SOURCES
    include/*.h
    src/*.c
    )

add_executable(aurelian ${SOURCES})
