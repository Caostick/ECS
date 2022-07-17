cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME DemoScene)

project(${PROJECT_NAME})

FILE(GLOB _FILES_ ./*.h ./*.cpp)
add_executable(${PROJECT_NAME} ${_FILES_})

find_library(ECS ECS)
target_link_libraries (${PROJECT_NAME} ECS)
include_directories(../../Include/)

include(../../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

target_link_libraries(${PROJECT_NAME} CONAN_PKG::glfw)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::glew)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::glm)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Examples")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")