#pragma once

template<typename T, typename ...TArgs>
void ecs::Construct(T* ptr, TArgs&&... args) {
	new(ptr) T(std::forward<TArgs>(args)...);
}