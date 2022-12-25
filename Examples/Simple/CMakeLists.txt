cmake_minimum_required (VERSION 3.1)

project(SimpleExample)

FILE(GLOB _FILES_ ./*.h ./*.cpp)
add_executable(SimpleExample ${_FILES_})

find_library(ECS ECS)
target_link_libraries (SimpleExample ECS)

set_property(TARGET SimpleExample PROPERTY CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")