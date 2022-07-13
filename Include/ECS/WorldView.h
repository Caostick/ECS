#pragma once

#include <ECS/Include/ECS/World.h>

namespace ecs {
	template<typename... ARGS>
	class WorldView {
		template<typename, typename, typename, typename>
		friend struct Query;
	public:
		WorldView(ecs::World& world);

		auto CreateEntity() -> ecs::EntityHandle;
		void DestroyEntity(ecs::EntityHandle entityHandle);

		template<typename T, typename ...TArgs>
		auto AttachComponent(ecs::EntityHandle entityHandle, TArgs&&... args) -> T&;

		template<typename T>
		void DetachComponent(ecs::EntityHandle entityHandle);

		template<typename T>
		auto GetComponent(ecs::EntityHandle entityHandle) -> T&;

	protected:
		operator ecs::World& ();

	private:
		ecs::World& m_World;
	};
}

#include <ECS/Source/WorldView.inl>