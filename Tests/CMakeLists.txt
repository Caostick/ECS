cmake_minimum_required(VERSION 3.5)

include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.9
)
FetchContent_MakeAvailable(catch2)

FILE(GLOB TestApp ./TestApp/App.cpp)
FILE(GLOB Tests ./Tests/*.cpp ./Tests/*.h)
add_executable(ECS_ut ${TestApp} ${Tests})

find_library(ECS ECS)
target_link_libraries(ECS_ut
	PRIVATE
		ECS
		Catch2::Catch2
)

enable_testing()
add_test(NAME ECS_ut COMMAND ECS_ut)

set_property(TARGET ECS_ut PROPERTY CXX_STANDARD 17)