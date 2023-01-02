#pragma once

#include <ECS/TypeList.h>

template<typename... Args>
ecs::WorldView<Args...>::WorldView(World& world)
	: m_World(world) {
}

template<typename... Args>
auto ecs::WorldView<Args...>::CreateEntity()->EntityHandle {
	return m_World.CreateEntity();
}

template<typename... Args>
void ecs::WorldView<Args...>::DestroyEntity(EntityHandle entityHandle) {
	m_World.DestroyEntity(entityHandle);
}

template<typename... Args>
template<typename T, typename ...TArgs>
auto ecs::WorldView<Args...>::AttachComponent(EntityHandle entityHandle, TArgs&&... args)->T& {
	return m_World.AttachComponent<T>(entityHandle, std::forward<TArgs>(args)...);
}

template<typename... Args>
template<typename T>
void ecs::WorldView<Args...>::DetachComponent(EntityHandle entityHandle) {
	m_World.DetachComponent<T>(entityHandle);
}

template<typename... Args>
template<typename T>
auto ecs::WorldView<Args...>::GetComponent(EntityHandle entityHandle) -> typename std::conditional_t<ecs::Contains<std::remove_const_t<T>, TL>::value, T&, const T&> {
	static_assert(Contains<const T, TL>::value, "Component of type is inaccessible!");
	static_assert(!std::is_const_v<T>, "Modifier const is deprecated!");
	static_assert(!std::is_empty<T>::value, "Can't access empty component!");

	return m_World.GetComponent<T>(entityHandle);
}

template<typename... Args>
template<typename T>
bool ecs::WorldView<Args...>::HasComponent(EntityHandle entityHandle) const {
	static_assert(Contains<const T, TL>::value, "WorldView doesn't contain component!");

	return m_World.HasComponent<T>(entityHandle);
}

template<typename... Args>
ecs::WorldView<Args...>::operator ecs::World& () {
	return m_World;
}