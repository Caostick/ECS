cmake_minimum_required (VERSION 3.1)

project(ECSDemoScene)

FILE(GLOB _FILES_ ./*.h ./*.cpp)
add_executable(ECSDemoScene ${_FILES_})

find_library(ECS ECS)
target_link_libraries (ECSDemoScene ECS)
include_directories(../../Include/)

include(../../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

target_link_libraries(ECSDemoScene CONAN_PKG::glfw)
target_link_libraries(ECSDemoScene CONAN_PKG::glew)
target_link_libraries(ECSDemoScene CONAN_PKG::glm)

set_property(TARGET ECSDemoScene PROPERTY CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")