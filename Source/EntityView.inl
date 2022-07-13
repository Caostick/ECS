#pragma once

template<typename... TArgs>
template<typename T>
auto ecs::EntityView<TArgs...>::Get() const -> T& {
	static_assert(ecs::Contains<T, TypeList>::value || ecs::Contains<typename std::remove_const<T>::type, TypeList>::value, "Component of type is inaccessible!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");
	constexpr int idx = ecs::Find<std::remove_const_t<T>, TypeList>::value;

	return *(T*)(m_Components.m_Pointers[idx]);
}

template<typename... TArgs>
ecs::EntityView<TArgs...>::operator ecs::EntityHandle() const {
	return m_EntityHandle;
}

template<typename... TArgs>
ecs::EntityView<TArgs...>::EntityView(ecs::EntityHandle handle, const ComponentArray<TypeList>& components)
	: m_EntityHandle(handle) 
	, m_Components(components) {}