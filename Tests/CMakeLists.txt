cmake_minimum_required(VERSION 3.5)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

FILE(GLOB TestApp ./TestApp/App.cpp)
FILE(GLOB Tests ./Tests/*.cpp ./Tests/*.h)
add_executable(ut ${TestApp} ${Tests})

find_library(ECS ECS)
target_link_libraries (ut ECS)
include_directories(../Include/)
  
target_link_libraries(ut CONAN_PKG::catch2)
  
add_test(NAME ut COMMAND ut)

set_property(TARGET ut PROPERTY CXX_STANDARD 17)