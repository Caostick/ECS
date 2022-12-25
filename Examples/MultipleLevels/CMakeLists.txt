cmake_minimum_required (VERSION 3.1)

project(MultipleLevelsExample)

FILE(GLOB _FILES_ ./*.h ./*.cpp)
add_executable(MultipleLevelsExample ${_FILES_})

find_library(ECS ECS)
target_link_libraries (MultipleLevelsExample ECS)

set_property(TARGET MultipleLevelsExample PROPERTY CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")