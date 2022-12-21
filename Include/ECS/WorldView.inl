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
auto ecs::WorldView<Args...>::GetComponent(EntityHandle entityHandle)->T& {
	static_assert(Contains<T, TypeList<Args...>>::value, "WorldView doesn't contain component!");

	return m_World.GetComponent<T>(entityHandle);
}

template<typename... Args>
ecs::WorldView<Args...>::operator ecs::World& () {
	return m_World;
}