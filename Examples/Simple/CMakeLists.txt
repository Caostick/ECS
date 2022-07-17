cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME Simple)

project(${PROJECT_NAME})

FILE(GLOB _FILES_ ./*.h ./*.cpp)
add_executable(${PROJECT_NAME} ${_FILES_})

find_library(ECS ECS)
target_link_libraries (${PROJECT_NAME} ECS)
include_directories(../../Include/)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Examples")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")