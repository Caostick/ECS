cmake_minimum_required (VERSION 3.1)

option(ECS_TESTS "Enable build of tests" ON)
option(ECS_EXAMPLE "Enable build of example" ON)

set(PROJECT_NAME ECS)
set(CPP_VERSION 17)

project(${PROJECT_NAME})

set_property(GLOBAL PROPERTY ECS_TMP_SRC_LIST)
function(ECS_GATHER_FILES DIRNAME DIR)
	FILE(GLOB ECS_TMP_FILES ../${PROJECT_NAME}/${DIR}/*.h ../${PROJECT_NAME}/${DIR}/*.cpp)
	source_group(${DIRNAME} FILES ${ECS_TMP_FILES})
	get_property(ECS_LOCAL_SRC_LIST GLOBAL PROPERTY ECS_TMP_SRC_LIST)	
	set_property(GLOBAL PROPERTY ECS_TMP_SRC_LIST ${ECS_LOCAL_SRC_LIST} ${ECS_TMP_FILES})
endfunction()


set_property(GLOBAL PROPERTY TMP_SRC_LIST)

# === BEGIN FILES ===
ECS_GATHER_FILES("Include\\ECS" Include/ECS)
ECS_GATHER_FILES("Source" Source)
# === END FILES ===

get_property(ECS_SRC_LIST GLOBAL PROPERTY ECS_TMP_SRC_LIST)
add_library (${PROJECT_NAME} STATIC ${ECS_SRC_LIST})

include_directories(Include/)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "ECS")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

if(ECS_TESTS)
add_subdirectory(Tests)
endif()

if(ECS_EXAMPLE)
add_subdirectory(Examples)
endif()