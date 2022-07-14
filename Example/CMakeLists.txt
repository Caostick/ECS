cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME Example)

project(${PROJECT_NAME})

FILE(GLOB SRC_FILES ./*.h ./*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

find_library(ECS ECS)
target_link_libraries (${PROJECT_NAME} ECS)
include_directories(../Include/)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")