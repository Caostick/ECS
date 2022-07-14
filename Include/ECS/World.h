#pragma once

#include <ECS/EntityView.h>
#include <ECS/Common.h>
#include <ECS/Component.h>
#include <ECS/Entity.h>
#include <ECS/EntityViewConstructor.h>
#include <ECS/Group.h>
#include <ECS/WorldTLS.h>

namespace ecs {
	class System;
	class World;
	class ThreadManager;
}

namespace ecs {
	using SystemUpdateFunction = void(ecs::World& world, ecs::System& system, float deltaTime);

	struct SystemInfo {
		SystemInfo() = default;
		SystemInfo(const SystemInfo&) = default;
		SystemInfo(SystemInfo&&) = default;
		SystemInfo& operator = (const SystemInfo&) = default;
		SystemInfo& operator = (SystemInfo&&) = default;

		ecs::System* m_System = nullptr;
		ecs::SystemUpdateFunction* m_UpdateFunction = nullptr;
	};

	class World {
		template<typename, typename, typename, typename>
		friend struct Query;
	public:
		void Init(const ecs::ThreadManager* threadManager = nullptr);
		void Release();

		auto CreateEntity() -> ecs::EntityHandle;
		void DestroyEntity(ecs::EntityHandle entityHandle);

		template<typename TEntityType>
		auto Construct()->EntityViewConstructor<TEntityType, typename TEntityType::TypeList>;

		template<typename T, typename ...TArgs>
		void RegisterSystem(TArgs&&... args);

		template<typename T, typename ...TArgs>
		auto AttachComponent(ecs::EntityHandle entityHandle, TArgs&&... args) -> T&;

		template<typename T>
		void DetachComponent(ecs::EntityHandle entityHandle);

		template<typename T>
		T& GetComponent(ecs::EntityHandle entityHandle);

		template<typename T>
		bool IsAdded(ecs::EntityHandle entityHandle) const;

		template<typename T>
		bool IsRemoved(ecs::EntityHandle entityHandle) const;

		void Update(float dt);
		void ExecuteCommands();
		void FinishAllCommands();

	public:
		uint32_t GetGroupEntityCount(const ecs::Bitset& typeBitmask) const;
		ecs::EntityHandle GetEntityBackreference(ecs::EntityHandle entityHandle) const;

	private:
		template<typename T>
		static void UpdateSystem(ecs::World& world, ecs::System& system, float deltaTime);

	private:
		void ExecuteChangeEntityLayout(ecs::EntityHandle entityHandle);

		ecs::Group* GetGroup(const ecs::Bitset& typeBitmask);

		const ecs::ThreadManager* m_ThreadManager = nullptr;
		ecs::ThreadManager* m_DefaultThreadManager = nullptr;

		ecs::Vector<ecs::WorldTLS> m_TLS;
		ecs::Vector<ecs::Group*> m_Groups;

		ecs::Vector<ecs::EntityHandle> m_EntitiesToDestroy;
		ecs::Vector<ecs::EntityHandle> m_EntitiesToChangeLayout;
		ecs::Vector<ecs::EntityHandle> m_EntitiesToRefresh;

		ecs::Vector<ecs::SystemInfo> m_SystemInfos;
	};
}

#include <ECS/World.inl>