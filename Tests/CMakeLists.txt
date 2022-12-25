cmake_minimum_required(VERSION 3.5)

include(../ThirdParty/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS TARGETS)

FILE(GLOB TestApp ./TestApp/App.cpp)
FILE(GLOB Tests ./Tests/*.cpp ./Tests/*.h)
add_executable(ECS_ut ${TestApp} ${Tests})

find_library(ECS ECS)
target_link_libraries (ECS_ut ECS)

target_link_libraries(ECS_ut CONAN_PKG::catch2)

enable_testing()
add_test(NAME ECS_ut COMMAND ECS_ut)

set_property(TARGET ECS_ut PROPERTY CXX_STANDARD 17)