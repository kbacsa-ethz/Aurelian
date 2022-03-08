cmake_minimum_required(VERSION 2.8)

project(moloc2)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include SDL_PATH)
file(GLOB SOURCES
    include/*.h
    src/*.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
