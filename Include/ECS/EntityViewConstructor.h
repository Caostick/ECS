#pragma once

#include <ECS/World.h>
#include <ECS/EntityView.h>
#include <ECS/Common.h>
#include <ECS/Entity.h>

namespace ecs {
	class World;
}

namespace ecs {
	template<typename TEntityType, typename TL, typename Enable = void>
	class EntityViewConstructor;

	template<typename TEntityType, typename TL>
	class EntityViewConstructor<TEntityType, TL, typename std::enable_if<ecs::Length<TL>::value == 1>::type> {
	public:
		EntityViewConstructor(World& world, EntityHandle entityHandle);
		EntityViewConstructor(World& world, EntityHandle entityHandle, ComponentArray<typename TEntityType::TypeList>& components);

		template<typename T, typename ...TArgs>
		auto Construct(TArgs&&... args) -> const TEntityType;

	private:
		World& m_World;
		EntityHandle m_EntityHandle;
		ComponentArray<typename TEntityType::TypeList> m_Components;
	};

	template<typename TEntityType, typename TL>
	class EntityViewConstructor<TEntityType, TL, typename std::enable_if<ecs::Length<TL>::value != 1>::type> {
	public:
		EntityViewConstructor(World& world, EntityHandle entityHandle);
		EntityViewConstructor(World& world, EntityHandle entityHandle, ComponentArray<typename TEntityType::TypeList>& components);

		template<typename T, typename ...TArgs>
		auto Construct(TArgs&&... args)->EntityViewConstructor<TEntityType, typename ecs::RemoveAll<T, TL>::type>;

	private:
		World& m_World;
		EntityHandle m_EntityHandle;
		ComponentArray<typename TEntityType::TypeList> m_Components;
	};
}

#include <ECS/EntityViewConstructor.inl>