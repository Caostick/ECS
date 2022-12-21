#pragma once

#include <ECS/World.h>

namespace ecs {
	template<typename... ARGS>
	class WorldView {
		template<typename, typename, typename, typename>
		friend struct Query;
	public:
		WorldView(World& world);

		auto CreateEntity() -> EntityHandle;
		void DestroyEntity(EntityHandle entityHandle);

		template<typename T, typename ...TArgs>
		auto AttachComponent(EntityHandle entityHandle, TArgs&&... args) -> T&;

		template<typename T>
		void DetachComponent(EntityHandle entityHandle);

		template<typename T>
		auto GetComponent(EntityHandle entityHandle) -> T&;

	protected:
		operator ecs::World& ();

	private:
		World& m_World;
	};
}

#include <ECS/WorldView.inl>