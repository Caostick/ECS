cmake_minimum_required (VERSION 3.1)

option(ECS_TESTS "Enable build of tests" OFF)
option(ECS_EXAMPLE "Enable build of example" OFF)

project(ECS)

set(ECS_INCLUDE_LIST
	Include/ECS/Assert.h
	Include/ECS/Bitset.h
	Include/ECS/CommandBuffer.h
	Include/ECS/CommandBuffer.inl
	Include/ECS/Common.h
	Include/ECS/Component.h
	Include/ECS/Component.inl
	Include/ECS/Entity.h
	Include/ECS/EntityHandle.h
	Include/ECS/EntityView.h
	Include/ECS/EntityView.inl
	Include/ECS/EntityViewConstructor.h
	Include/ECS/EntityViewConstructor.inl
	Include/ECS/Group.h
	Include/ECS/Group.inl
	Include/ECS/Query.h
	Include/ECS/Query.inl
	Include/ECS/QueryCache.h
	Include/ECS/System.h
	Include/ECS/System.inl
	Include/ECS/ThreadManager.h
	Include/ECS/TypeList.h
	Include/ECS/Vector.h
	Include/ECS/World.h
	Include/ECS/World.inl
	Include/ECS/WorldTLS.h
	Include/ECS/WorldView.h
	Include/ECS/WorldView.inl
)

set(ECS_SRC_LIST
	Source/CommandBuffer.cpp
	Source/Common.cpp
	Source/Component.cpp
	Source/Group.cpp
	Source/Query.cpp
	Source/World.cpp
	Source/WorldTLS.cpp
)

add_library (ECS STATIC ${ECS_INCLUDE_LIST} ${ECS_SRC_LIST})

target_include_directories(ECS PUBLIC Include/)

set_property(TARGET ECS PROPERTY CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")

if(ECS_TESTS)
	add_subdirectory(Tests)
endif()

if(ECS_EXAMPLE)
	add_subdirectory(Examples)
endif()