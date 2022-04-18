cmake_minimum_required(VERSION 2.8)

project(aurelian)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_libraries(dl)
link_libraries(m)
link_libraries(OpenGL::GL)
link_libraries(glfw)

include_directories(include)

file(GLOB SOURCES
    include/*.h
    include/stb/*.h
    src/*.c
    )

file(GLOB RES_FILES resource/shaders/*.frag resource/shaders/*.vert resource/textures/*)

file(COPY ${RES_FILES}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(aurelian ${RES_FILES} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE cglm)
