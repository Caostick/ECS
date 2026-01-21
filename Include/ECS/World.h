#pragma once

#include <ECS/EntityView.h>
#include <ECS/Common.h>
#include <ECS/Bitset.h>
#include <ECS/Component.h>
#include <ECS/Entity.h>
#include <ECS/Group.h>
#include <ECS/WorldTLS.h>
#include <ECS/QueryCache.h>

namespace ecs {
	class System;
	class ThreadManager;
}

namespace ecs {
	using SystemUpdateFunction = void(class World& world, System& system, float deltaTime);

	struct SystemInfo {
		SystemInfo() = default;
		SystemInfo(const SystemInfo&) = default;
		SystemInfo(SystemInfo&&) = default;
		SystemInfo& operator = (const SystemInfo&) = default;
		SystemInfo& operator = (SystemInfo&&) = default;

		System* m_System = nullptr;
		SystemUpdateFunction* m_UpdateFunction = nullptr;
		Bitset m_ComponentAccessMask;
		Bitset m_MutableComponentAccessMask;

		bool IsParallelCompatible(const SystemInfo& other) const;
	};

	class World {
		template<typename, typename, typename, typename>
		friend struct Query;

		template<typename, typename>
		friend class QueryBatch;

	public:
		void Init(const ThreadManager* threadManager = nullptr);
		void Release();

		auto CreateEntity() -> EntityHandle;
		void DestroyEntity(EntityHandle entityHandle);

		template<typename T, typename ...TArgs>
		void RegisterSystem(TArgs&&... args);

		template<typename T, typename ...TArgs>
		auto AttachComponent(EntityHandle entityHandle, TArgs&&... args) -> T&;

		template<typename T>
		void DetachComponent(EntityHandle entityHandle);

		template<typename T>
		auto GetComponent(EntityHandle entityHandle) -> T&;

		template<typename T>
		bool HasComponent(EntityHandle entityHandle) const;

		template<typename T>
		bool IsAdded(EntityHandle entityHandle) const;

		template<typename T>
		bool IsRemoved(EntityHandle entityHandle) const;

		void Update(float dt);
		void ExecuteCommands();
		void FinishAllCommands();

	public:
		auto GetGroupEntityCount(const Bitset& typeBitmask) const -> uint32_t;
		auto GetEntityBackreference(EntityHandle entityHandle) const -> EntityHandle;

	private:
		template<typename T>
		static void UpdateSystem(World& world, System& system, float deltaTime);

	private:
		void ExecuteChangeEntityLayout(EntityHandle entityHandle);

		auto GetGroup(const Bitset& typeBitmask) -> Group*;
		auto GetQueryCache(uint32_t id) -> QueryCache&;
		void InitQueryCache(QueryCache& queryCache);
		void TryCacheGroup(QueryCache& queryCache, Group* group);

		const ThreadManager* m_ThreadManager = nullptr;
		ThreadManager* m_DefaultThreadManager = nullptr;

		Vector<WorldTLS> m_TLS;
		Vector<Group*> m_Groups;
		Vector<QueryCache> m_Queries;

		Vector<EntityHandle> m_EntitiesToDestroy;
		Vector<EntityHandle> m_EntitiesToChangeLayout;
		Vector<EntityHandle> m_EntitiesToRefresh;

		Vector<SystemInfo> m_SystemInfos;
	};
}

#include <ECS/World.inl>