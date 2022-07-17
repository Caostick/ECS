#pragma once

template<typename T>
T* ecs::Group::GetComponent(uint32_t localIndex) {
	return reinterpret_cast<T*>(GetComponentData(localIndex, ecs::Component<T>::GetTypeId()));
}