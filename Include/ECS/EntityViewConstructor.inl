#pragma once

#include <ECS/World.h>

template<typename TEntityType, typename TL>
ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value == 1>>::EntityViewConstructor(World& world, EntityHandle entityHandle)
	: m_World(world)
	, m_EntityHandle(entityHandle) {
	for (auto& ci : m_Components.m_Pointers) { ci = nullptr; }
}

template<typename TEntityType, typename TL>
ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value == 1>>::EntityViewConstructor(World& world, EntityHandle entityHandle, ComponentArray<typename TEntityType::TypeList>& components)
	: m_World(world)
	, m_EntityHandle(entityHandle)
	, m_Components(components) {}

template<typename TEntityType, typename TL>
template<typename T, typename ...TArgs>
auto ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value == 1>>::Construct(TArgs&&... args) -> const TEntityType {
	static_assert(Contains<T, TL>::value, "Component is not available for construction!");
	constexpr int idx = Find<std::remove_const_t<T>, typename TEntityType::TypeList>::value;

	m_Components.m_Pointers[idx] = &m_World.AttachComponent<T>(m_EntityHandle, std::forward<TArgs>(args)...);

	return TEntityType(m_EntityHandle, m_Components);
}




template<typename TEntityType, typename TL>
ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value != 1>>::EntityViewConstructor(World& world, EntityHandle entityHandle)
	: m_World(world)
	, m_EntityHandle(entityHandle) {
	for (auto& ci : m_Components.m_Pointers) { ci = nullptr; }
}

template<typename TEntityType, typename TL>
ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value != 1>>::EntityViewConstructor(World& world, EntityHandle entityHandle, ComponentArray<typename TEntityType::TypeList>& components)
	: m_World(world)
	, m_EntityHandle(entityHandle)
	, m_Components(components) {}

template<typename TEntityType, typename TL>
template<typename T, typename ...TArgs>
auto ecs::EntityViewConstructor<TEntityType, TL, typename std::enable_if_t<ecs::Length<TL>::value != 1>>::Construct(TArgs&&... args) ->EntityViewConstructor<TEntityType, typename RemoveAll<T, TL>::type> {
	static_assert(Contains<T, TL>::value, "Component is not available for construction!");
	constexpr int idx = Find<std::remove_const_t<T>, typename TEntityType::TypeList>::value;

	if constexpr (std::is_empty<T>::value) {
		m_World.AttachComponent<T>(m_EntityHandle, std::forward<TArgs>(args)...);
	} else {
		m_Components.m_Pointers[idx] = &m_World.AttachComponent<T>(m_EntityHandle, std::forward<TArgs>(args)...);
	}

	return EntityViewConstructor<TEntityType, typename RemoveAll<T, TL>::type>(m_World, m_EntityHandle, m_Components);
}

template<typename TEntityType>
auto ecs::Construct(World& world)->EntityViewConstructor<TEntityType, typename TEntityType::TypeList> {
	return EntityViewConstructor<TEntityType, typename TEntityType::TypeList>(world, world.CreateEntity());
}