#pragma once

template<typename T>
auto ecs::Group::GetComponent(uint32_t localIndex) -> T* {
	return reinterpret_cast<T*>(GetComponentData(localIndex, Component<T>::GetTypeId()));
}