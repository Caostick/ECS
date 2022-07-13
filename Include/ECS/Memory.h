#pragma once

namespace ecs {
	template<typename T, typename ...TArgs>
	void Construct(T* ptr, TArgs&&... args);
}

#include <ECS/Memory.inl>